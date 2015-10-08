/*
 * inverse.c
 *
 *  Created on: 26.04.2011
 *      Author: Stefan
 */

#include "inverse.h"
#include "servo/servo.h"
#include "dbgu/dbgu.h"
#include "config/config.h"
#include <math.h>

#define INVERSE_SERVO_NOT_IN_USE 	-1000.f

float inverse_servoOffsets[SERVO_COUNT_MAX] = INVERSE_SERVO_OFFSETS;
float inverse_servoAngles[SERVO_COUNT_MAX]; // angles in degrees
float inverse_hipAngle = 0.f;

struct vect3d_ext inverse_currentPosition[4]; // 4 extremities

void inverse_startCycle(void) {
	for (int i = 0; i < SERVO_COUNT_MAX; i++) {
		inverse_servoAngles[i] = INVERSE_SERVO_NOT_IN_USE;
	}
}

void inverse_setServoAngle(unsigned char id, float angle) {
#ifdef DYNAMIXEL
	id--;
#endif
	inverse_servoAngles[id] = angle;
}

void inverse_addServoAngle(unsigned char id, float angle) {
#ifdef DYNAMIXEL
	id--;
#endif
	if( inverse_servoAngles[id] != INVERSE_SERVO_NOT_IN_USE) {
		inverse_servoAngles[id] += angle;
	}
}

void inverse_calc(enum body_extremity ext, struct vect3d_ext * target) {
	float angles[INVERSE_SERVOS_MAX];

	// Exit when 0-vector is given
	if (target->x == 0 && target->y == 0 && target->z == 0) {
		return;
	}

	inverse_calculateAngles(ext, target, &angles[0]);
	//dbgu_printf("[inverse] calc: servoAngles[]=%3.5f, %3.5f, %3.5f, %3.5f, %3.5f, %3.5f\r\n", angles[0], angles[1], angles[2], angles[3], angles[4], angles[5]);

	inverse_currentPosition[ext].x = target->x;
	inverse_currentPosition[ext].y = target->y;
	inverse_currentPosition[ext].z = target->z;
	inverse_currentPosition[ext].yaw = target->yaw;

	float hip = inverse_hipAngle * (M_PI / 180.0f);
	if (ext == ARM_LEFT) {
		inverse_setServoAngle( SERVO_ID_L_SHOULDER_PITCH, angles[0]);
		inverse_setServoAngle( SERVO_ID_L_SHOULDER_ROLL,  angles[1]);
		inverse_setServoAngle( SERVO_ID_L_ELBOW,          angles[2]);
	} else if (ext == ARM_RIGHT) {
		inverse_setServoAngle( SERVO_ID_R_SHOULDER_PITCH, angles[0]);
		inverse_setServoAngle( SERVO_ID_R_SHOULDER_ROLL,  angles[1]);
		inverse_setServoAngle( SERVO_ID_R_ELBOW,          angles[2]);
	} else if (ext == LEG_LEFT) {
		inverse_setServoAngle( SERVO_ID_L_HIP_YAW,     angles[0]);
		inverse_setServoAngle( SERVO_ID_L_HIP_ROLL,    angles[1]);
		inverse_setServoAngle( SERVO_ID_L_HIP_PITCH,   angles[2] - hip);
		inverse_setServoAngle( SERVO_ID_L_KNEE,        angles[3]);
		inverse_setServoAngle( SERVO_ID_L_ANKLE_PITCH, angles[4]);
		inverse_setServoAngle( SERVO_ID_L_ANKLE_ROLL,  angles[5]);
	} else if (ext == LEG_RIGHT) {
		inverse_setServoAngle( SERVO_ID_R_HIP_YAW,     angles[0]);
		inverse_setServoAngle( SERVO_ID_R_HIP_ROLL,    angles[1]);
		inverse_setServoAngle( SERVO_ID_R_HIP_PITCH,   angles[2] + hip);
		inverse_setServoAngle( SERVO_ID_R_KNEE,        angles[3]);
		inverse_setServoAngle( SERVO_ID_R_ANKLE_PITCH, angles[4]);
		inverse_setServoAngle( SERVO_ID_R_ANKLE_ROLL,  angles[5]);
	}
	//dbgu_printf("[inverse] calc: servoPositions[]=%d, %d, %d, %d, %d, %d\r\n", inverse_servoPositions[offset + 0], inverse_servoPositions[offset + 1], inverse_servoPositions[offset + 2], inverse_servoPositions[offset + 3], inverse_servoPositions[offset + 4], inverse_servoPositions[offset + 5]);
}

/*float inverse_calcServoPos(unsigned char id, double angle) {
	return (((angle * (180.0 / M_PI)) + inverse_servoOffsets[id]));
}*/

void inverse_endCycle(unsigned char doNotSend) {
	unsigned char servocnt = Config_GetServoCount();
	unsigned char i;
	for( i = 0; i < servocnt; i++) {
		if (inverse_servoAngles[i] != INVERSE_SERVO_NOT_IN_USE) {
			float pos = (inverse_servoAngles[i] * (180.f / M_PI));
			pos += inverse_servoOffsets[i];
#ifdef DYNAMIXEL
			Servo_SetPosition(i+1, pos);
#else
			Servo_SetPosition(i, pos);
#endif
		}
	}

	Servo_SetPositionsTorque(0);
	if (!doNotSend) {
		Servo_SendPositions();
	}
}

struct vect3d_ext inverse_getCurrentPosExt(enum body_extremity ext) {
	return inverse_currentPosition[ext];
}

struct vect3d inverse_getCurrentPos(enum body_extremity ext) {
	struct vect3d vec;
	vec.x = inverse_currentPosition[ext].x;
	vec.y = inverse_currentPosition[ext].y;
	vec.z = inverse_currentPosition[ext].z;
	return vec;
}

signed short inverse_getCurrentPosX(enum body_extremity ext) {
	return inverse_currentPosition[ext].x;
}

signed short inverse_getCurrentPosY(enum body_extremity ext) {
	return inverse_currentPosition[ext].y;
}

signed short inverse_getCurrentPosZ(enum body_extremity ext) {
	return inverse_currentPosition[ext].z;
}

signed short inverse_getCurrentPosYaw(enum body_extremity ext) {
	return inverse_currentPosition[ext].yaw;
}

void inverse_setHipAngle(float angle) {
	inverse_hipAngle = angle;
}

float inverse_getHipAngle(void) {
	return inverse_hipAngle;
}
