/*
 * shell_commands.c
 *
 *  Created on: 18.04.2011
 *      Author: Stefan
 */

#include "shell_commands.h"
#include "dbgu/dbgu.h"
#include "tests/imu.h"
#include "tests/kickertest.h"
#include "tests/kickertuner.h"
#include "tests/kinematictest.h"
#include "tests/motioneditor.h"
#include "tests/walktuner.h"
#include "servo/servo.h"
#include "config/config.h"
#include "ui/ui.h"
#include "tc/tc.h"
#include "movements/scenes.h"
#include "kinematics/inverse.h"
#include "movements/kinematic_scenes.h"
#include "rbc/rbc.h"
#include "adc/adc.h"
#include "odometry/odometry.h"
#include "imu/imu.h"
#include "controller/controller.h"
#include "eeprom/eeprom.h"
#define USE_NAME_RES
#include "movements/motionselect.h"
#undef USE_NAME_RES
#include "bootloader/bootloader.h"
#include "shell/xmodem/xmodem.h"
#include "movements/action.h"
#include "tests/offsettuner.h"
#include "include/Board.h"
#include <stdlib.h>

#define LOOP_UPDATE_INTERVAL		25 // ms

#ifdef DYNAMIXEL
#define PLAY_SCENE		Action_Start
#define SCENE_PLAYING	Action_IsRunning
#else
#define PLAY_SCENE		Scenes_StartPlay
#define SCENE_PLAYING	Scenes_IsPlaying
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

// imutest command implementation
int shell_cmd_imutest(char * args[], char num_args) {
	IMUTest_Main();
	return 0;
}

// setservo command implementation
int shell_cmd_setservo(char * args[], char num_args) {
	if (num_args >= 2) {
		unsigned char id = atoi(args[0]);
		float pos = atof(args[1]);
		Servo_SetPosition(id, pos);
		Servo_SendPosition(id, pos);
		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_setoffsetenable(char * args[], char num_args) {
	if (num_args >= 1) {
		unsigned char state = atoi(args[0]);
		if (state == 0) {
			Servo_SetOffsetEnable(0);
		} else {
			Servo_SetOffsetEnable(1);
		}
		return 0;
	} else {
		return -1;
	}
}

// getservo command implementation
int shell_cmd_getservo(char * args[], char num_args) {
	if (num_args >= 1) {
		unsigned char id = atoi(args[0]);
		float pos = Servo_ReadPos(id);
		dbgu_printf("%f\r\n", pos);
		return 0;
	} else {
		return -1;
	}
}

void print_bot(unsigned char* values) {
	dbgu_printf(	\
		"                ======\r\n"
		"                | %2u |\r\n"
		"                ======\r\n"
		"         =======| %2u |========\r\n"
		"         |      ======       |\r\n"
		"      ======               ======\r\n"
		"======| %2u |   F r o n t   | %2u |======\r\n"
		"| %2u |======               ======| %2u |\r\n"
		"======   |                   |   ======\r\n"
		"| %2u |   |  ======   ======  |   | %2u |\r\n"
		"======   ---| %2u |---| %2u |---   ======\r\n"
		"          ========   ========\r\n"
		"          |   %2u |   | %2u   |\r\n"
		"          ========   ========\r\n"
		"            | %2u |   | %2u |\r\n"
		"            ======   ======\r\n"
		"            | %2u |   | %2u |\r\n"
		"            ======   ======\r\n"
		"            | %2u |   | %2u |\r\n"
		"          ========   ========\r\n"
		"          |   %2u |   | %2u   |\r\n"
		"          ========   ========\r\n" \
			, values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8], values[9], values[10], values[11], values[12], values[13] \
			, values[14], values[15], values[16], values[17], values[18], values[19]);
}

// getservo command implementation
int shell_cmd_servohelp(char * args[], char num_args) {
	unsigned char values[] = 	{ SERVO_ID_HEAD_TILT, SERVO_ID_HEAD_PAN, SERVO_ID_R_SHOULDER_PITCH, SERVO_ID_L_SHOULDER_PITCH, SERVO_ID_R_SHOULDER_ROLL, SERVO_ID_L_SHOULDER_ROLL,
								  SERVO_ID_R_ELBOW, SERVO_ID_L_ELBOW, SERVO_ID_R_HIP_YAW, SERVO_ID_L_HIP_YAW, SERVO_ID_R_HIP_ROLL, SERVO_ID_L_HIP_ROLL,
								  SERVO_ID_R_HIP_PITCH, SERVO_ID_L_HIP_PITCH, SERVO_ID_R_KNEE, SERVO_ID_L_KNEE, SERVO_ID_R_ANKLE_PITCH, SERVO_ID_L_ANKLE_PITCH,
								  SERVO_ID_R_ANKLE_ROLL, SERVO_ID_L_ANKLE_ROLL};

	print_bot(&values[0]);
	return 0;
}

int shell_cmd_servotemps(char * args[], char num_args) {
	unsigned char values[] = 	{ SERVO_ID_HEAD_TILT, SERVO_ID_HEAD_PAN, SERVO_ID_R_SHOULDER_PITCH, SERVO_ID_L_SHOULDER_PITCH, SERVO_ID_R_SHOULDER_ROLL, SERVO_ID_L_SHOULDER_ROLL,
								  SERVO_ID_R_ELBOW, SERVO_ID_L_ELBOW, SERVO_ID_R_HIP_YAW, SERVO_ID_L_HIP_YAW, SERVO_ID_R_HIP_ROLL, SERVO_ID_L_HIP_ROLL,
								  SERVO_ID_R_HIP_PITCH, SERVO_ID_L_HIP_PITCH, SERVO_ID_R_KNEE, SERVO_ID_L_KNEE, SERVO_ID_R_ANKLE_PITCH, SERVO_ID_L_ANKLE_PITCH,
								  SERVO_ID_R_ANKLE_ROLL, SERVO_ID_L_ANKLE_ROLL};

	for (unsigned char i = 0; i < sizeof(values); ++i) {
		values[i] = Servo_ReadTemperature(values[i]);
	}
	print_bot(&values[0]);

	return 0;
}

int shell_cmd_getzeropos(char * args[], char num_args) {
	unsigned char servocnt = Config_GetServoCount();
	float* servo_pos = (float*)malloc(servocnt * sizeof(float));

	// Set all servos to passive mode
	dbgu_printf("Setting servos to passive mode...\r\n");
	int i;
	for (i = 0; i < servocnt; i++) {
		Servo_SetPassive(i, 1);
	}

	while (1) {
		unsigned long loopStartTime = TC_GetSystemTicks();
		UI_ToggleLED(1);

		// Read all positions
		dbgu_printf("Reading data... ");
		for (i = 0; i < servocnt; i++) {
			dbgu_printf("%d ", i);
			float pos = Servo_ReadPos(i);
			servo_pos[i] = pos;
		}
		dbgu_printf("\r\n");

		// Look for non-responding servos
		for (i = 0; i < servocnt; i++) {
			if (servo_pos[i] == -1000) {
				dbgu_printf("Servo %d did not respond!\r\n", i);
			}
		}

		// Print positions
		for (i = 0; i < servocnt; i++) {
			dbgu_printf(" %2d: %3f;", i, servo_pos[i]);
			if ((i + 1) % 10 == 0) {
				dbgu_printf("\r\n");
			}
		}
		dbgu_printf("\r\n");

		if (dbgu_char_available()) {
			if (dbgu_get_char() == 3) { // Ctrl + C
				break;
			}
		}

		// Waste remaining loop time
		while (TC_GetMsSinceTick(loopStartTime) < LOOP_UPDATE_INTERVAL);
	}
	//Servo_SendPositions(); // Warning: Could be dangerous when robot is in scaffolding
	free(servo_pos);
	return 0;
}

int shell_cmd_playscene(char * args[], char num_args) {
	if (num_args >= 1) {
		unsigned char index = atoi(args[0]);

		PLAY_SCENE(index);
		while (SCENE_PLAYING());

		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_setservoid(char * args[], char num_args) {
	if (num_args >= 2) {
		unsigned char oldid = atoi(args[0]);
		unsigned char newid = atoi(args[1]);

		if (oldid <= 30 && newid <= 30) {
			if (Servo_IsResponding(oldid) == 1) {
				unsigned char okToSet = 1;
				if (Servo_IsResponding(newid) == 1) {
					okToSet = 0;
					dbgu_printf("Servo ID %d seems to be in use! Change ID anyway? (y/n)", newid);
					char ans = dbgu_get_char();
					dbgu_printf("%c\r\n", ans);
					if (ans == 'y') {
						okToSet = 1;
					}
				}
				if (okToSet) {
					Servo_SetID(oldid, newid);
					TC_DelayMs(300); // Give servo time to change it's ID
					if (Servo_IsResponding(newid) == 1) {
						dbgu_printf("Changed servo ID %d to %d.", oldid, newid);
					} else {
						dbgu_printf("ERROR: New ID does not respond!");
						return -4;
					}
				}
			} else {
				dbgu_printf("Servo %d did not respond!\r\n", oldid);
				return -3;
			}
		} else {
			dbgu_printf("Either old or new servo ID is out of range (> 30)\r\n");
			return -2;
		}
		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_listservos(char * args[], char num_args) {
	unsigned char servocnt = 31;
	int cnt = 0;

	dbgu_printf("Responding servos: ");
	for (int i = 0; i < servocnt; i++) {
		if (Servo_IsResponding(i) == 1) {
			dbgu_printf("%d ", i);
			cnt++;
		}
	}
	dbgu_printf("\r\n%d servos found.\r\n", cnt);

	return 0;
}

int shell_cmd_servoinfo(char * args[], char num_args) {
	if (num_args >= 1) {
		unsigned char id = atoi(args[0]);
		float pos = Servo_ReadPos(id);
		unsigned char load = Servo_ReadLoad(id);
		unsigned char overload = Servo_ReadOverload(id);
		unsigned short bound_l, bound_h;
		Servo_ReadBounds(id, &bound_l, &bound_h);
		unsigned char p, i, d;
		Servo_ReadPID(id, &p, &i, &d);

		int over_ma = 0;
		switch (overload) {
			case 33: over_ma = 400; break;
			case 44: over_ma = 500; break;
			case 56: over_ma = 600; break;
			case 68: over_ma = 700; break;
			case 80: over_ma = 800; break;
			case 92: over_ma = 900; break;
			case 104: over_ma = 1000; break;
			case 116: over_ma = 1100; break;
			case 128: over_ma = 1200; break;
			case 199: over_ma = 1800; break;
		}
		double voltage = (double)Servo_ReadVoltage(id) / 10.0;
		unsigned char temp = Servo_ReadTemperature(id);

		dbgu_printf("Position:           %f\r\n", pos);
		dbgu_printf("Load:               %d\r\n", load);
		dbgu_printf("Overload threshold: %d (%d mA)\r\n", overload, over_ma);
		dbgu_printf("Boundary:           %d - %d\r\n", bound_l, bound_h);
		dbgu_printf("P-, I-, D-Gain:     %d, %d, %d\r\n", p, i, d);
		dbgu_printf("Voltage:            %.1f\r\n", voltage);
		dbgu_printf("Temperature:        %d\r\n", temp);
		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_kinematictest(char * args[], char num_args) {
	KinematicTest_Main();
	return 0;
}

int shell_cmd_kickertest(char * args[], char num_args) {
	Kickertest_Main();
	return 0;
}

int shell_cmd_kickertuner(char * args[], char num_args) {
	Kickertuner_Main();
	return 0;
}

int shell_cmd_playkinscene(char * args[], char num_args) {
	if (num_args >= 1) {
		unsigned char index = atoi(args[0]);
		unsigned char cnt = 1;
		if (num_args >= 2) {
			cnt = atoi(args[1]);
		}

		KinematicScenes_SetTimingFactor(1.0);
		if (cnt > 1) {
			KinematicScenes_StartPlay(index, cnt, 0);
		} else {
			KinematicScenes_StartPlay(index, 1, 0);
			while (KinematicScenes_IsPlaying()) {
				if (dbgu_char_available()) {
					if (dbgu_get_char() == 3) { // Ctrl + C
						KinematicScenes_CancelPlay();
						return 0;
					}
				}
			}
		}

		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_playkinscenewithcontroller(char * args[], char num_args) {
	#define MAIN_LOOP_DURATION		40 // ms

	if (num_args >= 1) {
		unsigned char index = atoi(args[0]);
		unsigned char cnt = 1;
		if (num_args >= 2) {
			cnt = atoi(args[1]);
		}

		PID_Init();

		KinematicScenes_SetTimingFactor(1.0);
		KinematicScenes_StartPlay(index, cnt, 1);
		while (KinematicScenes_IsPlaying()) {
			unsigned long loopStartTime = TC_GetSystemTicks();

			UI_ToggleLED(1);
			IMU_Update();
			float imu = 0; //IMU_GetPitch();
			float hip_adjust = PID_UpdateX(imu);
			float newAngle = inverse_getHipAngle() + hip_adjust;
			if (newAngle > 15) newAngle = 15;
			if (newAngle < -15) newAngle = -15;
			inverse_setHipAngle(newAngle);
			dbgu_printf("imu: %3.5f hip_adjust: %3.5f, newAngle: %3.5f\r\n", imu, hip_adjust, newAngle);

			if (dbgu_char_available()) {
				if (dbgu_get_char() == 3) { // Ctrl + C
					KinematicScenes_CancelPlay();
					return 0;
				}
			}

			/* Scene 4 is a dummy scene which does not move
			 * the robot. Therefore we have to update the kinematics
			 * manually.
			 */
			if (index == 4) {
				inverse_startCycle();
				struct vect3d_ext cur;
				cur = inverse_getCurrentPosExt(LEG_LEFT);
				inverse_calc(LEG_LEFT, &cur);
				cur = inverse_getCurrentPosExt(LEG_RIGHT);
				inverse_calc(LEG_RIGHT, &cur);
				inverse_endCycle(1);
			}

			Servo_SendPositions();

			// Waste remaining loop time
			if (TC_GetMsSinceTick(loopStartTime) >= MAIN_LOOP_DURATION) {
				dbgu_printf("[Main] WARNING: Main loop has taken too long: %d ms instead of %d ms!\r\n", TC_GetMsSinceTick(loopStartTime), MAIN_LOOP_DURATION);
			}
			while (TC_GetMsSinceTick(loopStartTime) < MAIN_LOOP_DURATION);
		}

		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_getoffset(char * args[], char num_args) {
	if (num_args >= 1) {
		unsigned char id = atoi(args[0]);
		signed char offset = Servo_GetOffset(id);
		dbgu_printf("%d\r\n", offset);
		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_setoffset(char * args[], char num_args) {
	if (num_args >= 2) {
		unsigned char id = atoi(args[0]);
		signed char offset = atoi(args[1]);
		Servo_SetOffset(id, offset);
		Servo_SendPosition(id, Servo_GetPosition(id));
		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_listoffsets(char * args[], char num_args) {
	Servo_ListOffsets();
	return 0;
}

int shell_cmd_rbcmode(char * args[], char num_args) {
	dbgu_print_ascii("Board is in RBC mode!\r\n");
	UI_SetLED(0, 1);
	RBC_Main(1);
	return 0;
}

int shell_cmd_getvoltage(char * args[], char num_args) {
	// Determine voltage from raw ADC value
	// Each ADC unit: 3300 / 1024 = 3.222656 mV
	// Voltage divider factor : 270k / (1M + 270k) = 0.2126
	// ADC units per battery V: 0.003222656 * (1 / 0.2126) = 0.01515831

	int voltage = (((float)ADC_GetChannel(6) * 0.01515831f) * 1000.0);
	voltage += (3 * 350); // Shottky diodes in input path

	dbgu_printf("Voltage: %d mV\r\n", voltage);
	dbgu_printf("Approx. cell voltage: %f mV\r\n", ((float)voltage / 3.0));
	return 0;
}

int shell_cmd_walktuner(char * args[], char num_args) {
	WalkTuner_Main();
	return 0;
}

int shell_cmd_listpositions(char * args[], char num_args) {
	unsigned char servocnt = Config_GetServoCount();

	for (int i = 0; i < servocnt; i++) {
		dbgu_printf(" %2d: %3f;", i, Servo_GetPosition(i));
		if ((i + 1) % 10 == 0) {
			dbgu_printf("\r\n");
		}
	}
	dbgu_printf("\r\n");

	return 0;
}

int shell_cmd_servotest(char * args[], char num_args) {
#define SERVO_TEST_POS_TOLERANCE	1
#define SERVO_TEST_MOVE_DURATION	1500
#define SERVO_TEST_LOAD_THRES		135

	if (num_args >= 1) {
		unsigned char id = atoi(args[0]);
		unsigned char failed = 0;

		Servo_SetOffsetEnable(0);

		// 1st test: Read position to check communications
		dbgu_printf("Test 1 (Communication): ");
		if (Servo_IsResponding(id) != 1) {
			Servo_SendPosition(id, 0);
			TC_DelayMs(10);
			if (Servo_IsResponding(id) != 1) {
				dbgu_printf("FAILED - Position could not be read\r\n");
				return -2;
			} else {
				dbgu_printf("SUCCESSFUL\r\n");
			}
		} else {
			dbgu_printf("SUCCESSFUL\r\n");
		}

		// Read current settings
		dbgu_printf("Test 2 (Settings):      ");
		unsigned short bound_l, bound_h;
		Servo_ReadBounds(id, &bound_l, &bound_h);
		unsigned char p, i, d;
		Servo_ReadPID(id, &p, &i, &d);

		// 2nd test: Check servo settings
		if (bound_l > 0 || bound_h < 254) {
			dbgu_printf("FAILED - Boundaries incorrect (%d > 0 || %d < 254)\r\n", bound_l, bound_h);
			failed = 1;
		} else {
			dbgu_printf("SUCCESSFUL\r\n");
		}

		// 3nd test: Set position to extremes and test if they are reached
		dbgu_printf("Test 3 (Positions):     ");
		Servo_SendPosition(id, SERVO_MAX_ANGLE);
		TC_DelayMs(SERVO_TEST_MOVE_DURATION);
		float pos = Servo_ReadPos(id);
		if (pos < SERVO_MAX_ANGLE - SERVO_TEST_POS_TOLERANCE) {
			dbgu_printf("FAILED - Target pos not reached (%f < %f)\r\n", pos, SERVO_MAX_ANGLE - SERVO_TEST_POS_TOLERANCE);
			failed = 1;
			//return -2;
		} else {
			Servo_SendPosition(id, SERVO_MIN_ANGLE);
			TC_DelayMs(SERVO_TEST_MOVE_DURATION);
			pos = Servo_ReadPos(id);
			if (pos > SERVO_MIN_ANGLE + SERVO_TEST_POS_TOLERANCE) {
				dbgu_printf("FAILED - Target pos not reached (%f > %f)\r\n", pos, SERVO_MIN_ANGLE + SERVO_TEST_POS_TOLERANCE);
				failed = 1;
				//return -2;
			} else {
				dbgu_printf("SUCCESSFUL\r\n");
			}
		}

		// 4th test: Check load while moving
		dbgu_printf("Test 4 (Load):          ");
		Servo_SendPosition(id, SERVO_MAX_ANGLE);
		unsigned char maxload = 0;
		for (int j = 0; j < SERVO_TEST_MOVE_DURATION / 2; j++) {
			unsigned char load = Servo_ReadLoad(id);
			if (load > maxload) maxload = load;
			TC_DelayMs(2);
		}
		Servo_SendPosition(id, SERVO_MIN_ANGLE);
		for (int j = 0; j < SERVO_TEST_MOVE_DURATION / 2; j++) {
			unsigned char load = Servo_ReadLoad(id);
			if (load > maxload) maxload = load;
			TC_DelayMs(2);
		}
		if (maxload > SERVO_TEST_LOAD_THRES) {
			dbgu_printf("FAILED - Load during movement too high (%d > %d)\r\n", maxload, SERVO_TEST_LOAD_THRES);
			failed = 1;
		} else if (maxload == 0) {
			dbgu_printf("FAILED - No load during movement\r\n");
			failed = 1;
		} else {
			dbgu_printf("SUCCESSFUL (%d)\r\n", maxload);
		}

		if (!failed) {
			dbgu_printf("\r\nOverall result: Servo OK\r\n");
		} else {
			dbgu_printf("\r\nOverall result: Servo FAILED!\r\n");
		}

		dbgu_printf("\r\nServo weights:\r\n");
		dbgu_printf("All plastic	 gears: 46.7 g\r\n");
		dbgu_printf("One plastic gear:  50.7 g\r\n");
		dbgu_printf("All metal gears:   51.7 g\r\n");

		Servo_SetOffsetEnable(1);

		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_setservoboundary(char * args[], char num_args) {
	if (num_args >= 3) {
		unsigned char id = atoi(args[0]);
		unsigned short lower = atoi(args[1]);
		unsigned short upper = atoi(args[2]);

		Servo_SetBoundaries(id, lower, upper);

		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_checkservo(char * args[], char num_args) { // Future Syntax: checkservo [CheckingServo] [CheckedServo]
	if (num_args >= 2) {
		unsigned char id = atoi(args[0]); // CheckingServo-ID
		unsigned char id2 = atoi(args[1]); // CheckedServo-ID

		// Check if the choosen servos respond
		if (Servo_IsResponding(id) != 1) {
			dbgu_printf("Servo %d did not respond!\r\n", id);
			return -2;
		}
		if (Servo_IsResponding(id2) != 1) {
			dbgu_printf("Servo %d did not respond!\r\n", id2);
			return -2;
		}

		// Set CheckedServo in Passive-Mode
		Servo_SetPassive(id2, 1);

		float defaultpos;
		float sendpos;

		//Compare if both servos are on same position
		for (unsigned char i = 0; i <= 253; i++) {
			Servo_SetPosition(id, i);
			Servo_SendPosition(id, i);

			defaultpos = Servo_ReadPos(id);
			sendpos = Servo_ReadPos(id2);

			if (defaultpos != sendpos) {
				dbgu_printf("Error on position %f\r\n", defaultpos);
			} else if (i == 253 && defaultpos == sendpos) {
				dbgu_printf("Servo passed :) \r\n");
				return 0;
			}
		}
		return -3;
	} else {
		return -1;
	}
}

int shell_cmd_saveconfig(char * args[], char num_args) {
	if (Config_Save() == 0) {
		dbgu_printf("Failed to save config to EEPROM!\r\n");
		return -2;
	}
	dbgu_printf("Successfully saved configuration!\r\n");
	return 0;
}

int shell_cmd_eraseeeprom(char * args[], char num_args) {
	dbgu_printf("This will erase all contents of the EEPROM. Are you sure (y/n)? ");
	char ans = dbgu_get_char();
	dbgu_printf("%c\r\n", ans);
	if (ans == 'y') {
		eeprom_erase(1);
	}
	return 0;
}

int shell_cmd_motioneditor(char * args[], char num_args) {
	if (num_args >= 1) {
		unsigned char idx = atoi(args[0]);
		MotionEditor_Main(idx);
		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_showservopos(char * args[], char num_args) {
	if (num_args >= 2) {
		unsigned char id = atoi(args[0]);
		unsigned char passive = atoi(args[1]);

		// Check if the choosen servos respond
		if (Servo_IsResponding(id) != 1) {
			dbgu_printf("Servo %d did not respond!\r\n", id);
			return -2;
		}

		// Set CheckedServo in Passive-Mode
		if (passive > 0) {
			Servo_SetPassive(id, 1);
		}

		while (1) {
			unsigned long loopStartTime = TC_GetSystemTicks();
			UI_ToggleLED(1);

			float pos = Servo_ReadPos(id);
			dbgu_printf("%f\r\n", pos);

			if (dbgu_char_available()) {
				if (dbgu_get_char() == 3) { // Ctrl + C
					break;
				}
			}

			// Waste remaining loop time
			while (TC_GetMsSinceTick(loopStartTime) < LOOP_UPDATE_INTERVAL);
		}
		Servo_SendPositions(); // Warning: Could be dangerous when robot is in scaffolding
		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_selectmotion(char * args[], char num_args) {
	struct t_motion_select motionSelects;

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

	if (num_args >= 2) {
		unsigned char motionID = atoi(args[0]);
		unsigned char motionPage = atoi(args[1]);

		if (motionID < END_OF_MOTION_SELECT_LIST) {
			motionSelects.motionIDs[motionID] = motionPage;

			unsigned short addr = eeprom_write("MOTION SELECTS", &motionSelects, sizeof(motionSelects));
			if (addr > 0) {
				dbgu_printf("Saved motion page %d for motion %s\r\n", motionPage, GetMotionSelectIDName(motionID));
				dbgu_printf("Current motion IDs / pages:\r\n");
				for (unsigned char i = 0; i < END_OF_MOTION_SELECT_LIST; ++i) {
					dbgu_printf("%d - %s: Page %d\r\n", i, GetMotionSelectIDName(i), motionSelects.motionIDs[i]);
				}
				return 0;
			} else {
				dbgu_printf("Failed to save motion assignment! (No EEPROM?)\r\n");
				return -3;
			}
		} else {
			dbgu_printf("Invalid motion ID!\r\n");
			return -2;
		}
	} else {
		dbgu_printf("Usage: selectmotion <motionID> <index of motion page>\r\n");
		dbgu_printf("Current motion IDs / pages:\r\n");
		for (unsigned char i = 0; i < END_OF_MOTION_SELECT_LIST; ++i) {
			dbgu_printf("%d - %s: Page %d\r\n", i, GetMotionSelectIDName(i), motionSelects.motionIDs[i]);
		}
		return -1;
	}
}

int shell_cmd_setservooverload(char * args[], char num_args) {
	if (num_args >= 2) {
		unsigned char id = atoi(args[0]);
		unsigned short over_ma = atoi(args[1]);

		unsigned char over;
		switch (over_ma) {
			case 400: over = 33; break;
			case 500: over = 44; break;
			case 600: over = 56; break;
			case 700: over = 68; break;
			case 800: over = 80; break;
			case 900: over = 92; break;
			case 1000: over = 104; break;
			case 1100: over = 116; break;
			case 1200: over = 128; break;
			case 1800: over = 199; break;
			default: return -1;
		}

		Servo_SetOverload(id, over);

		return 0;
	} else {
		dbgu_printf("Possible overload thresholds:\r\n");
		for (unsigned short i = 400; i <= 1200; i += 100) {
			dbgu_printf("* %d ma\r\n", i);
		}
		dbgu_printf("* 1800 ma\r\n");
		return -1;
	}
}

int shell_cmd_setservopid(char * args[], char num_args) {
	if (num_args >= 4) {
		unsigned char id = atoi(args[0]);
		unsigned char p = atoi(args[1]);
		unsigned char i = atoi(args[2]);
		unsigned char d = atoi(args[3]);

		Servo_SendTGain(id, p, i, d);

		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_bootloader(char * args[], char num_args) {
	Bootloader_start();
	return 0;
}

int shell_cmd_receivemotionfile(char * args[], char num_args) {
	dbgu_printf("Send motion file (.bin) via XMODEM now!\r\n");
	xmodem_receiveEEPROM();
	return 0;
}

int shell_cmd_dumpeeprom(char * args[], char num_args) {
	if (num_args >= 1) {
		unsigned char id = atoi(args[0]);
/*		unsigned char buffer[128];
		unsigned int pos = 0;
		unsigned int length = EEPROM_CONFIG_SIZE - 1;
		if (id == 1) {
			length = EEPROM_MOTION_SIZE - 1;
		}
		do {
			eeprom_readRaw(id, pos, &buffer[0], 128);
			pos += 128;
			for (unsigned char i = 0; i < 128; ++i) {
				dbgu_print_char_in_hex(buffer[i]);
				if (i % 16 == 15) {
					dbgu_print_ascii("\r\n");
				} else {
					dbgu_print_char(' ');
				}
			}
		} while (pos < length);
*/
		dbgu_printf("Start file reception via XMODEM now!\r\n");
		xmodem_sendEEPROM(id);
		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_servopower(char * args[], char num_args) {
	if (num_args >= 1) {
		unsigned char state = atoi(args[0]);
		if (state == 0) {
			Servo_SetPowerState(0);
		} else {
			Servo_SetPowerState(1);
		}
		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_settype(char * args[], char num_args) {
	if (num_args >= 2) {
		unsigned char id = atoi(args[0]);
		unsigned char type = atoi(args[1]);
		Servo_SetType(id, type);
		return 0;
	} else {
		return -1;
	}
}
int shell_cmd_showOdometrieData(char * args[], char num_args)
{
	struct position* p = odometry_getPosition();
	dbgu_printf("Position X = %d, Y = %d, View = %d",p->xPos,(int)p->yPos,(int)p->viewDirection);
	return 0;
}
int shell_cmd_setOdometrieDataXChange(char * args[],char num_args)
{
	odometry_setxChange(atoi(args[0]));
	return 0;
}
int shell_cmd_setOdometrieDataYChange(char * args[],char num_args)
{
	odometry_setyChange(atoi(args[0]));
	return 0;
}
int shell_cmd_setOdometrieDataViewChange(char * args[],char num_args)
{
	odometry_setViewChange(atoi(args[0]));
	return 0;
}
int shell_cmd_resetOdometrie(char * args[],char num_args)
{
	odometry_initOdemetry();
	return 0;
}
int shell_cmd_saveOdometrie(char * args[],char num_args)
{
	odometry_saveChanges();
	return 0;
}
int shell_cmd_showmagnet(char * args[], char num_args)
{
	while (1) {
		unsigned long loopStartTime = TC_GetSystemTicks();

		UI_ToggleLED(1);
		IMU_Update();

		int magnetX, magnetY, magnetZ;
		magnetX = IMU_GetUDMagnet();
		magnetY = IMU_GetRLMagnet();
		magnetZ = IMU_GetFBMagnet();
		double angle = IMU_CalculateHeadingTiltCompensated();
		dbgu_printf("angle = %.3f, angle = %.3f °, x = %d, y = %d, z = %d\r\n", angle, angle * 180.0f/M_PI, magnetX, magnetY, magnetZ);

		if (dbgu_char_available()) {
			if (dbgu_get_char() == 3) { // Ctrl + C
				return 0;
			}
		}

		// Waste remaining loop time
		while (TC_GetMsSinceTick(loopStartTime) < MAIN_LOOP_DURATION);
	}

	return 0;
}

int shell_cmd_offsettuner(char * args[], char num_args) {
	OffsetTuner_Main();
	return 0;
}
int shell_cmd_fakelook(char * args[], char num_args)
{
	int cancel = 1;
	int look = 0;
	int waitTime = 0;
	while(cancel)
	{
		int ra = rand() % 100;
		unsigned long loopStartTime = TC_GetSystemTicks();
		if (dbgu_char_available() == 1)
		{
			int ch = dbgu_get_char();
			if(ch == 0x03||ch == 0x27)
			{
				cancel =0;
			}

		}
		dbgu_printf("Starting with %d \r\n",look);
		Servo_SetPosition(19,90-look*10/180);
		int fahr = (look%360)-90;
		if(fahr>90)
		{
			fahr= 180-fahr;
		}
		Servo_SetPosition(18,fahr);
		Servo_SendPositions();

		if(look>900)
		{
			look = 0;
		}
		look = look +atoi(args[0]);
		if(ra>96)
		{
			waitTime = 800;
		}else
		{
			waitTime = 0;
		}
		while (TC_GetMsSinceTick(loopStartTime) < MAIN_LOOP_DURATION+waitTime) {};
	}

	return 0;
}

int shell_cmd_setbaudrate(char * args[], char num_args) {
	if (num_args >= 1) {
		unsigned int baud = atoi(args[0]);
		uart1_SetBaudrate(baud);
		dbgu_printf("Set baudrate to %d baud.\r\n", baud);
		return 0;
	} else {
		return -1;
	}
}

int shell_cmd_searchservos(char * args[], char num_args) {
	int baudrates[] = { 2000000, 1000000, 500000, 400000, 250000, 200000, 117647, 57142, 19230, 9615 };
	int cnt = 0;
	unsigned char servocnt = 31;

	if (num_args >= 1) {
		for (unsigned char i = 0; i < 254; ++i) {
			unsigned int baud = 2000000 / (i + 1);
			uart1_SetBaudrate(baud);
			dbgu_printf("Set baudrate to %d baud (%d).\r\n", baud, i);

			dbgu_printf("Responding servos: ");
			for (int j = 0; j < servocnt; j++) {
				if (Servo_IsResponding(j) == 1) {
					dbgu_printf("%d ", j);
					cnt++;
				}
			}
			dbgu_printf("\r\n");
		}
	} else {
		for (unsigned char i = 0; i < (sizeof(baudrates) / sizeof(int)); ++i) {
			unsigned int baud = baudrates[i];
			uart1_SetBaudrate(baud);
			dbgu_printf("Set baudrate to %d baud.\r\n", baud);

			dbgu_printf("Responding servos: ");
			for (int j = 0; j < servocnt; j++) {
				if (Servo_IsResponding(j) == 1) {
					dbgu_printf("%d ", j);
					cnt++;
				}
			}
			dbgu_printf("\r\n");
		}
	}
	dbgu_printf("\r\n%d servos found.\r\n", cnt);
	uart1_SetBaudrate(AT91_BAUD_RATE_SERVO);
	return 0;
}

int shell_cmd_setservobaud(char * args[], char num_args) {
	if (num_args >= 2) {
		unsigned char id = atoi(args[0]);
		unsigned int baud = atoi(args[1]);
		Servo_SetBaudrate(id, baud);
		return 0;
	} else {
		return -1;
	}
}
