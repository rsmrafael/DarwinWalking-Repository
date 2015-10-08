/*
 * servo.c
 *
 *  Created on: 15.03.2010
 *      Author: Stefan
 */


#include "servo.h"
#include "tc/tc.h"
#include "dbgu/dbgu.h"
#include "config/config.h"

float servo_positions[SERVO_COUNT_MAX];
signed char servo_offset[SERVO_COUNT_MAX];
unsigned char servo_disableOffsets = 0;
char servo_spinlock = 0;

void Servo_Init(void) {
	for (int i = 0; i < SERVO_COUNT_MAX; i++) {
		servo_positions[i] = 0;
	}
	// Calculate offsets from zero position
	for (int i = 0; i < SERVO_COUNT_MAX; i++) {
		if (Config_UseServoOffsets() == 1 && i < Config_GetServoCount()) {
			servo_offset[i] = Config_GetServoOffset(i);
		} else {
			servo_offset[i] = 0;
		}
	}
	Servo_ListOffsets();
#ifdef DYNAMIXEL
	Servo_Init_DYN();
#else
	Servo_Init_WCK();
#endif
	Servo_SpinLock();
	TC_DelayMs(500);			// Give the servos some time to initialize
	Servo_SpinUnLock();
}

void Servo_SetPosition(unsigned char id, float pos) {
	if (id < SERVO_COUNT_MAX) {
		servo_positions[id] = pos;
	}
}

float Servo_GetPosition(unsigned char id) {
	return servo_positions[id];
}

void Servo_SpinLock(void) {
	while (servo_spinlock == 1);
	TC_SetInterruptState(0);
	servo_spinlock = 1;
}

void Servo_SpinUnLock(void) {
	servo_spinlock = 0;
	TC_SetInterruptState(1);
}

void Servo_SetOffsetEnable(unsigned int state) {
	servo_disableOffsets = 1 - state;
}

signed char Servo_GetOffset(unsigned char id) {
	if (servo_disableOffsets) {
		return 0;
	} else {
		return servo_offset[id];
	}
}

void Servo_SetOffset(unsigned char id, signed char offset) {
	servo_offset[id] = offset;
	Config_SetServoOffset(id, offset);
}

void Servo_ListOffsets(void) {
	dbgu_printf("[Servo] Offsets:\r\n");
	for (int i = 0; i < SERVO_COUNT_MAX; i++) {
		dbgu_printf(" %2d: %2d;", i, servo_offset[i]);
		if ((i + 1) % 10 == 0) {
			dbgu_printf("\r\n");
		}
	}
	dbgu_printf("\r\n");
}
