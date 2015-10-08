/*
 * kicker.c
 *
 *  Created on: 13.11.2012
 *      Author: Oliver
 */

#include "kicker.h"
#include "balance.h"
#include "kinematics/vector.h"
#include "walker/walker.h"
#include "eeprom/eeprom.h"
#include "tc/tc.h"
#include "servo/servo.h"
#include "imu/imu.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

// use the balancing function, not recommended!
//#define KICKER_USE_BALANCE

#define KICKER_R_SHOULDER_PITCH  +0.7 // in radians
#define KICKER_L_SHOULDER_PITCH  -0.7 // in radians

#define DEGREES_TO_RADIANS 0.017453

#define BALL_RADIUS 34 // 33.5 -> float, but is not stringently more accurate (radius can vary), so it is better to compute with this integer value

signed short kicker_balance_z = LEG_VEC_Z_MAX - 10;

// Note: all lengths are in millimeter (mm)
#ifdef DYNAMIXEL
	#define KICKER_SHOOT_RANGE_MAXIMAL	3500.0 	// maximal shoot range with kicker type maximal
	#define KICKER_SHOOT_RANGE_POWER	3200.0 	// maximal shoot range with kicker type power
	#define KICKER_SHOOT_RANGE_MEDIUM	3000.0 	// maximal shoot range with kicker type medium
	#define KICKER_SHOOT_RANGE_SHORT	1000.0 	// maximal shoot range with kicker type short
	#define KICKER_SHOOT_RANGE_MINIMAL	 500.0 	// maximal shoot range with kicker type minimal

	#define KICKER_BALL_X_MIN			-100.0	// minimal distance of ball in X-axis
	#define KICKER_BALL_X_MAX			+150.0	// maximal distance of ball in X-axis
	#define KICKER_BALL_Y_MIN			-150.0	// minimal distance of ball in Y-axis
	#define KICKER_BALL_Y_MAX			+150.0	// maximal distance of ball in Y-axis

	#define KICKER_OFFSET_TOE			+52		// difference of middle of bot to hoe (X-axis)
	#define KICKER_OFFSET_HEEL			-52		// difference of middle of bot to heel (X-axis)
	#define KICKER_OFFSET_RIGHT			+50		// difference of middle of bot to right foot (Y-axis)
	#define KICKER_OFFSET_LEFT			-50		// difference of middle of bot to left foot (Y-axis)
	#define KICKER_OFFSET_Z				LEG_VEC_Z_MAX		// difference of foot plate to center of bot (Z-axis)

	#define KICKER_BALANCE_Y_RIGHT		0//+62 					// end position of legs in y for balance phase
	#define KICKER_BALANCE_Y_LEFT		0//-62					// end position of legs in y for balance phase
	#define KICKER_SHOOT_Z				(kicker_balance_z - BALL_RADIUS - 10)	// shooting leg in z
	#define KICKER_RESET_SHOOT_Z		(kicker_balance_z - 12) // - 8

	#define KICKER_STEP_MINIMAL			9
	#define KICKER_STEP_SHORT			9
	#define KICKER_STEP_MEDIUM			10
	#define KICKER_STEP_POWER			10
	#define KICKER_STEP_MAXIMAL			10

	#define KICKER_BACK_X_MINIMAL		 90
	#define KICKER_BACK_X_SHORT			100
	#define KICKER_BACK_X_MEDIUM		120
	#define KICKER_BACK_X_POWER			120
	#define KICKER_BACK_X_MAXIMAL		120

	#define KICKER_RIGHT_FOOT_Y_MIN		+65			// minimal y for right foot
	#define KICKER_LEFT_FOOT_Y_MAX		-65			// maximal y for left foot

	#define KICKER_HIP_PITCH_ANGLE_MAX 	1.0f //0.7f //0.5 //0.3

#else
	#define KICKER_SHOOT_RANGE_MAXIMAL	1000.0 	// maximal shoot range with kicker type maximal
	#define KICKER_SHOOT_RANGE_POWER	800.0 	// maximal shoot range with kicker type power
	#define KICKER_SHOOT_RANGE_MEDIUM	500.0 	// maximal shoot range with kicker type medium
	#define KICKER_SHOOT_RANGE_SHORT	200.0 	// maximal shoot range with kicker type short
	#define KICKER_SHOOT_RANGE_MINIMAL	100.0 	// maximal shoot range with kicker type minimal

	#define KICKER_BALL_X_MIN			-50.0	// maximal distance of ball in X-axis
	#define KICKER_BALL_X_MAX			+50.0	// maximal distance of ball in X-axis
	#define KICKER_BALL_Y_MIN			-50.0	// maximal distance of ball in Y-axis
	#define KICKER_BALL_Y_MAX			+50.0	// maximal distance of ball in Y-axis

	#define KICKER_OFFSET_TOE			+50		// difference of middle of bot to hoe (X-axis)
	#define KICKER_OFFSET_HEEL			-50		// difference of middle of bot to heel (X-axis)
	#define KICKER_OFFSET_RIGHT			+36		// difference of middle of bot to middle of right foot (Y-axis)
	#define KICKER_OFFSET_LEFT			-36		// difference of middle of bot to middle of left foot (Y-axis)
	#define KICKER_OFFSET_Z				LEG_VEC_Z_MAX		// difference of foot plate to center of bot

	#define KICKER_BALANCE_Y_RIGHT		+40						// end position of legs in y for balance phase
	#define KICKER_BALANCE_Y_LEFT		-40						// end position of legs in y for balance phase
	//#define KICKER_BALANCE_Z			(LEG_VEC_Z_MAX - 10)	// end position of legs in z for balance phase
	#define KICKER_SHOOT_Z				(kicker_balance_z - BALL_RADIUS - 10)	// shooting leg in z
	#define KICKER_RESET_SHOOT_Z		(kicker_balance_z - 4)

	#define KICKER_STEP_MINIMAL			 5
	#define KICKER_STEP_SHORT			10
	#define KICKER_STEP_MEDIUM			15
	#define KICKER_STEP_POWER			20
	#define KICKER_STEP_MAXIMAL			25

	#define KICKER_BACK_X_MINIMAL		20
	#define KICKER_BACK_X_SHORT			30
	#define KICKER_BACK_X_MEDIUM		40
	#define KICKER_BACK_X_POWER			50
	#define KICKER_BACK_X_MAXIMAL		60

	#define KICKER_RIGHT_FOOT_Y_MIN		+50
	#define KICKER_LEFT_FOOT_Y_MAX		-50

	#define KICKER_HIP_PITCH_ANGLE_MAX 	0.3f

#endif

#define KICKER_TIMER_INTERVAL		 10		// do not change this value, only if you know what you do!
#define KICKER_CALL_COUNTER_LIMIT 	200 	// limit for one kick phase, cancel if reached (= <time in ms> / timer-interval = 2000 / 10)

struct vect3d_ext kicker_shootingFoot_back;   	// position of shooting foot at end of swing back phase
struct vect3d_ext kicker_shootingFoot_target; 	// target position of shooting foot (end of kick phase)
struct vect3d_ext kicker_shootingFoot_fade;   	// position of shooting foot at end of kick fade phase
struct vect3d_ext kicker_shootingFoot_kickStep; // step sizes for kick (from back position to target position)

struct vect3d_ext kicker_extremity[COUNT_EXTREMITIES];		// the references for the extremities (arms and legs) of the robot

enum kicker_type kicker_kick_type = KICKER_TYPE_MEDIUM;
unsigned char kicker_isRunning		= 0;			// if a kick is executed currently, this value is 1 (else 0)
unsigned char kicker_kickWithRight	= 0;			// 1: kicks with right foot; 0: kicks with left foot
unsigned char kicker_kickHigh		= 0;
unsigned char kicker_fadeEnable		= 1;
signed short kicker_shoot_range		= 0;			// how far the ball should go with the kick
signed short kicker_shoot_z 		= 0;
signed short kicker_call_counter 	= 0;

float kicker_AnklePitchAddAngle		= 0.f;
float kicker_HipPitchAddAngle 		= 0.f;
float kicker_HipPitchAddAngle_Rate 	= 0.f;
float kicker_RangeMultiplierLeg		= 1.f;
float kicker_RangeMultiplierHip		= 1.f;

unsigned char Kicker_IsRunning(void)
{
	return kicker_isRunning;
}

signed short Kicker_GetBalanceLeftOffset(void)
{
	return kicker_Params.balance_y_l_offset;
}

signed short Kicker_GetBalanceRightOffset(void)
{
	return kicker_Params.balance_y_r_offset;
}

void Kicker_AddBalanceLeftOffset( signed short a)
{
	kicker_Params.balance_y_l_offset += a;
}

void Kicker_AddBalanceRightOffset( signed short a)
{
	kicker_Params.balance_y_r_offset += a;
}

void Kicker_ResetParameters(void)
{
	kicker_Params.version = 0;
	kicker_Params.balance_y_l_offset = -68;
	kicker_Params.balance_y_r_offset = +68;
#ifdef KICKER_USE_BALANCE
	kicker_Params.balance_rl_p_gain = 0.24f;
	kicker_Params.balance_rl_i_gain = 0.0f;
	kicker_Params.balance_rl_d_gain = 0.10f;
	kicker_Params.balance_fb_p_gain = 0.10f;
	kicker_Params.balance_fb_i_gain = 0.0f;
	kicker_Params.balance_fb_d_gain = 0.04f;
	kicker_Params.balance_enable = 1;
#else
	kicker_Params.balance_rl_p_gain = 0.0f;
	kicker_Params.balance_rl_i_gain = 0.0f;
	kicker_Params.balance_rl_d_gain = 200.0f;
	kicker_Params.balance_fb_p_gain = 0.0f;
	kicker_Params.balance_fb_i_gain = 0.0f;
	kicker_Params.balance_fb_d_gain = 200.0f;
	kicker_Params.balance_enable = 0;
#endif

	kicker_ServoParams.servo_gain_p = 32.f;
	kicker_ServoParams.servo_gain_i = 0.f;
	kicker_ServoParams.servo_gain_d = 0.f;

	kicker_OffsetParams.hip_roll_offset_l = 0;
	kicker_OffsetParams.hip_roll_offset_r = 0;
	kicker_OffsetParams.hip_pitch_offset_l = 0;
	kicker_OffsetParams.hip_pitch_offset_r = 0;
	kicker_OffsetParams.ankle_roll_offset_l = 0;
	kicker_OffsetParams.ankle_roll_offset_r = 0;
	kicker_OffsetParams.ankle_pitch_offset_l = 0;
	kicker_OffsetParams.ankle_pitch_offset_r = 0;
}

void Kicker_SetServoParams(void)
{
	Servo_SendTGainGlobal(kicker_ServoParams.servo_gain_p, kicker_ServoParams.servo_gain_i, kicker_ServoParams.servo_gain_d);
}

void Kicker_LoadParams(void)
{
	eeprom_read("KICKER", &kicker_Params, sizeof(kicker_Params));
	eeprom_read("KICKER_SERVO", &kicker_ServoParams, sizeof(kicker_ServoParams));
	eeprom_read("KICKER_OFFSETS", &kicker_OffsetParams, sizeof(kicker_OffsetParams));
}

void Kicker_SaveParams(void)
{
	eeprom_write("KICKER", &kicker_Params, sizeof(kicker_Params));
	eeprom_write("KICKER_SERVO", &kicker_ServoParams, sizeof(kicker_ServoParams));
	eeprom_write("KICKER_OFFSETS", &kicker_OffsetParams, sizeof(kicker_OffsetParams));
}

void Kicker_Init(void)
{
	Kicker_ResetParameters();
	Kicker_LoadParams();
}

void Kicker_AddOffsetsRight(void)
{
	inverse_addServoAngle( SERVO_ID_L_HIP_ROLL, kicker_OffsetParams.hip_roll_offset_r * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_R_HIP_ROLL, kicker_OffsetParams.hip_roll_offset_r * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_L_HIP_PITCH, kicker_OffsetParams.hip_pitch_offset_r * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_R_HIP_PITCH, -kicker_OffsetParams.hip_pitch_offset_r * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_L_ANKLE_ROLL, kicker_OffsetParams.ankle_roll_offset_r * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_R_ANKLE_ROLL, kicker_OffsetParams.ankle_roll_offset_r * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, kicker_OffsetParams.ankle_pitch_offset_r * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, -kicker_OffsetParams.ankle_pitch_offset_r * DEGREES_TO_RADIANS);
}

void Kicker_AddOffsetsLeft(void)
{
	inverse_addServoAngle( SERVO_ID_L_HIP_ROLL, kicker_OffsetParams.hip_roll_offset_l * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_R_HIP_ROLL, kicker_OffsetParams.hip_roll_offset_l * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_L_HIP_PITCH, kicker_OffsetParams.hip_pitch_offset_l * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_R_HIP_PITCH, -kicker_OffsetParams.hip_pitch_offset_l * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_L_ANKLE_ROLL, kicker_OffsetParams.ankle_roll_offset_l * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_R_ANKLE_ROLL, kicker_OffsetParams.ankle_roll_offset_l * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, kicker_OffsetParams.ankle_pitch_offset_l * DEGREES_TO_RADIANS);
	inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, -kicker_OffsetParams.ankle_pitch_offset_l * DEGREES_TO_RADIANS);
}

void Kicker_ResetLeft(void)
{
	kicker_call_counter = 0;
	kicker_AnklePitchAddAngle = 0.f;
	kicker_HipPitchAddAngle = 0.f;
	kicker_HipPitchAddAngle_Rate = 0.f;
	kicker_kickHigh = 0;

/*	kicker_extremity[ARM_LEFT].x = ARM_VEC_X_INIT;
	kicker_extremity[ARM_LEFT].y = 40;
	kicker_extremity[ARM_LEFT].z = ARM_VEC_Z_INIT;
	kicker_extremity[ARM_LEFT].yaw = 0;

	kicker_extremity[ARM_RIGHT].x = ARM_VEC_X_INIT;
	kicker_extremity[ARM_RIGHT].y = 40;
	kicker_extremity[ARM_RIGHT].z = ARM_VEC_Z_INIT;
	kicker_extremity[ARM_RIGHT].yaw = 0;*/

#ifdef KICKER_USE_BALANCE
	kicker_extremity[LEG_LEFT].x = LEG_VEC_X_INIT;
	kicker_extremity[LEG_LEFT].y = LEG_VEC_Y_INIT;
	kicker_extremity[LEG_LEFT].z = LEG_VEC_Z_INIT;
	kicker_extremity[LEG_RIGHT].x = LEG_VEC_X_INIT;
	kicker_extremity[LEG_RIGHT].y = LEG_VEC_Y_INIT;
	kicker_extremity[LEG_RIGHT].z = LEG_VEC_Z_INIT;
	kicker_balance_z = LEG_VEC_Z_INIT;
#else
	kicker_extremity[LEG_LEFT].x = kicker_Params.balance_fb_p_gain;
	kicker_extremity[LEG_LEFT].y = kicker_Params.balance_fb_i_gain;
	kicker_extremity[LEG_LEFT].z = kicker_Params.balance_fb_d_gain;
	kicker_extremity[LEG_RIGHT].x = kicker_Params.balance_fb_p_gain;
	kicker_extremity[LEG_RIGHT].y = kicker_Params.balance_fb_i_gain;
	kicker_extremity[LEG_RIGHT].z = kicker_Params.balance_fb_d_gain;
	kicker_balance_z = kicker_Params.balance_fb_d_gain;
#endif
	kicker_extremity[LEG_LEFT].yaw = 0;
	kicker_extremity[LEG_RIGHT].yaw = 0;

	inverse_startCycle();
	inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
	inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
	//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
	//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
	Kicker_AddOffsetsLeft();
	inverse_endCycle(0);

#ifdef DYNAMIXEL
	Servo_SetPosition(SERVO_ID_R_SHOULDER_PITCH, KICKER_R_SHOULDER_PITCH*180/M_PI);
	Servo_SetPosition(SERVO_ID_L_SHOULDER_PITCH, KICKER_L_SHOULDER_PITCH*180/M_PI);
	Servo_SetPosition(SERVO_ID_R_SHOULDER_ROLL, 7);
	Servo_SetPosition(SERVO_ID_L_SHOULDER_ROLL, -7);
	Servo_SetPosition(SERVO_ID_R_ELBOW, -20);
	Servo_SetPosition(SERVO_ID_L_ELBOW, 20);
#endif
}

void Kicker_ResetRight(void)
{
	kicker_call_counter = 0;
	kicker_AnklePitchAddAngle = 0.f;
	kicker_HipPitchAddAngle = 0.f;
	kicker_HipPitchAddAngle_Rate = 0.f;
	kicker_kickHigh = 0;

/*	kicker_extremity[ARM_LEFT].x = ARM_VEC_X_INIT;
	kicker_extremity[ARM_LEFT].y = 40;
	kicker_extremity[ARM_LEFT].z = ARM_VEC_Z_INIT;
	kicker_extremity[ARM_LEFT].yaw = 0;

	kicker_extremity[ARM_RIGHT].x = ARM_VEC_X_INIT;
	kicker_extremity[ARM_RIGHT].y = 40;
	kicker_extremity[ARM_RIGHT].z = ARM_VEC_Z_INIT;
	kicker_extremity[ARM_RIGHT].yaw = 0;*/

#ifdef KICKER_USE_BALANCE
	kicker_extremity[LEG_LEFT].x = LEG_VEC_X_INIT;
	kicker_extremity[LEG_LEFT].y = LEG_VEC_Y_INIT;
	kicker_extremity[LEG_LEFT].z = LEG_VEC_Z_INIT;
	kicker_extremity[LEG_RIGHT].x = LEG_VEC_X_INIT;
	kicker_extremity[LEG_RIGHT].y = LEG_VEC_Y_INIT;
	kicker_extremity[LEG_RIGHT].z = LEG_VEC_Z_INIT;
	kicker_balance_z = LEG_VEC_Z_INIT;
#else
	kicker_extremity[LEG_LEFT].x = kicker_Params.balance_rl_p_gain;
	kicker_extremity[LEG_LEFT].y = kicker_Params.balance_rl_i_gain;
	kicker_extremity[LEG_LEFT].z = kicker_Params.balance_rl_d_gain;
	kicker_extremity[LEG_RIGHT].x = kicker_Params.balance_rl_p_gain;
	kicker_extremity[LEG_RIGHT].y = kicker_Params.balance_rl_i_gain;
	kicker_extremity[LEG_RIGHT].z = kicker_Params.balance_rl_d_gain;
	kicker_balance_z = kicker_Params.balance_rl_d_gain;
#endif
	kicker_extremity[LEG_LEFT].yaw = 0;
	kicker_extremity[LEG_RIGHT].yaw = 0;

	inverse_startCycle();
	inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
	inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
	//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
	//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
	Kicker_AddOffsetsRight();
	inverse_endCycle(0);

#ifdef DYNAMIXEL
	Servo_SetPosition(SERVO_ID_R_SHOULDER_PITCH, KICKER_R_SHOULDER_PITCH*180/M_PI);
	Servo_SetPosition(SERVO_ID_L_SHOULDER_PITCH, KICKER_L_SHOULDER_PITCH*180/M_PI);
	Servo_SetPosition(SERVO_ID_R_SHOULDER_ROLL, 7);
	Servo_SetPosition(SERVO_ID_L_SHOULDER_ROLL, -7);
	Servo_SetPosition(SERVO_ID_R_ELBOW, -20);
	Servo_SetPosition(SERVO_ID_L_ELBOW, 20);
#endif
}

unsigned char Kicker_ApproachStep( signed short *var, signed short target, unsigned short step)
{
	signed short diff = target - (*var);
	if( diff == 0) {
		return 1;
	} else if( diff > 0) {
		(*var) += step;
		if( (*var) >= target) {
			(*var) = target;
			return 1;
		}
	} else {
		(*var) -= step;
		if( (*var) <= target) {
			(*var) = target;
			return 1;
		}
	}
	return 0;
}

unsigned char Kicker_ApproachStepFloat( float *var, float target, float step)
{
	float diff = target - (*var);
	if( diff == 0.f) {
		return 1;
	} else if( diff > 0.f) {
		(*var) += step;
		if( (*var) >= target) {
			(*var) = target;
			return 1;
		}
	} else {
		(*var) -= step;
		if( (*var) <= target) {
			(*var) = target;
			return 1;
		}
	}
	return 0;
}

void Kicker_BalanceLeft(void)
{
#ifdef KICKER_USE_BALANCE
	if( kicker_Params.balance_enable) {
		IMU_Update();
		balance_left_fb( kicker_Params.balance_fb_p_gain, kicker_Params.balance_fb_i_gain, kicker_Params.balance_fb_d_gain, KICKER_TIMER_INTERVAL);
		balance_left_rl( kicker_Params.balance_rl_p_gain, kicker_Params.balance_rl_i_gain, kicker_Params.balance_rl_d_gain, KICKER_TIMER_INTERVAL);
		//balance_rl( kicker_Params.BALANCE_RL_GAIN_P, kicker_Params.BALANCE_RL_GAIN_I, kicker_Params.BALANCE_RL_GAIN_D, KICKER_TIMER_INTERVAL);
	}
#endif
}

void Kicker_BalanceLeftRL(void)
{
#ifdef KICKER_USE_BALANCE
	if( kicker_Params.balance_enable) {
		IMU_Update();
		balance_left_rl( kicker_Params.balance_rl_p_gain, kicker_Params.balance_rl_i_gain, kicker_Params.balance_rl_d_gain, KICKER_TIMER_INTERVAL);
		//balance_rl( kicker_Params.BALANCE_RL_GAIN_P, kicker_Params.BALANCE_RL_GAIN_I, kicker_Params.BALANCE_RL_GAIN_D, KICKER_TIMER_INTERVAL);
	}
#endif
}

void Kicker_BalanceRight(void)
{
#ifdef KICKER_USE_BALANCE
	if( kicker_Params.balance_enable) {
		IMU_Update();
		balance_right_fb( kicker_Params.balance_fb_p_gain, kicker_Params.balance_fb_i_gain, kicker_Params.balance_fb_d_gain, KICKER_TIMER_INTERVAL);
		balance_right_rl( kicker_Params.balance_rl_p_gain, kicker_Params.balance_rl_i_gain, kicker_Params.balance_rl_d_gain, KICKER_TIMER_INTERVAL);
		//balance_rl( kicker_Params.BALANCE_RL_GAIN_P, kicker_Params.BALANCE_RL_GAIN_I, kicker_Params.BALANCE_RL_GAIN_D, KICKER_TIMER_INTERVAL);
	}
#endif
}

void Kicker_BalanceRightRL(void)
{
#ifdef KICKER_USE_BALANCE
	if( kicker_Params.balance_enable) {
		IMU_Update();
		balance_right_rl( kicker_Params.balance_rl_p_gain, kicker_Params.balance_rl_i_gain, kicker_Params.balance_rl_d_gain, KICKER_TIMER_INTERVAL);
		//balance_rl( kicker_Params.BALANCE_RL_GAIN_P, kicker_Params.BALANCE_RL_GAIN_I, kicker_Params.BALANCE_RL_GAIN_D, KICKER_TIMER_INTERVAL);
	}
#endif
}

void Kicker_BalanceBoth(void)
{
#ifdef KICKER_USE_BALANCE
	if( kicker_Params.balance_enable) {
		IMU_Update();
		balance_right_fb( kicker_Params.balance_fb_p_gain, kicker_Params.balance_fb_i_gain, kicker_Params.balance_fb_d_gain, KICKER_TIMER_INTERVAL);
		balance_left_fb( kicker_Params.balance_fb_p_gain, kicker_Params.balance_fb_i_gain, kicker_Params.balance_fb_d_gain, KICKER_TIMER_INTERVAL);
		balance_rl( kicker_Params.balance_rl_p_gain, kicker_Params.balance_rl_i_gain, kicker_Params.balance_rl_d_gain, KICKER_TIMER_INTERVAL);
	}
#endif
}

void Kicker_Balance(void)
{
#ifdef KICKER_USE_BALANCE
	if( kicker_Params.balance_enable) {
		inverse_startCycle();
		inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
		inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
		inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
		inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
		IMU_Update();
	//	Kicker_BalanceBody();
		balance_right_fb( kicker_Params.balance_fb_p_gain, kicker_Params.balance_fb_i_gain, kicker_Params.balance_fb_d_gain, KICKER_TIMER_INTERVAL);
		balance_left_fb( kicker_Params.balance_fb_p_gain, kicker_Params.balance_fb_i_gain, kicker_Params.balance_fb_d_gain, KICKER_TIMER_INTERVAL);
		balance_rl( kicker_Params.balance_rl_p_gain, kicker_Params.balance_rl_i_gain, kicker_Params.balance_rl_d_gain, KICKER_TIMER_INTERVAL);
		inverse_endCycle(0);
	}
#endif
}

void Kicker_ResetPhaseRight5()
{
	static unsigned char second = 0;
	second = !second;
	if( second) {
		unsigned char ready = 1;
		if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].z, kicker_extremity[LEG_LEFT].z, 1)) {
			ready = 0;
		} else if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].z, kicker_balance_z, 1)) {
			ready = 0;
			kicker_extremity[LEG_LEFT].z = kicker_extremity[LEG_RIGHT].z;
		}

		if( !ready && kicker_call_counter < KICKER_CALL_COUNTER_LIMIT) {
			inverse_startCycle();
			inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
			inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
			//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
			//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
			Kicker_BalanceBoth();
			Kicker_AddOffsetsRight();
			inverse_endCycle(0);
			kicker_call_counter++;
		} else {
			Kicker_ResetRight();
			TC_AssignTimer1Callback(0); // Disable timer callback
			kicker_isRunning = 0;
		}
	}
}

void Kicker_ResetPhaseRight4()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].y, LEG_VEC_Y_INIT, 1)) {
		ready = 0;
		kicker_extremity[LEG_LEFT].y = kicker_extremity[LEG_RIGHT].y;
		inverse_startCycle();
		inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
		inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
		//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
		//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
		Kicker_BalanceBoth();
		Kicker_AddOffsetsRight();
		inverse_endCycle(0);
		kicker_call_counter++;
	}
	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback(Kicker_ResetPhaseRight5);
	}
}

void Kicker_ResetPhaseRight3()
{
	static unsigned char second = 0;
	second = !second;
	if( second) {
		unsigned char ready = 1;
		if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].z, KICKER_RESET_SHOOT_Z, 1)) {
			ready = 0;
			inverse_startCycle();
			inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
			inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
			//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
			//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
			Kicker_BalanceLeft();
			Kicker_ApproachStepFloat( &kicker_HipPitchAddAngle, 0.f, 0.2f * kicker_HipPitchAddAngle_Rate);
			if( !kicker_kickHigh) {
				inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, kicker_HipPitchAddAngle);
			}
			inverse_addServoAngle( SERVO_ID_L_HIP_PITCH, kicker_HipPitchAddAngle);
			Kicker_AddOffsetsRight();
			inverse_endCycle(0);
			kicker_call_counter++;
		}
		if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
			kicker_call_counter = 0;
			TC_AssignTimer1Callback(Kicker_ResetPhaseRight4);
		}
	}
}

void Kicker_ResetPhaseRight2()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].y, kicker_extremity[LEG_LEFT].y, 1)) {
		ready = 0;
		inverse_startCycle();
		inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
		inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
		//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
		//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
		Kicker_BalanceLeft();
		Kicker_ApproachStepFloat( &kicker_HipPitchAddAngle, 0.f, 0.2f * kicker_HipPitchAddAngle_Rate);
		if( !kicker_kickHigh) {
			inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, kicker_HipPitchAddAngle);
		}
		inverse_addServoAngle( SERVO_ID_L_HIP_PITCH, kicker_HipPitchAddAngle);
		Kicker_AddOffsetsRight();
		inverse_endCycle(0);
		kicker_call_counter++;
	}
	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT){
		kicker_call_counter = 0;
		TC_AssignTimer1Callback(Kicker_ResetPhaseRight3);
	}
}

void Kicker_ResetPhaseRight1()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].x, LEG_VEC_X_INIT, 2)) {
		ready = 0;
		inverse_startCycle();
		inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
		inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
		//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
		//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
		inverse_setServoAngle( SERVO_ID_R_SHOULDER_PITCH, KICKER_R_SHOULDER_PITCH + (float)(kicker_extremity[LEG_RIGHT].x) / 90.f);
		inverse_setServoAngle( SERVO_ID_L_SHOULDER_PITCH, KICKER_L_SHOULDER_PITCH + (float)(kicker_extremity[LEG_RIGHT].x) / 90.f);
		Kicker_BalanceLeft();
		Kicker_ApproachStepFloat( &kicker_HipPitchAddAngle, 0.f, 0.2f * kicker_HipPitchAddAngle_Rate);
		if( !kicker_kickHigh) {
			inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, kicker_HipPitchAddAngle);
		}
		inverse_addServoAngle( SERVO_ID_L_HIP_PITCH, kicker_HipPitchAddAngle);
		Kicker_AddOffsetsRight();
		inverse_endCycle(0);
		kicker_call_counter++;
	}
	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT){
		kicker_call_counter = 0;
		TC_AssignTimer1Callback(Kicker_ResetPhaseRight2);
	}
}

/** ResetPhase:
	reset shooting foot (right) and establish stability on both feet
	1. Yaw-angle (shooting foot)
	2. X-position (shooting foot)
	3. Y-position (shooting foot like other foot)
	4. Z-position (shooting foot like other foot)
	5. Y-position (reset weight stabilizing, position body in center)
	6. Z-position (reset legs in height)
	*/
void Kicker_ResetPhaseRight()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].yaw, 0, 2)) {
		ready = 0;
		inverse_startCycle();
		inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
		inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
		//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
		//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
		Kicker_BalanceLeft();
		Kicker_ApproachStepFloat( &kicker_HipPitchAddAngle, 0.f, 0.2f * kicker_HipPitchAddAngle_Rate);
		if( !kicker_kickHigh) {
			inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, kicker_HipPitchAddAngle);
		}
		inverse_addServoAngle( SERVO_ID_L_HIP_PITCH, kicker_HipPitchAddAngle);
		Kicker_AddOffsetsRight();
		inverse_endCycle(0);
		kicker_call_counter++;
	}
	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback(Kicker_ResetPhaseRight1);
	}
}

/** KickFadePhase:
 * reduce speed and fade motion after the kick
 */
void Kicker_KickFadePhaseRight()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].x, kicker_shootingFoot_fade.x, kicker_shootingFoot_kickStep.x)) {
		ready = 0;
		kicker_shootingFoot_kickStep.x = MAX( 1, kicker_shootingFoot_kickStep.x / 2);
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].y, kicker_shootingFoot_fade.y, kicker_shootingFoot_kickStep.y)) {
		ready = 0;
		kicker_shootingFoot_kickStep.y = MAX( 1, kicker_shootingFoot_kickStep.y / 2);
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].z, kicker_shootingFoot_fade.z, kicker_shootingFoot_kickStep.z)) {
		ready = 0;
		kicker_shootingFoot_kickStep.z = MAX( 1, kicker_shootingFoot_kickStep.z / 2);
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].yaw, kicker_shootingFoot_fade.yaw, kicker_shootingFoot_kickStep.yaw)) {
		ready = 0;
		kicker_shootingFoot_kickStep.yaw = MAX( 1, kicker_shootingFoot_kickStep.yaw / 2);
	}

	inverse_startCycle();
	inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
	inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
	//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
	//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
	inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, kicker_AnklePitchAddAngle);
	if( !kicker_kickHigh) {
		inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, kicker_HipPitchAddAngle);
	}
	inverse_addServoAngle( SERVO_ID_L_HIP_PITCH, kicker_HipPitchAddAngle);
	inverse_setServoAngle( SERVO_ID_R_SHOULDER_PITCH, KICKER_R_SHOULDER_PITCH + (float)(kicker_extremity[LEG_RIGHT].x) / 90.f); // 70
	inverse_setServoAngle( SERVO_ID_L_SHOULDER_PITCH, KICKER_L_SHOULDER_PITCH + (float)(kicker_extremity[LEG_RIGHT].x) / 90.f); // 70
	Kicker_BalanceLeftRL();
	Kicker_AddOffsetsRight();
	inverse_endCycle(0);
	kicker_call_counter++;

	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback(Kicker_ResetPhaseRight);
	}
}

/** KickPhase:
 * move shooting foot (right) to ball
 * 1. X-axis
 * 2. y-axis
 * 3. yaw-angle (no different angle to prior phase used, so commented it out)
	*/
void Kicker_KickPhaseRight()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].x, kicker_shootingFoot_target.x, kicker_shootingFoot_kickStep.x)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].y, kicker_shootingFoot_target.y, kicker_shootingFoot_kickStep.y)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].z, kicker_shootingFoot_target.z, kicker_shootingFoot_kickStep.z)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].yaw, kicker_shootingFoot_target.yaw, kicker_shootingFoot_kickStep.yaw)) {
		ready = 0;
	}

	inverse_startCycle();
	inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
	inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
	//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
	//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
	inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, kicker_AnklePitchAddAngle);
	if( !kicker_kickHigh) {
		inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, kicker_HipPitchAddAngle);
	}
	inverse_addServoAngle( SERVO_ID_L_HIP_PITCH, kicker_HipPitchAddAngle);
	inverse_setServoAngle( SERVO_ID_R_SHOULDER_PITCH, KICKER_R_SHOULDER_PITCH + (float)(kicker_extremity[LEG_RIGHT].x) / 90.f); // 70
	inverse_setServoAngle( SERVO_ID_L_SHOULDER_PITCH, KICKER_L_SHOULDER_PITCH + (float)(kicker_extremity[LEG_RIGHT].x) / 90.f); // 70
	Kicker_BalanceLeftRL();
	Kicker_AddOffsetsRight();
	inverse_endCycle(0);
	kicker_call_counter++;
	kicker_HipPitchAddAngle += kicker_HipPitchAddAngle_Rate;
	kicker_HipPitchAddAngle = MIN( kicker_HipPitchAddAngle, KICKER_HIP_PITCH_ANGLE_MAX);

	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		if( kicker_fadeEnable) {
			TC_AssignTimer1Callback(Kicker_KickFadePhaseRight);
		} else {
			TC_AssignTimer1Callback(Kicker_ResetPhaseRight);
		}
	}
}

/** LegSwingBackPhase:
 * 	move shooting foot (right) to swing back position
 * 	1. X-axis
 * 	2. Y-axis
 * 	3. yaw-angle
	*/
void Kicker_SwingBackPhaseRight()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].x, kicker_shootingFoot_back.x, 2)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].y, kicker_shootingFoot_back.y, 2)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].z, kicker_shootingFoot_back.z, 2)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].yaw, kicker_shootingFoot_back.yaw, 2)) {
		ready = 0;
	}
	inverse_startCycle();
	inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
	inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
	//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
	//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
	inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, kicker_AnklePitchAddAngle);
	if( !kicker_kickHigh) {
		inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, kicker_HipPitchAddAngle);
	}
	inverse_addServoAngle( SERVO_ID_L_HIP_PITCH, kicker_HipPitchAddAngle);
	inverse_setServoAngle( SERVO_ID_R_SHOULDER_PITCH, KICKER_R_SHOULDER_PITCH + (float)(kicker_extremity[LEG_RIGHT].x) / 200.f); // 90
	inverse_setServoAngle( SERVO_ID_L_SHOULDER_PITCH, KICKER_L_SHOULDER_PITCH + (float)(kicker_extremity[LEG_RIGHT].x) / 200.f); // 90
	Kicker_BalanceLeftRL();
	Kicker_AddOffsetsRight();
	inverse_endCycle(0);
	kicker_call_counter++;
	kicker_HipPitchAddAngle -= 0.01f * kicker_HipPitchAddAngle_Rate; //0.02
	kicker_HipPitchAddAngle = MAX( kicker_HipPitchAddAngle, -KICKER_HIP_PITCH_ANGLE_MAX);

	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback(Kicker_KickPhaseRight);
	}
}

/** BalancePhase:
 *  stretch feet (Z-axis)
 * 	weight stabilizing (Y-axis)
 * 	lift shooting foot (right) (Z-axis)
	*/
void Kicker_BalancePhaseRight()
{
	// beine ausstrecken
	unsigned char ready = 0;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].z, kicker_balance_z, 1)) {
		kicker_extremity[LEG_RIGHT].z = kicker_extremity[LEG_LEFT].z;
	}
	else {
		if( kicker_extremity[LEG_LEFT].y < (KICKER_BALANCE_Y_RIGHT + kicker_Params.balance_y_r_offset)) {
			kicker_extremity[LEG_LEFT].y += 1;
			kicker_extremity[LEG_RIGHT].y = kicker_extremity[LEG_LEFT].y;
		}
		else if( kicker_extremity[LEG_RIGHT].z > kicker_shoot_z) {
			kicker_extremity[LEG_RIGHT].z -= 1;
		}
		else {
			ready = 1;
		}
	}
	inverse_startCycle();
	inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
	inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
	//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
	//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
	Kicker_BalanceLeft();
	Kicker_AddOffsetsRight();
	inverse_endCycle(0);
	kicker_call_counter++;

	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback(Kicker_SwingBackPhaseRight);
	}
}

void Kicker_ResetPhaseLeft5()
{
	static unsigned char second = 0;
	second = !second;
	if( second) {
		unsigned char ready = 1;
		if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].z, kicker_extremity[LEG_RIGHT].z, 1)) {
			ready = 0;
		} else if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].z, kicker_balance_z, 1)) {
			ready = 0;
			kicker_extremity[LEG_RIGHT].z = kicker_extremity[LEG_LEFT].z;
		}

		if( !ready && kicker_call_counter < KICKER_CALL_COUNTER_LIMIT) {
			inverse_startCycle();
			inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
			inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
			//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
			//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
			Kicker_BalanceBoth();
			Kicker_AddOffsetsRight();
			inverse_endCycle(0);
			kicker_call_counter++;
		} else {
			Kicker_ResetLeft();
			TC_AssignTimer1Callback(0); // disable timer callback
			kicker_isRunning = 0;
		}
	}
}

void Kicker_ResetPhaseLeft4()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].y, LEG_VEC_Y_INIT, 1)) {
		ready = 0;
		kicker_extremity[LEG_RIGHT].y = kicker_extremity[LEG_LEFT].y;
		inverse_startCycle();
		inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
		inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
		//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
		//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
		Kicker_BalanceBoth();
		Kicker_AddOffsetsLeft();
		inverse_endCycle(0);
		kicker_call_counter++;
	}
	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback( Kicker_ResetPhaseLeft5);
	}
}

void Kicker_ResetPhaseLeft3()
{
	static unsigned char second = 0;
	second = !second;
	if( second) {
		unsigned char ready = 1;
		if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].z, KICKER_RESET_SHOOT_Z, 1)) {
			ready = 0;
			inverse_startCycle();
			inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
			inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
			//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
			//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
			Kicker_BalanceRight();
			Kicker_ApproachStepFloat( &kicker_HipPitchAddAngle, 0.f, 0.2f * kicker_HipPitchAddAngle_Rate);
			if( !kicker_kickHigh) {
				inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, -kicker_HipPitchAddAngle);
			}
			inverse_addServoAngle( SERVO_ID_R_HIP_PITCH, -kicker_HipPitchAddAngle);
			Kicker_AddOffsetsLeft();
			inverse_endCycle(0);
			kicker_call_counter++;
		}
		if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
			kicker_call_counter = 0;
			TC_AssignTimer1Callback( Kicker_ResetPhaseLeft4);
		}
	}
}

void Kicker_ResetPhaseLeft2()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].y, kicker_extremity[LEG_RIGHT].y, 1)) {
		ready = 0;
		inverse_startCycle();
		inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
		inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
		//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
		//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
		Kicker_BalanceRight();
		Kicker_ApproachStepFloat( &kicker_HipPitchAddAngle, 0.f, 0.2f * kicker_HipPitchAddAngle_Rate);
		if( !kicker_kickHigh) {
			inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, -kicker_HipPitchAddAngle);
		}
		inverse_addServoAngle( SERVO_ID_R_HIP_PITCH, -kicker_HipPitchAddAngle);
		Kicker_AddOffsetsLeft();
		inverse_endCycle(0);
		kicker_call_counter++;
	}
	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback( Kicker_ResetPhaseLeft3);
	}
}

void Kicker_ResetPhaseLeft1()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].x, LEG_VEC_X_INIT, 2)) {
		ready = 0;
		inverse_startCycle();
		inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
		inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
		//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
		//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
		inverse_setServoAngle( SERVO_ID_L_SHOULDER_PITCH, KICKER_L_SHOULDER_PITCH - (float)(kicker_extremity[LEG_LEFT].x) / 90.f);
		inverse_setServoAngle( SERVO_ID_R_SHOULDER_PITCH, KICKER_R_SHOULDER_PITCH - (float)(kicker_extremity[LEG_LEFT].x) / 90.f);
		Kicker_BalanceRight();
		Kicker_ApproachStepFloat( &kicker_HipPitchAddAngle, 0.f, 0.2f * kicker_HipPitchAddAngle_Rate);
		if( !kicker_kickHigh) {
			inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, -kicker_HipPitchAddAngle);
		}
		inverse_addServoAngle( SERVO_ID_R_HIP_PITCH, -kicker_HipPitchAddAngle);
		Kicker_AddOffsetsLeft();
		inverse_endCycle(0);
		kicker_call_counter++;
	}
	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback( Kicker_ResetPhaseLeft2);
	}
}

/** ResetPhase:
	reset shooting foot (left) and establish stability on both feet
	1. Yaw-angle (shooting foot)
	2. X-position (shooting foot)
	3. Y-position (shooting foot like other foot)
	4. Z-position (shooting foot like other foot)
	5. Y-position (reset weight stabilizing, position body in center)
	6. Z-position (reset legs in height)
	*/
void Kicker_ResetPhaseLeft()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].yaw, 0, 2)) {
		ready = 0;
		inverse_startCycle();
		inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
		inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
		//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
		//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
		Kicker_BalanceRight();
		Kicker_ApproachStepFloat( &kicker_HipPitchAddAngle, 0.f, 0.2f * kicker_HipPitchAddAngle_Rate);
		if( !kicker_kickHigh) {
			inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, -kicker_HipPitchAddAngle);
		}
		inverse_addServoAngle( SERVO_ID_R_HIP_PITCH, -kicker_HipPitchAddAngle);
		Kicker_AddOffsetsLeft();
		inverse_endCycle(0);
		kicker_call_counter++;
	}
	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback( Kicker_ResetPhaseLeft1);
	}
}

/** KickFadePhase:
 * reduce speed and fade motion after the kick
 */
void Kicker_KickFadePhaseLeft()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].x, kicker_shootingFoot_fade.x, kicker_shootingFoot_kickStep.x)) {
		ready = 0;
		kicker_shootingFoot_kickStep.x = MAX( 1, kicker_shootingFoot_kickStep.x / 2);
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].y, kicker_shootingFoot_fade.y, kicker_shootingFoot_kickStep.y)) {
		ready = 0;
		kicker_shootingFoot_kickStep.y = MAX( 1, kicker_shootingFoot_kickStep.y / 2);
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].z, kicker_shootingFoot_fade.z, kicker_shootingFoot_kickStep.z)) {
		ready = 0;
		kicker_shootingFoot_kickStep.z = MAX( 1, kicker_shootingFoot_kickStep.z / 2);
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].yaw, kicker_shootingFoot_fade.yaw, kicker_shootingFoot_kickStep.yaw)) {
		ready = 0;
		kicker_shootingFoot_kickStep.yaw = MAX( 1, kicker_shootingFoot_kickStep.yaw / 2);
	}

	inverse_startCycle();
	inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
	inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
	//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
	//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
	inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, -kicker_AnklePitchAddAngle);
	if( !kicker_kickHigh) {
		inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, -kicker_HipPitchAddAngle);
	}
	inverse_addServoAngle( SERVO_ID_R_HIP_PITCH, -kicker_HipPitchAddAngle);
	inverse_setServoAngle( SERVO_ID_L_SHOULDER_PITCH, KICKER_L_SHOULDER_PITCH - (float)(kicker_extremity[LEG_LEFT].x) / 90.f);
	inverse_setServoAngle( SERVO_ID_R_SHOULDER_PITCH, KICKER_R_SHOULDER_PITCH - (float)(kicker_extremity[LEG_LEFT].x) / 90.f);
	Kicker_BalanceRightRL();
	Kicker_AddOffsetsLeft();
	inverse_endCycle(0);
	kicker_call_counter++;

	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback(Kicker_ResetPhaseLeft);
	}
}


/** KickPhase:
 * move shooting foot (left) to ball
 * 1. X-axis
 * 2. Y-axis
 * 3. yaw-angle (no different angle to prior phase used, so commented it out)
	*/
void Kicker_KickPhaseLeft()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].x, kicker_shootingFoot_target.x, kicker_shootingFoot_kickStep.x)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].y, kicker_shootingFoot_target.y, kicker_shootingFoot_kickStep.y)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].z, kicker_shootingFoot_target.z, kicker_shootingFoot_kickStep.z)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].yaw, kicker_shootingFoot_target.yaw, kicker_shootingFoot_kickStep.yaw)) {
		ready = 0;
	}

	inverse_startCycle();
	inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
	inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
	//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
	//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
	inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, -kicker_AnklePitchAddAngle);
	if( !kicker_kickHigh) {
		inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, -kicker_HipPitchAddAngle);
	}
	inverse_addServoAngle( SERVO_ID_R_HIP_PITCH, -kicker_HipPitchAddAngle);
	inverse_setServoAngle( SERVO_ID_L_SHOULDER_PITCH, KICKER_L_SHOULDER_PITCH - (float)(kicker_extremity[LEG_LEFT].x) / 90.f);
	inverse_setServoAngle( SERVO_ID_R_SHOULDER_PITCH, KICKER_R_SHOULDER_PITCH - (float)(kicker_extremity[LEG_LEFT].x) / 90.f);
	Kicker_BalanceRightRL();
	Kicker_AddOffsetsLeft();
	inverse_endCycle(0);
	kicker_HipPitchAddAngle += kicker_HipPitchAddAngle_Rate;
	kicker_HipPitchAddAngle = MIN( kicker_HipPitchAddAngle, KICKER_HIP_PITCH_ANGLE_MAX);
	kicker_call_counter++;

	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		if( kicker_fadeEnable) {
			TC_AssignTimer1Callback(Kicker_KickFadePhaseLeft);
		} else {
			TC_AssignTimer1Callback(Kicker_ResetPhaseLeft);
		}
	}
}


/** LegSwingBackPhase:
 * 	move shooting foot (left) to swing back position
 * 	1. X-axis
 * 	2. Y-axis
 * 	3. yaw-angle
	*/
void Kicker_SwingBackPhaseLeft()
{
	unsigned char ready = 1;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].x, kicker_shootingFoot_back.x, 2)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].y, kicker_shootingFoot_back.y, 2)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].z, kicker_shootingFoot_back.z, 2)) {
		ready = 0;
	}
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_LEFT].yaw, kicker_shootingFoot_back.yaw, 2)) {
		ready = 0;
	}
	inverse_startCycle();
	inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
	inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
	//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
	//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
	inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, -kicker_AnklePitchAddAngle);
	if( !kicker_kickHigh) {
		inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, -kicker_HipPitchAddAngle);
	}
	inverse_addServoAngle( SERVO_ID_R_HIP_PITCH, -kicker_HipPitchAddAngle);
	inverse_setServoAngle( SERVO_ID_L_SHOULDER_PITCH, KICKER_L_SHOULDER_PITCH - (float)(kicker_extremity[LEG_LEFT].x) / 200.f);
	inverse_setServoAngle( SERVO_ID_R_SHOULDER_PITCH, KICKER_R_SHOULDER_PITCH - (float)(kicker_extremity[LEG_LEFT].x) / 200.f);
	Kicker_BalanceRightRL();
	Kicker_AddOffsetsLeft();
	inverse_endCycle(0);
	kicker_HipPitchAddAngle -= 0.01f * kicker_HipPitchAddAngle_Rate; // 0.02
	kicker_HipPitchAddAngle = MAX( kicker_HipPitchAddAngle, -KICKER_HIP_PITCH_ANGLE_MAX);
	kicker_call_counter++;

	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback(Kicker_KickPhaseLeft);
	}
}

/** BalancePhase:
 *  1. stretch feet (Z-axis)
 * 	2. weight stabilizing (Y-axis)
 * 	3. lift shooting foot (left) (Z-axis)
	*/
void Kicker_BalancePhaseLeft()
{
	unsigned char ready = 0;
	if( !Kicker_ApproachStep( &kicker_extremity[LEG_RIGHT].z, kicker_balance_z, 1)) {
		kicker_extremity[LEG_LEFT].z = kicker_extremity[LEG_RIGHT].z;
	}
	else {
		if( kicker_extremity[LEG_LEFT].y > (KICKER_BALANCE_Y_LEFT + kicker_Params.balance_y_l_offset)) {
			kicker_extremity[LEG_LEFT].y -= 1;
			kicker_extremity[LEG_RIGHT].y = kicker_extremity[LEG_LEFT].y;
		}
		else if( kicker_extremity[LEG_LEFT].z > kicker_shoot_z) {
			kicker_extremity[LEG_LEFT].z -= 1;
		}
		else {
			ready = 1;
		}
	}
	inverse_startCycle();
	inverse_calc( LEG_LEFT, &kicker_extremity[LEG_LEFT]);
	inverse_calc( LEG_RIGHT, &kicker_extremity[LEG_RIGHT]);
	//inverse_calc( ARM_RIGHT, &kicker_extremity[ARM_RIGHT]);
	//inverse_calc( ARM_LEFT, &kicker_extremity[ARM_LEFT]);
	Kicker_BalanceRight();
	Kicker_AddOffsetsLeft();
	inverse_endCycle(0);
	kicker_call_counter++;

	if( ready || kicker_call_counter >= KICKER_CALL_COUNTER_LIMIT) {
		kicker_call_counter = 0;
		TC_AssignTimer1Callback(Kicker_SwingBackPhaseLeft);
	}
}

void Kicker_CorrectLegRight( struct vect3d_ext* pos)
{
	if( pos->y < KICKER_RIGHT_FOOT_Y_MIN
			/*&& pos->x + KICKER_OFFSET_HEEL < KICKER_OFFSET_TOE
			&& pos->x + KICKER_OFFSET_TOE > KICKER_OFFSET_HEEL*/) {
		pos->y = KICKER_RIGHT_FOOT_Y_MIN;
	}
}

void Kicker_CorrectLegLeft( struct vect3d_ext* pos)
{
	if( pos->y > KICKER_LEFT_FOOT_Y_MAX
			/*&& pos->x + KICKER_OFFSET_HEEL < KICKER_OFFSET_TOE
			&& pos->x + KICKER_OFFSET_TOE > KICKER_OFFSET_HEEL*/) {
		pos->y = KICKER_LEFT_FOOT_Y_MAX;
	}
}

void Kicker_Begin_Kick(void)
{
	Kicker_SetServoParams();
	kicker_isRunning = 1;
}

/**
 * finalize kicks
 * 1. calculate offsets and fit vectors
 * 2. conversion of step size to the single components (x and y)
 * 3. start with the balance phase */
void Kicker_Finalize_Kick( signed short step)
{
	signed short dx = kicker_shootingFoot_target.x - kicker_shootingFoot_back.x;
	signed short dy = kicker_shootingFoot_target.y - kicker_shootingFoot_back.y;
	signed short dz = kicker_shootingFoot_target.z - kicker_shootingFoot_back.z;

	// subtract the offsets from the vectors
	kicker_shootingFoot_target.x -= KICKER_OFFSET_TOE;
	kicker_shootingFoot_back.x   -= KICKER_OFFSET_TOE;
	kicker_shootingFoot_target.z = kicker_balance_z - kicker_shootingFoot_target.z;
	kicker_shootingFoot_back.z   = kicker_balance_z - kicker_shootingFoot_back.z;
	if( kicker_kickWithRight) {
		kicker_shootingFoot_target.y -= (KICKER_OFFSET_RIGHT - KICKER_BALANCE_Y_RIGHT - kicker_Params.balance_y_r_offset);
		kicker_shootingFoot_back.y   -= (KICKER_OFFSET_RIGHT - KICKER_BALANCE_Y_RIGHT - kicker_Params.balance_y_r_offset);
		Kicker_CorrectLegRight( &kicker_shootingFoot_target);
		Kicker_CorrectLegRight( &kicker_shootingFoot_back);
		inverse_fitVector( LEG_RIGHT, &kicker_shootingFoot_target);
		inverse_fitVector( LEG_RIGHT, &kicker_shootingFoot_back);
	}
	else {
		kicker_shootingFoot_target.y -= (KICKER_OFFSET_LEFT - KICKER_BALANCE_Y_LEFT - kicker_Params.balance_y_l_offset);
		kicker_shootingFoot_back.y   -= (KICKER_OFFSET_LEFT - KICKER_BALANCE_Y_LEFT - kicker_Params.balance_y_l_offset);
		Kicker_CorrectLegLeft( &kicker_shootingFoot_target);
		Kicker_CorrectLegLeft( &kicker_shootingFoot_back);
		inverse_fitVector( LEG_LEFT, &kicker_shootingFoot_target);
		inverse_fitVector( LEG_LEFT, &kicker_shootingFoot_back);
	}

	// conversion of step size to single components (x and y)
	if( ABS(dx) >= ABS(dy)) {
		kicker_shootingFoot_kickStep.x = step;
		if( dy != 0) {
			step = MAX( 1, ABS( (step * dy) / dx));
			if( step + 1 <= kicker_shootingFoot_kickStep.x)
				step++;
			kicker_shootingFoot_kickStep.y = step;
		} else {
			kicker_shootingFoot_kickStep.y = 0;
		}
		if( dz != 0) {
			step = MAX( 1, ABS( (step * dz) / dx));
			if( step + 1 <= kicker_shootingFoot_kickStep.x)
				step++;
			kicker_shootingFoot_kickStep.z = step;
		} else {
			kicker_shootingFoot_kickStep.z = 0;
		}
	} else {
		kicker_shootingFoot_kickStep.y = step;
		if( dx != 0) {
			step = MAX( 1, ABS( (step * dx) / dy));
			if( step + 1 <= kicker_shootingFoot_kickStep.y)
				step++;
			kicker_shootingFoot_kickStep.x = step;
		} else {
			kicker_shootingFoot_kickStep.x = 0;
		}
		if( dz != 0) {
			step = MAX( 1, ABS( (step * dz) / dy));
			if( step + 1 <= kicker_shootingFoot_kickStep.y)
				step++;
			kicker_shootingFoot_kickStep.z = step;
		} else {
			kicker_shootingFoot_kickStep.z = 0;
		}
	}
	kicker_shootingFoot_kickStep.yaw = 0;

	kicker_shootingFoot_fade.x = kicker_shootingFoot_target.x + (dx / 20);
	kicker_shootingFoot_fade.y = kicker_shootingFoot_target.y + (dy / 20);
	kicker_shootingFoot_fade.z = kicker_shootingFoot_target.z + (dz / 20);
	kicker_shootingFoot_fade.yaw = kicker_shootingFoot_target.yaw;

	// start with the balance phase
	TC_SetTimer1Interval( KICKER_TIMER_INTERVAL);
	if( kicker_kickWithRight) {
		TC_AssignTimer1Callback( Kicker_BalancePhaseRight);
	} else {
		TC_AssignTimer1Callback( Kicker_BalancePhaseLeft);
	}
}

/**
 * The simple kick function
 * 1. set ball as target position of shooting foot
 * 2. define swing back position as start position from kick (no swing back)
 * 3. pass to finalize_kick */
unsigned char Kicker_Kick( struct vect3d* ball)
{
	if( kicker_isRunning) {
		return 0;
	}

	Kicker_Begin_Kick();

	kicker_kickWithRight = ball->y > 0;
	kicker_kick_type = KICKER_TYPE_MEDIUM;
	kicker_shoot_range = KICKER_SHOOT_RANGE_SHORT;

	if( kicker_kickWithRight) {
		Kicker_ResetRight();
	} else {
		Kicker_ResetLeft();
	}

	kicker_shoot_z = kicker_balance_z - ball->z;//KICKER_SHOOT_Z;

	// set ball as target position of shooting foot
	kicker_shootingFoot_target.x = ball->x;
	kicker_shootingFoot_target.y = ball->y;
	kicker_shootingFoot_target.z = ball->z;//BALL_RADIUS;
	kicker_shootingFoot_target.yaw = 0;

	kicker_shootingFoot_back.x = KICKER_OFFSET_TOE;
	kicker_shootingFoot_back.z = ball->z;//BALL_RADIUS;
	kicker_shootingFoot_back.yaw = 0;
	kicker_shoot_z = KICKER_SHOOT_Z;

	if( kicker_kickWithRight) {
		kicker_shootingFoot_back.y = KICKER_OFFSET_RIGHT;
	} else {
		kicker_shootingFoot_back.y = KICKER_OFFSET_LEFT;
	}
	kicker_AnklePitchAddAngle = 0.1f;
	kicker_HipPitchAddAngle_Rate = 0.0f;

	Kicker_Finalize_Kick( KICKER_STEP_MEDIUM);
	return 1;
}

/**
 * The forward kick function
 * 1. set ball as target position of shooting foot
 * 2. decision for step size and swing back phase with kicker type
 * 3. pass to finalize_kick */
unsigned char Kicker_KickForward( struct vect3d* ball, /*struct vect3d* param,*/ enum kicker_type type)
{
	if( kicker_isRunning) {
		return 0;
	}

	Kicker_Begin_Kick();

	signed short step = 1;
	signed short backX = 0;

	kicker_kickWithRight = ball->y > 0;
	kicker_kick_type = type;

	if( kicker_kickWithRight) {
		Kicker_ResetRight();
	} else {
		Kicker_ResetLeft();
	}

	if( ball->x > 180) {
		ball->x = 180;
	}

	kicker_shoot_z = kicker_balance_z - ball->z;//KICKER_SHOOT_Z;

	// set ball as target position of shooting foot
	kicker_shootingFoot_target.x = ball->x;
	kicker_shootingFoot_target.y = ball->y;
	kicker_shootingFoot_target.z = ball->z;//BALL_RADIUS + 10;
	kicker_shootingFoot_target.yaw = 0;

	kicker_AnklePitchAddAngle = 0.1f;

	kicker_shootingFoot_back.x = kicker_shootingFoot_target.x;
	kicker_shootingFoot_back.y = kicker_shootingFoot_target.y;
	kicker_shootingFoot_back.z = kicker_shootingFoot_target.z;
	kicker_shootingFoot_back.yaw = 0;

	// decision for step size and swing back phase with kicker type
	switch( type) {
	case KICKER_TYPE_MINIMAL:
		step = KICKER_STEP_MINIMAL;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MINIMAL;
		backX = KICKER_BACK_X_MINIMAL;
		kicker_HipPitchAddAngle_Rate = 0.01f;
		break;
	case KICKER_TYPE_SHORT:
		step = KICKER_STEP_SHORT;
		kicker_shoot_range = KICKER_SHOOT_RANGE_SHORT;
		backX = KICKER_BACK_X_SHORT;
		kicker_HipPitchAddAngle_Rate = 0.02f;
		break;
	case KICKER_TYPE_MEDIUM:
		step = KICKER_STEP_MEDIUM;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MEDIUM;
		backX = KICKER_BACK_X_MEDIUM;
		kicker_HipPitchAddAngle_Rate = 0.03f;
		break;
	case KICKER_TYPE_POWER:
		step = KICKER_STEP_POWER;
		kicker_shoot_range = KICKER_SHOOT_RANGE_POWER;
		backX = KICKER_BACK_X_POWER;
		kicker_HipPitchAddAngle_Rate = 0.035f;
		break;
	case KICKER_TYPE_MAXIMAL:
	default:
		step = KICKER_STEP_MAXIMAL;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MAXIMAL;
		backX = KICKER_BACK_X_MAXIMAL;
		kicker_HipPitchAddAngle_Rate = 0.04f;
		break;
	}

	kicker_shootingFoot_back.x -= backX;

	Kicker_Finalize_Kick( step);
	return 1;
}

/**
 * The backward kick function
 * 1. set ball as target position of shooting foot
 * 2. decision for step size and swing back phase with kicker type
 * 3. pass to finalize_kick */
unsigned char Kicker_KickBackward( struct vect3d* ball, enum kicker_type type)
{
	if( kicker_isRunning) {
		return 0;
	}

	Kicker_Begin_Kick();

	signed short step = 1;
	signed short backX = 0;

	kicker_kickWithRight = ball->y > 0;
	kicker_kick_type = type;

	if( kicker_kickWithRight) {
		Kicker_ResetRight();
	} else {
		Kicker_ResetLeft();
	}

	// set ball as target position of shooting foot
	kicker_shootingFoot_target.x = 2 * LEG_VEC_X_MIN / 3;
	kicker_shootingFoot_target.y = ball->y;
	kicker_shootingFoot_target.z = ball->z;
	kicker_shootingFoot_target.yaw = 0;

	if( kicker_kickWithRight) {
		if( kicker_shootingFoot_target.y < KICKER_RIGHT_FOOT_Y_MIN) {
			kicker_shootingFoot_target.y = KICKER_RIGHT_FOOT_Y_MIN;
		}
	} else {
		if( kicker_shootingFoot_target.y > KICKER_LEFT_FOOT_Y_MAX) {
			kicker_shootingFoot_target.y = KICKER_LEFT_FOOT_Y_MAX;
		}
	}

	// decision for step size and swing back phase with kicker type
	switch( type) {
	case KICKER_TYPE_MINIMAL:
		step = KICKER_STEP_MINIMAL;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MINIMAL;
		backX = KICKER_BACK_X_MINIMAL;
		kicker_AnklePitchAddAngle = 1.2f;
		break;
	case KICKER_TYPE_SHORT:
		step = KICKER_STEP_SHORT;
		kicker_shoot_range = KICKER_SHOOT_RANGE_SHORT;
		backX = KICKER_BACK_X_SHORT;
		kicker_AnklePitchAddAngle = 1.3f;
		break;
	case KICKER_TYPE_MEDIUM:
		step = KICKER_STEP_MEDIUM;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MEDIUM;
		backX = KICKER_BACK_X_MEDIUM;
		kicker_AnklePitchAddAngle = 1.4f;
		break;
	case KICKER_TYPE_POWER:
		step = KICKER_STEP_POWER;
		kicker_shoot_range = KICKER_SHOOT_RANGE_POWER;
		backX = KICKER_BACK_X_POWER;
		kicker_AnklePitchAddAngle = 1.5f;
		break;
	case KICKER_TYPE_MAXIMAL:
	default:
		step = KICKER_STEP_MAXIMAL;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MAXIMAL;
		backX = KICKER_BACK_X_MAXIMAL;
		kicker_AnklePitchAddAngle = 1.6f;
		break;
	}

	if( ball->x > KICKER_OFFSET_HEEL) {
		kicker_shoot_z = kicker_balance_z - (2 * ball->z) - 20;
		kicker_shootingFoot_back.x = ball->x + (3 * BALL_RADIUS);
	} else {
		kicker_shoot_z = kicker_balance_z - KICKER_OFFSET_TOE - 30;
		kicker_shootingFoot_back.x = ball->x + backX;
	}
	kicker_shootingFoot_back.y = kicker_shootingFoot_target.y;
	kicker_shootingFoot_back.z = kicker_shoot_z;
	kicker_shootingFoot_back.yaw = 0;

	Kicker_Finalize_Kick( step);
	return 1;
}

/**
 * The sideward kick function
 * 1. set ball as target position of shooting foot
 * 2. decision for step size and swing back phase with kicker type
 * 3. pass to finalize_kick */
unsigned char Kicker_KickSideward( struct vect3d* ball, enum kicker_type type)
{
	if( kicker_isRunning) {
		return 0;
	}

	Kicker_Begin_Kick();

	signed short step = 1;

	kicker_kickWithRight = ball->y > 0;
	kicker_kick_type = type;

	if( kicker_kickWithRight) {
		Kicker_ResetRight();
	} else {
		Kicker_ResetLeft();
	}

	kicker_shoot_z = kicker_balance_z - ball->z;//KICKER_SHOOT_Z;

	// set ball as target position of shooting foot
	kicker_shootingFoot_target.x = ball->x;
	kicker_shootingFoot_target.y = ball->y;
	kicker_shootingFoot_target.z = ball->z;//BALL_RADIUS;
	kicker_shootingFoot_target.yaw = 0;

	kicker_shootingFoot_back.x = kicker_shootingFoot_target.x;
	kicker_shootingFoot_back.z = kicker_shootingFoot_target.z;
	kicker_shootingFoot_back.yaw = 0;

	if( kicker_kickWithRight) {
		kicker_shootingFoot_back.y = KICKER_OFFSET_RIGHT;
	} else {
		kicker_shootingFoot_back.y = KICKER_OFFSET_LEFT;
	}
	kicker_AnklePitchAddAngle = 0.1f;

	// decision for step size with kicker type
	switch( type) {
	case KICKER_TYPE_MINIMAL:
		step = KICKER_STEP_MINIMAL;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MINIMAL;
		break;
	case KICKER_TYPE_SHORT:
		step = KICKER_STEP_SHORT;
		kicker_shoot_range = KICKER_SHOOT_RANGE_SHORT;
		break;
	case KICKER_TYPE_MEDIUM:
		step = KICKER_STEP_MEDIUM;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MEDIUM;
		break;
	case KICKER_TYPE_POWER:
		step = KICKER_STEP_POWER;
		kicker_shoot_range = KICKER_SHOOT_RANGE_POWER;
		break;
	case KICKER_TYPE_MAXIMAL:
	default:
		step = KICKER_STEP_MAXIMAL;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MAXIMAL;
		break;
	}

	Kicker_Finalize_Kick( step);
	return 1;
}

/**
 * The advanced kick function
 * 1. set ball as target position of shooting foot
 * 2. calculate angle of shooting foot for kick
 * 3. calculate swing back position
 * 4. decision for kicker type and step size
 * 5. pass to finalize_kick */
unsigned char Kicker_KickToTarget( struct vect3d* ball, struct vect3d* target)
{
	if( kicker_isRunning) {
		return 0;
	}

	Kicker_Begin_Kick();

	signed short step = 1;
	struct vect3d shoot;
	float shootLen, angle;

	// set ball as target position of shooting foot
	kicker_shootingFoot_target.x = ball->x;
	kicker_shootingFoot_target.y = ball->y;
	kicker_shootingFoot_target.z = ball->z; //BALL_RADIUS;

	shoot.x = (target->x - ball->x);
	shoot.y = (target->y - ball->y);
	shoot.z = (target->z - ball->z);
	shootLen = sqrt( shoot.x*shoot.x + shoot.y*shoot.y + shoot.z*shoot.z);
	kicker_shoot_range = (signed short)shootLen;

	// calculate angle of shooting foot for kick
	angle = acos( (float)shoot.y / shootLen) * 180.f / M_PI;

	if( shoot.x < 0) {
		angle = 180.f - angle;
	}

	if( angle < 45.f) {
		kicker_shootingFoot_back.yaw = (signed short)(angle);
	} else if( angle < 135.f) {
		angle -= 90.f;
		kicker_shootingFoot_back.yaw = (signed short)(angle);
	} else {
		angle -= 180.f;
		kicker_shootingFoot_back.yaw = (signed short)(angle);
	}
	kicker_shootingFoot_target.yaw = kicker_shootingFoot_back.yaw;

	// calculate swing back position
	shoot.x = (signed short)(kicker_RangeMultiplierLeg * (float)shoot.x / 10.f);
	shoot.y = (signed short)(kicker_RangeMultiplierLeg * (float)shoot.y / 10.f);
	shoot.z = (signed short)(kicker_RangeMultiplierLeg * (float)shoot.z / 10.f);

	kicker_shootingFoot_target.z += shoot.z;

	kicker_shootingFoot_back.x = kicker_shootingFoot_target.x - shoot.x;
	kicker_shootingFoot_back.y = kicker_shootingFoot_target.y - shoot.y;
	kicker_shootingFoot_back.z = kicker_shootingFoot_target.z - shoot.z;

	// add offset to vectors, to center x value (use center of foot plate)
	//kicker_shootingFoot_target.x += KICKER_OFFSET_TOE;
	//kicker_shootingFoot_back.x   += KICKER_OFFSET_TOE;

	float footLineX = 0.f;
	float shootLineSlope = 0.f;

	if( ball->x > KICKER_OFFSET_TOE) {
		footLineX = KICKER_OFFSET_TOE;
	} else if( ball->x < KICKER_OFFSET_HEEL) {
		footLineX = KICKER_OFFSET_HEEL;
	}

	// the shooting line is needed to decide the shooting foot
	// the slope of the shooting line
	if( footLineX != 0.f) {
		shootLineSlope = (float)(shoot.x) / (float)(shoot.y);
	}

	// ensure that the slope is not 0, to prevent dividing by 0
	if( shootLineSlope != 0.f) {
		// the intercept with the foot line (y-axis) -> y-value
		float shootLineIntercept = (float)(ball->y) + ((footLineX - (float)(ball->x)) / shootLineSlope);

		// if the intersection is right (> 0) the right foot is used, else the left foot
		kicker_kickWithRight = (shootLineIntercept > 0.f);
		if( kicker_kickWithRight) {
			if( ball->y < KICKER_RIGHT_FOOT_Y_MIN && ball->x > target->x) {
				kicker_kickWithRight = 0;
			}
		} else {
			if( ball->y > KICKER_LEFT_FOOT_Y_MAX && ball->x > target->x) {
				kicker_kickWithRight = 1;
			}
		}
	} else {
		// alternative shooting foot
		kicker_kickWithRight = ball->y > 0;
	}

	if( kicker_kickWithRight) {
		Kicker_ResetRight();
	} else {
		Kicker_ResetLeft();
	}

	kicker_shoot_z = KICKER_SHOOT_Z;
	kicker_HipPitchAddAngle_Rate = kicker_RangeMultiplierHip * (float)shoot.x * 0.0001f;
	kicker_HipPitchAddAngle_Rate += kicker_RangeMultiplierHip * (float)shoot.z * 0.0001f;

	step = 10;
	kicker_kick_type = KICKER_TYPE_MEDIUM;

	Kicker_Finalize_Kick( step);
	return 1;
}

unsigned char Kicker_KickHigh( struct vect3d* ball, struct vect3d* target, enum kicker_type type)
{
	if( kicker_isRunning) {
		return 0;
	}

	Kicker_Begin_Kick();

	signed short step = 1;
	signed short dx, dy, dz;

	kicker_kickWithRight = ball->y > 0;

	if( kicker_kickWithRight) {
		Kicker_ResetRight();
	} else {
		Kicker_ResetLeft();
	}

	kicker_shoot_z = KICKER_SHOOT_Z;
	kicker_kick_type = type;
	kicker_kickHigh = 1;

	kicker_shootingFoot_back.x = 0;
	kicker_shootingFoot_back.y = ball->y;
	kicker_shootingFoot_back.z = KICKER_OFFSET_Z - BALL_RADIUS;
	kicker_shootingFoot_back.yaw = 0;

	// set ball as target position of shooting foot
	kicker_shootingFoot_target.x = ball->x - KICKER_OFFSET_TOE;
	kicker_shootingFoot_target.y = ball->y;
	kicker_shootingFoot_target.z = KICKER_OFFSET_Z - ball->z;
	kicker_shootingFoot_target.yaw = 0;

	kicker_shootingFoot_fade.x = target->x - KICKER_OFFSET_TOE;
	kicker_shootingFoot_fade.y = target->y;
	kicker_shootingFoot_fade.z = KICKER_OFFSET_Z - target->z;
	kicker_shootingFoot_fade.yaw = 0;

	// subtract the offsets from the vectors
	if( kicker_kickWithRight) {
		kicker_shootingFoot_back.y   -= (KICKER_OFFSET_RIGHT - KICKER_BALANCE_Y_RIGHT - kicker_Params.balance_y_r_offset);
		kicker_shootingFoot_target.y -= (KICKER_OFFSET_RIGHT - KICKER_BALANCE_Y_RIGHT - kicker_Params.balance_y_r_offset);
		kicker_shootingFoot_fade.y	 -= (KICKER_OFFSET_RIGHT - KICKER_BALANCE_Y_RIGHT - kicker_Params.balance_y_r_offset);
		inverse_fitVector( LEG_RIGHT, &kicker_shootingFoot_back);
		inverse_fitVector( LEG_RIGHT, &kicker_shootingFoot_target);
		inverse_fitVector( LEG_RIGHT, &kicker_shootingFoot_fade);
		kicker_AnklePitchAddAngle = 0.1f;
	} else {
		kicker_shootingFoot_back.y   -= (KICKER_OFFSET_LEFT - KICKER_BALANCE_Y_LEFT - kicker_Params.balance_y_l_offset);
		kicker_shootingFoot_target.y -= (KICKER_OFFSET_LEFT - KICKER_BALANCE_Y_LEFT - kicker_Params.balance_y_l_offset);
		kicker_shootingFoot_fade.y   -= (KICKER_OFFSET_LEFT - KICKER_BALANCE_Y_LEFT - kicker_Params.balance_y_l_offset);
		inverse_fitVector( LEG_LEFT, &kicker_shootingFoot_back);
		inverse_fitVector( LEG_LEFT, &kicker_shootingFoot_target);
		inverse_fitVector( LEG_LEFT, &kicker_shootingFoot_fade);
		kicker_AnklePitchAddAngle = -0.1f;
	}

	dx = kicker_shootingFoot_target.x - kicker_shootingFoot_back.x;
	dy = kicker_shootingFoot_target.y - kicker_shootingFoot_back.y;
	dz = kicker_shootingFoot_target.z - kicker_shootingFoot_back.z;

	// decision for step size and swing back phase with kicker type

	switch( type) {
	case KICKER_TYPE_MINIMAL:
		step = 10;
		kicker_HipPitchAddAngle_Rate = 0.1f;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MINIMAL;
		break;
	case KICKER_TYPE_SHORT:
		step = 10;
		kicker_HipPitchAddAngle_Rate = 0.2f;
		kicker_shoot_range = KICKER_SHOOT_RANGE_SHORT;
		break;
	case KICKER_TYPE_MEDIUM:
		step = 10;
		kicker_HipPitchAddAngle_Rate = 0.4f;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MEDIUM;
		break;
	case KICKER_TYPE_POWER:
		step = 10;
		kicker_HipPitchAddAngle_Rate = 0.8f;
		kicker_shoot_range = KICKER_SHOOT_RANGE_POWER;
		break;
	case KICKER_TYPE_MAXIMAL:
	default:
		step = 10;
		kicker_HipPitchAddAngle_Rate = 1.6f;
		kicker_shoot_range = KICKER_SHOOT_RANGE_MAXIMAL;
		break;
	}

	// conversion of step size to single components (x and y)
	if( ABS(dx) >= ABS(dy) && ABS(dx) >= ABS(dz)) {
		kicker_shootingFoot_kickStep.x = step;
		if( dy != 0) {
			step = MAX( 1, ABS( (step * dy) / dx));
			if( step + 1 <= kicker_shootingFoot_kickStep.x)
				step++;
			kicker_shootingFoot_kickStep.y = step;
		} else {
			kicker_shootingFoot_kickStep.y = 0;
		}
		if( dz != 0) {
			step = MAX( 1, ABS( (step * dz) / dx));
			if( step + 1 <= kicker_shootingFoot_kickStep.x)
				step++;
			kicker_shootingFoot_kickStep.z = step;
		} else {
			kicker_shootingFoot_kickStep.z = 0;
		}
	} else if( ABS(dy) >= ABS(dx) && ABS(dy) >= ABS(dz)) {
		kicker_shootingFoot_kickStep.y = step;
		if( dx != 0) {
			step = MAX( 1, ABS( (step * dx) / dy));
			if( step + 1 <= kicker_shootingFoot_kickStep.y)
				step++;
			kicker_shootingFoot_kickStep.x = step;
		} else {
			kicker_shootingFoot_kickStep.x = 0;
		}
		if( dz != 0) {
			step = MAX( 1, ABS( (step * dz) / dy));
			if( step + 1 <= kicker_shootingFoot_kickStep.y)
				step++;
			kicker_shootingFoot_kickStep.z = step;
		} else {
			kicker_shootingFoot_kickStep.z = 0;
		}
	} else {
		kicker_shootingFoot_kickStep.z = step;
		if( dx != 0) {
			step = MAX( 1, ABS( (step * dx) / dz));
			if( step + 1 <= kicker_shootingFoot_kickStep.z)
				step++;
			kicker_shootingFoot_kickStep.x = step;
		} else {
			kicker_shootingFoot_kickStep.x = 0;
		}
		if( dy != 0) {
			step = MAX( 1, ABS( (step * dy) / dz));
			if( step + 1 <= kicker_shootingFoot_kickStep.z)
				step++;
			kicker_shootingFoot_kickStep.y = step;
		} else {
			kicker_shootingFoot_kickStep.y = 0;
		}
	}
	kicker_shootingFoot_kickStep.yaw = 0;

	// start with the balance phase
	TC_SetTimer1Interval( KICKER_TIMER_INTERVAL);
	if( kicker_kickWithRight) {
		TC_AssignTimer1Callback( Kicker_BalancePhaseRight);
	} else {
		TC_AssignTimer1Callback( Kicker_BalancePhaseLeft);
	}

	return 1;

}

void Kicker_CancelKick(void)
{
	TC_SetTimer1Interval( KICKER_TIMER_INTERVAL);
	if( kicker_kickWithRight) {
		TC_AssignTimer1Callback( Kicker_ResetPhaseRight);
	} else {
		TC_AssignTimer1Callback( Kicker_ResetPhaseLeft);
	}
}
