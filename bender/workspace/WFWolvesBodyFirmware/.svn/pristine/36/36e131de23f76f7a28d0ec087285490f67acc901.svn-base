/*
 * balance.c
 *
 *  Created on: 22.04.2013
 *      Author: Oliver
 */
#include "balance.h"
#include "servo/servo.h"
#include "kinematics/inverse.h"
#include "imu/imu.h"
#include "walker/walker.h"

//extern struct Walker_Parameters Walker_Params;
extern signed char dir[14];

void balanced_gyro_left() {
	// adjust balance offset
#define GYRO_TO_SERVO_RATIO	16.f
	float rlGyroErr = (float)IMU_GetRLGyro() / GYRO_TO_SERVO_RATIO;
	float fbGyroErr = (float)IMU_GetFBGyro() / GYRO_TO_SERVO_RATIO;

	inverse_addServoAngle( SERVO_ID_L_HIP_ROLL,    (int)(+dir[7]  * rlGyroErr * Walker_Params.BALANCE_HIP_ROLL_GAIN));
	inverse_addServoAngle( SERVO_ID_L_KNEE,        (int)(-dir[9]  * fbGyroErr * Walker_Params.BALANCE_KNEE_GAIN));
	inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, (int)(-dir[10] * fbGyroErr * Walker_Params.BALANCE_ANKLE_PITCH_GAIN));
	inverse_addServoAngle( SERVO_ID_L_ANKLE_ROLL,  (int)(-dir[11] * rlGyroErr * Walker_Params.BALANCE_ANKLE_ROLL_GAIN));
}

void balanced_gyro_right(void) {
	// adjust balance offset
#define GYRO_TO_SERVO_RATIO	16.f
	float rlGyroErr = (float)IMU_GetRLGyro() / GYRO_TO_SERVO_RATIO;
	float fbGyroErr = (float)IMU_GetFBGyro() / GYRO_TO_SERVO_RATIO;

	inverse_addServoAngle( SERVO_ID_R_HIP_ROLL,    (int)(+dir[1] * rlGyroErr * Walker_Params.BALANCE_HIP_ROLL_GAIN));
	inverse_addServoAngle( SERVO_ID_R_KNEE,        (int)(-dir[3] * fbGyroErr * Walker_Params.BALANCE_KNEE_GAIN));
	inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, (int)(-dir[4] * fbGyroErr * Walker_Params.BALANCE_ANKLE_PITCH_GAIN));
	inverse_addServoAngle( SERVO_ID_R_ANKLE_ROLL,  (int)(-dir[5] * rlGyroErr * Walker_Params.BALANCE_ANKLE_ROLL_GAIN));
}

/* http://www.rn-wissen.de/index.php/Regelungstechnik#PID-Regler
	 *  e = w - x;									// Vergleich (e: Abweichung, w: Soll, x: Ist)
	 *  esum = esum + e;							// Integration I-Anteil
	 * 	y = Kp*e + Ki*Ta*esum + Kd/Ta*(e – ealt);	// Reglergleichung
	 * 	ealt = e;
	 */
int balance_rl( float p, float i, float d, float time) {
	static int errorSum = 0;
	static int errorOld = 0;
	float angle;
	int error = IMU_GetRLAccelAverageC();
	if( errorSum + error > -1000 && errorSum + error < 1000) {
		errorSum += error;
	}
	angle = p * (float)error + i * time * (float)errorSum + (d / time) * (float)(error - errorOld);
	angle /= 100.f;
	if( angle > 0.f) {
		inverse_addServoAngle( SERVO_ID_L_SHOULDER_ROLL, 2.f * angle);
	} else {
		inverse_addServoAngle( SERVO_ID_R_SHOULDER_ROLL, 2.f * angle);
	}
	errorOld = error;
	return errorSum;
}

int balance_left_rl( float p, float i, float d, float time) {
	static int errorSum = 0;
	static int errorOld = 0;
	float angle;
	int error = IMU_GetRLAccelAverageC();
	if( errorSum + error > -1000 && errorSum + error < 1000) {
		errorSum += error;
	}
	angle = p * (float)error + i * time * (float)errorSum + (d / time) * (float)(error - errorOld);
	angle /= 100.f;
	//inverse_addServoAngle( SERVO_ID_L_HIP_ROLL, angle); // angle / 2.f
	inverse_addServoAngle( SERVO_ID_L_ANKLE_ROLL, -angle / 2.f);
	if( angle > 0.f) {
		inverse_addServoAngle( SERVO_ID_L_SHOULDER_ROLL, 2.f * angle);
	} else {
		inverse_addServoAngle( SERVO_ID_R_SHOULDER_ROLL, 2.f * angle);
	}
	errorOld = error;
	return errorSum;
}

int balance_left_fb( float p, float i, float d, float time) {
	static int errorSum = 0;
	static int errorOld = 0;
	float angle;
	int error = IMU_GetFBAccelAverageC();
	if( errorSum + error > -1000 && errorSum + error < 1000) {
		errorSum += error;
	}
	angle = p * (float)error + i * time * (float)errorSum + (d / time) * (float)(error - errorOld);
	angle /= 200.f;
	inverse_addServoAngle( SERVO_ID_L_KNEE, -angle);
	inverse_addServoAngle( SERVO_ID_L_ANKLE_PITCH, angle);
	errorOld = error;
	return errorSum;
}

int balance_right_rl( float p, float i, float d, float time) {
	static int errorSum = 0;
	static int errorOld = 0;
	float angle;
	int error = IMU_GetRLAccelAverageC();
	if( errorSum + error > -1000 && errorSum + error < 1000) {
		errorSum += error;
	}
	angle = p * (float)error + i * time * (float)errorSum + (d / time) * (float)(error - errorOld);
	angle /= 100.f;
	//inverse_addServoAngle( SERVO_ID_R_HIP_ROLL, angle); // angle / 2.f
	inverse_addServoAngle( SERVO_ID_R_ANKLE_ROLL, -angle / 2.f);
	if( angle > 0.f) {
		inverse_addServoAngle( SERVO_ID_L_SHOULDER_ROLL, 2.f * angle);
	} else {
		inverse_addServoAngle( SERVO_ID_R_SHOULDER_ROLL, 2.f * angle);
	}
	errorOld = error;
	return errorSum;
}

int balance_right_fb( float p, float i, float d, float time) {
	static int errorSum = 0;
	static int errorOld = 0;
	float angle;
	int error = IMU_GetFBAccelAverageC();
	if( errorSum + error > -1000 && errorSum + error < 1000) {
		errorSum += error;
	}
	angle = p * (float)error + i * time * (float)errorSum + (d / time) * (float)(error - errorOld);
	angle /= 200.f;
	inverse_addServoAngle( SERVO_ID_R_KNEE, angle);
	inverse_addServoAngle( SERVO_ID_R_ANKLE_PITCH, -angle);
	errorOld = error;
	return errorSum;
}
