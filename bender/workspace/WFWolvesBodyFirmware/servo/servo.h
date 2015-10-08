/*
 * servo.h
 *
 *  Created on: 15.03.2010
 *      Author: Stefan
 */

#ifndef SERVO_H_
#define SERVO_H_

#define SERVO_COUNT_MAX					21

#ifdef DYNAMIXEL
#define SERVO_ID_R_SHOULDER_PITCH		1
#define SERVO_ID_L_SHOULDER_PITCH		2
#define SERVO_ID_R_SHOULDER_ROLL		3
#define SERVO_ID_L_SHOULDER_ROLL		4
#define SERVO_ID_R_ELBOW				5
#define SERVO_ID_L_ELBOW				6
#define SERVO_ID_R_HIP_YAW				7
#define SERVO_ID_L_HIP_YAW				8
#define SERVO_ID_R_HIP_ROLL				9
#define SERVO_ID_L_HIP_ROLL				10
#define SERVO_ID_R_HIP_PITCH			11
#define SERVO_ID_L_HIP_PITCH			12
#define SERVO_ID_R_KNEE					13
#define SERVO_ID_L_KNEE					14
#define SERVO_ID_R_ANKLE_PITCH			15
#define SERVO_ID_L_ANKLE_PITCH			16
#define SERVO_ID_R_ANKLE_ROLL			17
#define SERVO_ID_L_ANKLE_ROLL			18
#define SERVO_ID_HEAD_PAN				19
#define SERVO_ID_HEAD_TILT				20
#else
#define SERVO_ID_L_HIP_YAW             0
#define SERVO_ID_L_HIP_ROLL            1
#define SERVO_ID_L_HIP_PITCH           2
#define SERVO_ID_L_KNEE                3
#define SERVO_ID_L_ANKLE_PITCH         4
#define SERVO_ID_L_ANKLE_ROLL          5
#define SERVO_ID_R_HIP_YAW             6
#define SERVO_ID_R_HIP_ROLL            7
#define SERVO_ID_R_HIP_PITCH           8
#define SERVO_ID_R_KNEE                9
#define SERVO_ID_R_ANKLE_PITCH         10
#define SERVO_ID_R_ANKLE_ROLL          11
#define SERVO_ID_L_SHOULDER_PITCH      12
#define SERVO_ID_L_SHOULDER_ROLL       13
#define SERVO_ID_L_ELBOW               14
#define SERVO_ID_R_SHOULDER_PITCH      15
#define SERVO_ID_R_SHOULDER_ROLL       16
#define SERVO_ID_R_ELBOW               17
#define SERVO_ID_HEAD_PAN              18
#define SERVO_ID_HEAD_TILT             19
#endif

struct servoData_t {
	unsigned char maxPositionError;
	unsigned char temperature;
	unsigned char voltage;
} __attribute__((packed));

void Servo_Init(void);
void Servo_SetPosition(unsigned char id, float pos);
float Servo_GetPosition(unsigned char id);
void Servo_SetOffsetEnable(unsigned int state);
signed char Servo_GetOffset(unsigned char id);
void Servo_SetOffset(unsigned char id, signed char offset);
void Servo_ListOffsets(void);

void Servo_SpinLock(void);
void Servo_SpinUnLock(void);

#ifdef DYNAMIXEL
#include "servo_dynamixel.h"
#else
#include "servo_wck.h"
#endif

#endif /* SERVO_H_ */
