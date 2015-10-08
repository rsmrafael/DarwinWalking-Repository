#include <math.h>
#include "include/Board.h"
#include "tc/tc.h"
#include "uart/uart.h"
#include "dbgu/dbgu.h"
#include "adc/adc.h"
#include "imu/imu.h"
#include "movements/scenes.h"
#include "parser/parser.h"
#include "ui/ui.h"
#include "servo/servo.h"
#include "servo_pwm/servo_pwm.h"
#include "rbc/rbc.h"
#include "tests/imu.h"
#include "config/config.h"
#include "shell/shell.h"
#include "kinematics/inverse.h"
#include "movements/kinematic_scenes.h"
#include "controller/controller.h"
#include "walker/walker.h"
#include "i2c/i2c.h"
#include "movements/motionselect.h"
#include "eeprom/eeprom.h"
#include "usb/usb.h"
#include "cm730/cm730.h"
#include "movements/action.h"
#include "odometry/odometry.h"
#include "bootloader/bootloader.h"
#include "kicker/kicker.h"

#define USE_KICKENGINE	1

#ifdef DEBUG_X86
#define MAIN_LOOP_DURATION			24 // ms
#else
#define MAIN_LOOP_DURATION			10 // ms
#endif

#define GETUP_FRONT_TRIGGER			420
#define GETUP_BACK_TRIGGER			630

#define WALK_FORWARD_MAX_SPEED		20
#define WALK_SIDEWAY_MAX_SPEED		30
#define TURN_MAX_SPEED				10
#define UNIT_FB_STEP				0.3
#define UNIT_RL_STEP				UNIT_FB_STEP
#define UNIT_RL_TURN				0.1


#define STATE_MAIN					0
#define STATE_PLAYSTATIC			1
#define STATE_RETURNTOSTAY			2
#define STATE_GETUPFRONT			3
#define STATE_GETUPBACK				4
#define STATE_WALKBEGIN				5
#define STATE_WALK					6
#define STATE_WALKEND				7
#define STATE_PLAYKINEMATIC			8
#define STATE_STABILIZE				9
#define STATE_KICKER				10
#define STATE_WAITUNPARALYZE		11

#define MAX_STEPS					2

#define GO_FORWARD_CMD				1

#define WALK_START_TIMING_FACTOR	1.5
#define TIMING_FACTOR_STEP			0.02 // WALK_START_TIMING_FACTOR + (x * TIMING_FACTOR_STEP) must give 1!

//#define USE_FADE_TO_IDLE
#define FADE_TO_IDLE_INCEMENT		0.2 // °/cycle
#define FADE_PID_INCEMENT			0.1 // Step/cycle

//#define USE_UNPARALYZE_DELAY

#define SEND_STATUSDATA

#define STABILIZE_CYCLES			(1500 / MAIN_LOOP_DURATION) // ms
#define SERVO_STAT_RESET_CYCLES		(60000 / MAIN_LOOP_DURATION) // ms
#define UNPARALYZE_WAIT_CYCLES		(10000 / MAIN_LOOP_DURATION) // ms

#ifdef DYNAMIXEL
#define PLAY_SCENE		Action_Start
#define SCENE_PLAYING	Action_IsRunning
#else
#define PLAY_SCENE		Scenes_StartPlay
#define SCENE_PLAYING	Scenes_IsPlaying
#endif

#define CONTROLLER_MAIN				1
#define INSTRUCTION_STATUSMESSAGE	0x20
#define INSTRUCTION_BOOTLOADER		0x99

struct odometryData_t {
	signed short xPos;
	signed short yPos;
	signed short viewDirection;
	unsigned char weight;
} __attribute__((packed));

struct statusMessage_t {
	unsigned short voltage;
	struct servoData_t servo_status[SERVO_COUNT_MAX];
	struct imuData_t imu;
	struct odometryData_t odometry;
} __attribute__((packed)) statusMessage;

void main_loop_kinematic(void);
signed char ParseMessages(void);
void DisableWalkerControl(void);
char* GetStateName(unsigned char state);
void UpdateStatusMessage(void);

unsigned char mBlockPanTilt = 0;
unsigned char mWalkerControlAllowed = 1;

signed char mGoalFBStep = 0;
signed char mGoalRLStep = 0;
signed char mGoalRLTurn = 0;

struct vect3d mBall;
struct vect3d mTarget;

#ifdef USE_FADE_TO_IDLE
float mIdlePose[SERVO_COUNT_MAX];
#endif

void InitIO(void) {
#ifndef DEBUG_X86
	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);
	// enable periph clock for PIO controller
	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOB);
	// enable periph clock for PIO controller

	// Disable USB pullup (prevents enumeration)
    // Set in PIO mode and Configure in Output
	AT91C_BASE_PIOA->PIO_PER 	= AT91A_USB_PUP; 		// Set in PIO mode
    AT91C_BASE_PIOA->PIO_OER    = AT91A_USB_PUP;		// Configure as output
    AT91C_BASE_PIOA->PIO_CODR   = AT91A_USB_PUP;		// Set to LOW
    AT91C_BASE_PIOA->PIO_PPUDR  = AT91A_USB_PUP;		// Disable pullup
#endif
}

void EnableReset(void) {
#ifndef DEBUG_X86
	// Enable external reset via switch on PCB
	AT91C_BASE_RSTC->RSTC_RCR = 0xA5000008;
	AT91C_BASE_RSTC->RSTC_RMR = 0xA5000001;
#endif
}

int main(void) {
	EnableReset();							// Enable reset switch
	InitIO();								// Initialize I/O which no other module will initialize
	DBGU_Init();
	TC_Init();
	UI_Init();
	ADC_Init();
	UART_Init();
	I2C_Init();
	Parser_Init();
	USB_Init();
	uart0_AssignCharReceivedCallback(Parser_ProcessChar);

	dbgu_print_ascii("WF Wolves Body Firmware\r\nCompiled: ");
	dbgu_print_ascii(__DATE__);
	dbgu_print_char(' ');
	dbgu_print_ascii(__TIME__);
	dbgu_print_ascii("\r\n\r\n");

	dbgu_print_ascii("Hold down button 1 to enter Shell mode\r\n");
#ifdef DYNAMIXEL
	dbgu_print_ascii("Hold down button 2 to enter CM370 mode\r\n\r\n");
#else
	dbgu_print_ascii("Hold down button 2 to enter RoboBuilder (RBC) mode\r\n\r\n");
#endif

	UI_SetLED(0, 1);

	dbgu_print_ascii("Trying to enumerate USB... ");
	if (USBWaitForConfigTimeout(5000)) {
		dbgu_print_ascii("Enumerated!\r\n");
	} else {
		dbgu_print_ascii("Not enumerated\r\n");
	}

	dbgu_print_ascii("Initializing servos...\r\n");
	Config_Init();
	Servo_Init();
	ServoPWM_Init();
	Servo_SetPowerState(1);
	odometry_initOdemetry();

	UI_SetLED(0, 0);
	UI_SetLED(1, 1);

#ifdef DYNAMIXEL
	Servo_SetPosition(SERVO_ID_HEAD_PAN, 0);
	Servo_SetPosition(SERVO_ID_HEAD_TILT, 0);
#else
	Servo_SetPosition(SERVO_ID_HEAD_PAN, 0);
#ifdef TELEPRESENZ
	Servo_SetPosition(SERVO_ID_HEAD_TILT, -45);
#else
	Servo_SetPosition(SERVO_ID_HEAD_TILT, 90);
#endif
#endif
	Scenes_Init();
	Action_Init();
	KinematicScenes_Init();
	PID_Init();
	Walker_Init();
	Walker_SetServoParams();
	Kicker_Init();
	//if (UI_GetKeyState(1) != 1) {
		Walker_ResetArmPositions();
		Walker_SetWalk(0, 0, 0);
		Walker_Start();
		Walker_Process();
		Servo_SendPositions();
	//}
	dbgu_print_ascii("Done!\r\n");
	TC_DelayMs(1500);

#ifdef USE_FADE_TO_IDLE
	// Remember current pose as idle pose
	for (unsigned char i = 0; i < SERVO_COUNT_MAX; ++i) {
		mIdlePose[i] = Servo_GetPosition(i);
	}
#endif

	UI_SetLED(1, 0);
	UI_SetLED(2, 1);

	dbgu_print_ascii("Calibrating IMU... ");
	IMU_Init();
	if (IMU_Calibrate()) {
		for (int i = 0; i < 250; i++) {
			IMU_Update();
			TC_DelayMs(2);
		}
		dbgu_print_ascii("Done!\r\n");
	}
#ifdef DYNAMIXEL
	Servo_SetPosition(SERVO_ID_HEAD_PAN, 0);
	Servo_SendPosition(SERVO_ID_HEAD_PAN, 0);
	TC_DelayMs(1);
	Servo_SetPosition(SERVO_ID_HEAD_TILT, -45);
	Servo_SendPosition(SERVO_ID_HEAD_TILT, -45);
#else
	Servo_SetPosition(SERVO_ID_HEAD_PAN, 0);
	Servo_SendPosition(SERVO_ID_HEAD_PAN, 0);
	TC_DelayMs(1);
#ifdef TELEPRESENZ
	Servo_SetPosition(SERVO_ID_HEAD_TILT, 90);
	Servo_SendPosition(SERVO_ID_HEAD_TILT, 90);
#else
	Servo_SetPosition(SERVO_ID_HEAD_TILT, -90);
	Servo_SendPosition(SERVO_ID_HEAD_TILT, -90);
#endif
#endif
	UI_SetLED(2, 0);

	if (UI_GetKeyState(0) == 1) {
		UI_SetLED(0, 1);
#ifdef DYNAMIXEL
		//dbgu_print_ascii("Board is in Dynamixel forwarding mode!\r\n");
		//Servo_SetForwardingMode(1);
		//while (1 == 1);
		dbgu_print_ascii("Board is in CM730 mode!\r\n");
		CM730_Main();
#else
		dbgu_print_ascii("Board is in RBC mode!\r\n");
		RBC_Main(1);
#endif
	}

	if (UI_GetKeyState(1) == 1) {
		dbgu_print_ascii("Board is in Shell mode!\r\n");
		UI_SetLED(0, 1);
		Shell_Main();
	}

	dbgu_print_ascii("READY.\r\n\r\n");

	main_loop_kinematic();
}

void main_loop_kinematic(void) {
	unsigned char currentState = STATE_MAIN;
	unsigned char lastState = 99;
	signed char lastMotionCmd = -1;
	unsigned short stabilizeCnt = 0;
	unsigned char isParalyzed = 0;
	struct t_motion_select motionSelects;
	char usbBuffer[16];
	unsigned short servoUpdate_loopCnt = 0;

	for (unsigned char i = 0; i < sizeof(motionSelects.motionIDs); ++i) {
		motionSelects.motionIDs[i] = 0xff;
	}
	motionSelects.motionIDs[GETUP_FRONT] = 1;
	motionSelects.motionIDs[GETUP_BACK] = 0;
	motionSelects.motionIDs[KICK_LEFT] = 3;
	motionSelects.motionIDs[KICK_RIGHT] = 2;
	motionSelects.motionIDs[GOALIE_MOVE_LEFT] = 6;
	motionSelects.motionIDs[GOALIE_MOVE_RIGHT] = 6;
	eeprom_read("MOTION SELECTS", &motionSelects, sizeof(motionSelects));

	mWalkerControlAllowed = 1;

	while(1){
		unsigned long loopStartTime = TC_GetSystemTicks();

		UI_ToggleLED(0);
		IMU_Update();
		servoUpdate_loopCnt++;
		if (servoUpdate_loopCnt >= SERVO_STAT_RESET_CYCLES) {
			Servo_ResetMaxError();
			servoUpdate_loopCnt = 0;
		}
		Servo_UpdateStatus(statusMessage.servo_status);

		// Look for USB chars, when available feed to parser
		int read = USBRead_nonblocking(&usbBuffer[0], sizeof(usbBuffer));
		if (read > 0) {
			for (unsigned char i = 0; i < read; ++i) {
				Parser_ProcessChar(usbBuffer[i]);
			}
		}

		// Parse messages in all states, but actually use motion
		// instructions only in some of them
		signed char motionCmd = ParseMessages();

		if (UI_GetKeyState(0) == 1) {
#ifdef USE_UNPARALYZE_DELAY
			if (isParalyzed == 0) {
				isParalyzed = 1;
				UI_SetLED(2, isParalyzed);
				dbgu_printf("[Main] Robot is now paralyzed!\r\n");
			} else {
				dbgu_printf("[Main] Robot is going to be unparalyzed...\r\n");
				stabilizeCnt = UNPARALYZE_WAIT_CYCLES;
				currentState = STATE_WAITUNPARALYZE;
			}
#else
			isParalyzed = 1 - isParalyzed;
			UI_SetLED(2, isParalyzed);
			dbgu_printf("[Main] Robot is now %sparalyzed!\r\n", isParalyzed ? "" : "un");
#endif
			TC_DelayMs(50);
			while (UI_GetKeyState(0) == 1);
			TC_DelayMs(50);
		}

		if (isParalyzed == 1) {
			if (lastMotionCmd != 0) {
				motionCmd = 0;
			} else {
				motionCmd = -1;
			}
			mGoalFBStep = 0;
			mGoalRLStep = 0;
			mGoalRLTurn = 0;
		}

		if (currentState != lastState) {
			dbgu_printf("[Main] New state: %d (%s)\r\n", currentState, GetStateName(currentState));
			lastState = currentState;
		}

		if (currentState == STATE_MAIN) {
			if (IMU_GetFBAccelAverage() < GETUP_FRONT_TRIGGER) {
				dbgu_printf("[Main] Detected fall, getting up from front\r\n");
				DisableWalkerControl();
				PLAY_SCENE(motionSelects.motionIDs[GETUP_FRONT]);
#ifdef DYNAMIXEL
				Servo_SetPosition(SERVO_ID_HEAD_PAN, 0);
				Servo_SetPosition(SERVO_ID_HEAD_TILT, -90);
#else
				Servo_SetPosition(SERVO_ID_HEAD_PAN, 0);  // Set in current positions array (used for scenes)
				Servo_SetPosition(SERVO_ID_HEAD_TILT, 90);
#endif
				mBlockPanTilt = 1;
				currentState = STATE_GETUPFRONT;
			} else if (IMU_GetFBAccelAverage() > GETUP_BACK_TRIGGER) {
				dbgu_printf("[Main] Detected fall, getting up from back\r\n");
				DisableWalkerControl();
				PLAY_SCENE(motionSelects.motionIDs[GETUP_BACK]);
#ifdef DYNAMIXEL
				Servo_SetPosition(SERVO_ID_HEAD_PAN, 0);
				Servo_SetPosition(SERVO_ID_HEAD_TILT, -90);
#else
				Servo_SetPosition(SERVO_ID_HEAD_PAN, 0);  // Set in current positions array (used for scenes)
				Servo_SetPosition(SERVO_ID_HEAD_TILT, 90);
#endif
				mBlockPanTilt = 1;
				currentState = STATE_GETUPBACK;
			} else {
				if (motionCmd != lastMotionCmd || motionCmd == 0) {
					if (motionCmd > -1) {
						switch (motionCmd) {
						case 0:
							if (mGoalFBStep != 0 || mGoalRLStep != 0 || mGoalRLTurn != 0) {
								currentState = STATE_RETURNTOSTAY;
							}
							mGoalFBStep = 0;
							mGoalRLStep = 0;
							mGoalRLTurn = 0;
							break;
						case GO_FORWARD_CMD: // Go forward
							mGoalFBStep = WALK_FORWARD_MAX_SPEED;
							mGoalRLStep = 0;
							mGoalRLTurn = 0;
							break;
						case 2: // Turn left
							mGoalFBStep = 0;
							mGoalRLStep = 0;
							mGoalRLTurn = TURN_MAX_SPEED;
							break;
						case 3: // Turn right
							mGoalFBStep = 0;
							mGoalRLStep = 0;
							mGoalRLTurn = -TURN_MAX_SPEED;
							break;
						case 4: // Go right
							mGoalFBStep = 0;
							mGoalRLStep = -WALK_SIDEWAY_MAX_SPEED;
							mGoalRLTurn = 0;
							break;
						case 5: // Go left
							mGoalFBStep = 0;
							mGoalRLStep = WALK_SIDEWAY_MAX_SPEED;
							mGoalRLTurn = 0;
							break;
						case 6: //Kick Left
							DisableWalkerControl();
						  #ifdef USE_KICKENGINE
							mBall.x = 100;
							mBall.y = -70;
							mBall.z = 40;
							Kicker_KickForward( &mBall, KICKER_TYPE_MAXIMAL);
							currentState = STATE_KICKER;
						  #else
							PLAY_SCENE(motionSelects.motionIDs[KICK_LEFT]);
							currentState = STATE_PLAYSTATIC;
						  #endif
							break;
						case 7: //Kick Right
							DisableWalkerControl();
						  #ifdef USE_KICKENGINE
							mBall.x = 100;
							mBall.y = 70;
							mBall.z = 40;
							Kicker_KickForward( &mBall, KICKER_TYPE_MAXIMAL);
							currentState = STATE_KICKER;
						  #else
							PLAY_SCENE(motionSelects.motionIDs[KICK_RIGHT]);
							currentState = STATE_PLAYSTATIC;
						  #endif
							break;
						case 10://OH NO
							DisableWalkerControl();
							PLAY_SCENE(30);
							currentState = STATE_PLAYSTATIC;
							break;
						case 11:
							DisableWalkerControl();
							PLAY_SCENE(motionSelects.motionIDs[GOALIE_MOVE_LEFT]);
							currentState = STATE_PLAYSTATIC;
							break;
						case 12:
							DisableWalkerControl();
							PLAY_SCENE(motionSelects.motionIDs[GOALIE_MOVE_RIGHT]);
							currentState = STATE_PLAYSTATIC;
							break;
						case 14: // Hi pose
							DisableWalkerControl();
							PLAY_SCENE(15);
							currentState = STATE_PLAYSTATIC;
							break;
						case 15: // YeahGoal pose
							DisableWalkerControl();
							PLAY_SCENE(16);
							currentState = STATE_PLAYSTATIC;
							break;
						case 50: // KickToTarget
							DisableWalkerControl();
							Kicker_KickToTarget( &mBall, &mTarget);
							currentState = STATE_KICKER;
							break;
						case 100: // Walker cmd
							break;
						}
						lastMotionCmd = motionCmd;
					}
				}
			}
		} else if (currentState == STATE_PLAYSTATIC) {
			if (!SCENE_PLAYING()) {
				currentState = STATE_RETURNTOSTAY;
			}
		} else if (currentState == STATE_PLAYKINEMATIC) {
			if (!KinematicScenes_IsPlaying()) {
				currentState = STATE_RETURNTOSTAY;
			}
		} else if (currentState == STATE_KICKER) {
			if (!Kicker_IsRunning()) {
				currentState = STATE_RETURNTOSTAY;
			}
		} else if (currentState == STATE_RETURNTOSTAY) {
#ifdef USE_FADE_TO_IDLE
			unsigned char done_pos = 1;
			for (unsigned char i = 0; i < SERVO_COUNT_MAX; ++i) {
				if (i != SERVO_ID_HEAD_PAN && i != SERVO_ID_HEAD_TILT) { // Do not change Pan/Tilt
					float curPos = Servo_GetPosition(i);
					if (curPos != mIdlePose[i]) {
						float delta = mIdlePose[i] - curPos;
						if ((((delta) < 0) ? -(delta) : (delta)) > FADE_TO_IDLE_INCEMENT) {
							if (delta > 0) {
								Servo_SetPosition(i, curPos + FADE_TO_IDLE_INCEMENT);
							} else {
								Servo_SetPosition(i, curPos - FADE_TO_IDLE_INCEMENT);
							}
						} else {
							Servo_SetPosition(i, mIdlePose[i]);
						}
						done_pos = 0;
					}
				}
			}

			unsigned char done_pid = 1;
			float pidTarget[3];
			float pidCurrent[3];
			Walker_GetPIDGains(&pidTarget[0], &pidTarget[1], &pidTarget[2]);
			Servo_GetTGainGlobal(&pidCurrent[0], &pidCurrent[1], &pidCurrent[2]);
			for (unsigned char i = 0; i < 3; ++i) {
				if (pidCurrent[i] != pidTarget[i]) {
					float delta = pidTarget[i] - pidCurrent[i];
					if ((((delta) < 0) ? -(delta) : (delta)) > FADE_PID_INCEMENT) {
						if (delta > 0) {
							pidCurrent[i] += FADE_PID_INCEMENT;
						} else {
							pidCurrent[i]-= FADE_PID_INCEMENT;
						}
					} else {
						pidCurrent[i] = pidTarget[i];
					}
					done_pid = 0;
				}
			}
			if (done_pid == 0) {
				Servo_SendTGainGlobal(pidCurrent[0], pidCurrent[1], pidCurrent[2]);
			}
			if (done_pos == 1 && done_pid == 1) {
				dbgu_printf("[Main] All servos on target pos, re-initializing walker\r\n");
#endif
				Walker_SetServosActive(1);
				Walker_SetServoParams();
				Walker_ResetArmPositions();

				currentState = STATE_STABILIZE;
				stabilizeCnt = STABILIZE_CYCLES;
#ifdef USE_FADE_TO_IDLE
			} else {
				currentState = STATE_RETURNTOSTAY;
			}
#endif
		} else if (currentState == STATE_STABILIZE) {
			if (stabilizeCnt == 0) {
				lastMotionCmd = 0;
				mWalkerControlAllowed = 1;
				currentState = STATE_MAIN;
			} else {
				stabilizeCnt--;
			}
		} else if (currentState == STATE_GETUPBACK) {
			if (!SCENE_PLAYING()) {
				mBlockPanTilt = 0;
				currentState = STATE_RETURNTOSTAY;
			}
		} else if (currentState == STATE_GETUPFRONT) {
			if (!SCENE_PLAYING()) {
				mBlockPanTilt = 0;
				currentState = STATE_RETURNTOSTAY;
			}
		} else if (currentState == STATE_WAITUNPARALYZE) {
			if (stabilizeCnt == 0) {
				isParalyzed = 0;
				UI_SetLED(2, isParalyzed);
				dbgu_printf("[Main] Robot is now unparalyzed!\r\n");
				lastMotionCmd = 0;
				currentState = STATE_MAIN;
			} else {
				stabilizeCnt--;
			}
		}

		if(mGoalFBStep == 0 && mGoalRLStep == 0 && mGoalRLTurn == 0 && Walker_X_MOVE_AMPLITUDE == 0 && Walker_Y_MOVE_AMPLITUDE == 0 && Walker_A_MOVE_AMPLITUDE == 0) {
			if (Walker_IsRunning()) {
				dbgu_printf("[Main] Walker stop\r\n");
				Walker_Stop();
			}

		} else {
			if(!Walker_IsRunning()) {
				dbgu_printf("[Main] Walker start\r\n");
				Walker_X_MOVE_AMPLITUDE = 0;
				Walker_Y_MOVE_AMPLITUDE = 0;
				Walker_A_MOVE_AMPLITUDE = 0;
				Walker_Start();
			} else {
				float delta = mGoalFBStep - Walker_X_MOVE_AMPLITUDE;
				if ((((delta) < 0) ? -(delta) : (delta)) > UNIT_FB_STEP) {
					if (delta > 0) {
						Walker_X_MOVE_AMPLITUDE += UNIT_FB_STEP;
					} else {
						Walker_X_MOVE_AMPLITUDE -= UNIT_FB_STEP;
					}
				} else {
					Walker_X_MOVE_AMPLITUDE = mGoalFBStep;
				}

				delta = mGoalRLStep - Walker_Y_MOVE_AMPLITUDE;
				if ((((delta) < 0) ? -(delta) : (delta)) > UNIT_RL_STEP) {
					if (delta > 0) {
						Walker_Y_MOVE_AMPLITUDE += UNIT_RL_STEP;
					} else {
						Walker_Y_MOVE_AMPLITUDE -= UNIT_RL_STEP;
					}
				} else {
					Walker_Y_MOVE_AMPLITUDE = mGoalRLStep;
				}

				delta = mGoalRLTurn - Walker_A_MOVE_AMPLITUDE;
				if ((((delta) < 0) ? -(delta) : (delta)) > UNIT_RL_TURN) {
					if (delta > 0) {
						Walker_A_MOVE_AMPLITUDE += UNIT_RL_TURN;
					} else {
						Walker_A_MOVE_AMPLITUDE -= UNIT_RL_TURN;
					}
				} else {
					Walker_A_MOVE_AMPLITUDE = mGoalRLTurn;
				}

				//dbgu_printf("[Main] FB: %.1f, RL S: %.1f, ,RL T: %.1f\r\n", Walker_X_MOVE_AMPLITUDE, Walker_Y_MOVE_AMPLITUDE, Walker_A_MOVE_AMPLITUDE);
			}
		}

		Walker_Process();

		Servo_SendPositions();

		// Waste remaining loop time
		if (TC_GetMsSinceTick(loopStartTime) > MAIN_LOOP_DURATION) {
			dbgu_printf("[Main] WARNING: Main loop has taken too long: %d ms instead of %d ms!\r\n", TC_GetMsSinceTick(loopStartTime), MAIN_LOOP_DURATION);
		}
		while (TC_GetMsSinceTick(loopStartTime) < MAIN_LOOP_DURATION);
	}
}

void UpdateStatusMessage(void) {
#define AVERAGING_SIZE	16

	static int samples[AVERAGING_SIZE];
	static unsigned char readPos = 0;

	// Determine voltage from raw ADC value
	// Each ADC unit: 3300 / 1024 = 3.222656 mV
	// Voltage divider factor : 270k / (1M + 270k) = 0.2126
	// ADC units per battery V: 0.003222656 * (1 / 0.2126) = 0.01515831
	int voltage = (((float)ADC_GetChannel(6) * 0.01515831f) * 100.0);
#ifdef DYNAMIXEL
	voltage += 30; // Shottky diode on PDDI board
#else
	voltage += (3 * 35); // Shottky diodes in input path
#endif
	voltage -= 8; // Measured correction factor

	samples[readPos++] = voltage;
	if(readPos == AVERAGING_SIZE) readPos = 0;

	int vol_avg = 0;
	for (unsigned char i = 0; i < AVERAGING_SIZE; ++i) {
		vol_avg += samples[i];
	}
	vol_avg /= AVERAGING_SIZE;

	statusMessage.voltage = vol_avg;
	IMU_UpdateStatus(&statusMessage.imu);

	struct position* odo = odometry_getPosition();
	statusMessage.odometry.xPos = (signed short)odo->xPos;
	statusMessage.odometry.yPos = (signed short)odo->yPos;
	statusMessage.odometry.viewDirection = (signed short)odo->viewDirection;
	statusMessage.odometry.weight = odo->weight;
}

signed char ParseMessages(void) {
#ifdef SEND_STATUSDATA
	static unsigned char statusBuffer[8 + sizeof(struct statusMessage_t)];
#endif
	signed char ret = -1;

	if (UI_GetKeyState(1) == 1) {
		return GO_FORWARD_CMD;
	}

	if (Parser_IsMessageWaiting() == 1) {
		UI_ToggleLED(1);
		struct parser_message* msg = Parser_GetMessage();
		if (msg->type == 1) { // Message is an instruction
			unsigned char instruction = msg->instruction;
			if (instruction == 0x10) { // Play motion instruction
				ret = msg->data[1];
				dbgu_printf("[Main] Motion inst: %d\r\n", ret);
			} else if (instruction == 0x11) { // Full stop instruction
				ret = 0;
			} else if (instruction == 0x12) { // Set pan/tilt instruction
				signed char XGrad = msg->data[0];
				signed char YGrad = msg->data[1];
				float X = (float)XGrad;
				float Y = (float)YGrad + 90;
				if (Y < 0) Y = 0;
				if (Y > 90) Y = 90;

				if (X < -90) X = -90;
				if (X > 90) X = 90;
				if (!mBlockPanTilt) {
#ifdef DYNAMIXEL
					Servo_SetPosition(SERVO_ID_HEAD_PAN, X);
					Servo_SetPosition(SERVO_ID_HEAD_TILT, (-45-(Y-90)));
#else
					Servo_SetPosition(SERVO_ID_HEAD_PAN, X);  // Set in current positions array (used for scenes)
					Servo_SetPosition(SERVO_ID_HEAD_TILT, Y);
#endif
				}
				dbgu_printf("[Main] P/T: %d/%d\r\n", XGrad, YGrad);
			} else if (instruction == 0x14) { // KickToTarget (set ball and target and kick)
#define SHORT_FROM_CHARS(x, y) ((x << 8) | y)
				mBall.x = SHORT_FROM_CHARS( msg->data[0], msg->data[1]);
				mBall.y = SHORT_FROM_CHARS( msg->data[2], msg->data[3]);
				mBall.z = SHORT_FROM_CHARS( msg->data[4], msg->data[5]);
				mTarget.x = SHORT_FROM_CHARS( msg->data[6], msg->data[7]);
				mTarget.y = SHORT_FROM_CHARS( msg->data[8], msg->data[9]);
				mTarget.z = SHORT_FROM_CHARS( msg->data[10], msg->data[11]);
				ret = 50; //ret = msg->data[12];
				dbgu_printf("[Main] KickToTarget: %d, %d, %d - %d, %d, %d\r\n", mBall.x, mBall.y, mBall.z, mTarget.x, mTarget.y, mTarget.z);
			} else if (instruction == 0x15) { // Set motion vector
				signed char XSpeed = msg->data[0];
				signed char YSpeed = msg->data[1];
				signed char ThetaSpeed = msg->data[2];
				unsigned char turningAim = msg->data[3];

				if (XSpeed < -100) { XSpeed = -100; }
				if (XSpeed > +100) { XSpeed = +100; }
				if (YSpeed < -100) { YSpeed = -100; }
				if (YSpeed > +100) { YSpeed = +100; }
				if (ThetaSpeed < -100) { ThetaSpeed = -100; }
				if (ThetaSpeed > +100) { ThetaSpeed = +100; }
				if (mWalkerControlAllowed == 1) {
					mGoalFBStep = (WALK_FORWARD_MAX_SPEED / 100.0) * XSpeed;
					mGoalRLStep = (WALK_SIDEWAY_MAX_SPEED / 100.0) * YSpeed;
					mGoalRLTurn = (TURN_MAX_SPEED / 100.0) * ThetaSpeed;

					Walker_A_MOVE_AIM_ON = turningAim;

					ret = 100;
				} else {
					ret = 0;
				}
			} else if (instruction == INSTRUCTION_BOOTLOADER) { // bootloader
				if (msg->length == 4) {
					if (msg->data[0] == 0xDE && msg->data[1] == 0xAD && msg->data[2] == 0xBE && msg->data[3] == 0xEF) {
						dbgu_printf("[Main] Entering bootloader...\r\n");
						Bootloader_start();
					}
				}
				dbgu_printf("[Main] Received invalid bootloader start packet!\r\n");
			}

			UpdateStatusMessage();
#ifdef SEND_STATUSDATA
			size_t len = Parser_CreatePacket(CONTROLLER_MAIN, INSTRUCTION_STATUSMESSAGE, (unsigned char*)&statusMessage, sizeof(statusMessage), &statusBuffer[0], sizeof(statusBuffer));
			USB_send((const char*)&statusBuffer[0], len);
#endif

			//Servo_ResetMaxError();
		} else if (msg->type == 2) { // Message is a request
			unsigned char instruction = msg->instruction;

			if (instruction == 0x1B) {
				char item = msg->data[0];
				if (item == 1) {
					// Determine voltage from raw ADC value
					// Each ADC unit: 3300 / 1024 = 3.222656 mV
					// Voltage divider factor : 270k / (1M + 270k) = 0.2126
					// ADC units per battery V: 0.003222656 * (1 / 0.2126) = 0.01515831

					int voltage = (((float)ADC_GetChannel(6) * 0.01515831f) * 100.0);
#ifdef DYNAMIXEL
					voltage += 30; // Shottky diode on PDDI board
#else
					voltage += (3 * 35); // Shottky diodes in input path
#endif

					dbgu_printf("[Main] Voltage request received, V=%d cV\r\n", voltage);
					uart0_putc((voltage & 0xFF00) >> 8);
					uart0_putc((voltage & 0xFF));
				} else if (item == 2) {
					//int angle;
/*
					int angle = IMU_GetPitch();
					uart0_putc((angle & 0xFF00) >> 8);
					uart0_putc((angle & 0xFF));
					angle = IMU_GetRoll();
					uart0_putc((angle & 0xFF00) >> 8);
					uart0_putc((angle & 0xFF));
*/
				}
			}


		}
		Parser_SetMessageProcessed();
	}
	return ret;
}

void DisableWalkerControl(void) {
	dbgu_printf("[Main] Walker stop\r\n");
	Walker_Stop();
    // Uncomment following to stop robot after static scenes
	mGoalFBStep = 0;
	mGoalRLStep = 0;
	mGoalRLTurn = 0;
    while(Walker_IsRunning() == 1) {
		unsigned long loopStartTime = TC_GetSystemTicks();
		Walker_Process();
		Servo_SendPositions();
		while (TC_GetMsSinceTick(loopStartTime) < MAIN_LOOP_DURATION);
    }
    Walker_SetServosActive(0);
    mWalkerControlAllowed = 0;
}

char* GetStateName(unsigned char state) {
#define STATETOCASE(n)	case n: return #n
	switch (state) {
		STATETOCASE(STATE_MAIN);
		STATETOCASE(STATE_PLAYSTATIC);
		STATETOCASE(STATE_RETURNTOSTAY);
		STATETOCASE(STATE_GETUPFRONT);
		STATETOCASE(STATE_GETUPBACK);
		STATETOCASE(STATE_WALKBEGIN);
		STATETOCASE(STATE_WALK);
		STATETOCASE(STATE_WALKEND);
		STATETOCASE(STATE_PLAYKINEMATIC);
		STATETOCASE(STATE_STABILIZE);
		STATETOCASE(STATE_KICKER);
		STATETOCASE(STATE_WAITUNPARALYZE);
		default:
			return "<unknown>";
	}
}
