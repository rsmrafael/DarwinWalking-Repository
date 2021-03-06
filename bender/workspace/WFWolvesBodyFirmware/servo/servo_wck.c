/*
 * servo.c
 *
 *  Created on: 15.03.2010
 *      Author: Stefan
 */

#ifndef DYNAMIXEL

#include "servo.h"
#include "uart/uart.h"
#include "tc/tc.h"
#include "dbgu/dbgu.h"
#include "config/config.h"
#include <math.h>

#define HEADER				0xFF

#define SERVO_RECV_TIMEOUT		100

#define SERVO_RATIO_VALUE2ANGLE 		1.055 // 269� / 255
#define SERVO_RATIO_ANGLE2VALUE			0.9479554 // 255 / 269�

char servo_positionsTorque = 2;

int Servo_Angle2Value(float angle);
float Servo_Value2Angle(int value);

void Servo_Init_WCK(void) {
	servo_positionsTorque = 2;
}

unsigned char Servo_ReadByte(void) {
	unsigned long startTick = TC_GetSystemTicks();
	while (!uart1_char_available()) {
		if (TC_GetMsSinceTick(startTick) > SERVO_RECV_TIMEOUT) {
			startTick = 0;
			break;
		}
	}

	if (startTick > 0) {
		return uart1_getc();
	}
	return 255;
}

void Servo_SetPositionsTorque(unsigned char torque) {
	if (torque <= 4) {
		servo_positionsTorque = torque;
	}
}

void Servo_SendPositions(void) {
	char CheckSum;
	signed short tmp;
	unsigned char Data;

	Servo_SpinLock();
	Data = (servo_positionsTorque << 5) | 31;
	uart1_hintStartOfTransfer();
	uart1_putc(HEADER);
	uart1_putc(Data);
	uart1_putc(SERVO_COUNT_MAX);  // This is the (last ID - 1) why is it hardcoded ?
	CheckSum = 0;

	for(int i = 0; i < SERVO_COUNT_MAX; i++) {			// for all wCK
		tmp = Servo_Angle2Value(Servo_GetPosition(i));
		tmp += Servo_GetOffset(i);
		if (tmp > 254) tmp = 254;
		if (tmp < 0) tmp = 0;
		uart1_putc((unsigned char)tmp);					// put into transmit buffer
		CheckSum = CheckSum^(unsigned char)tmp;
	}
	CheckSum = CheckSum & 0x7f;
	uart1_putc(CheckSum);
	uart1_hintEndOfTransfer();
	Servo_SpinUnLock();
}

unsigned char Servo_SendPosition(unsigned char id, float pos) {
	char CheckSum;
	char Data = (servo_positionsTorque << 5) | (id & 0x1F);

	signed short tmp = Servo_Angle2Value(pos);
	if (id < SERVO_COUNT_MAX) {
		tmp += Servo_GetOffset(id);
		if (tmp > 254) tmp = 254;
		if (tmp < 0) tmp = 0;
	} else {
		dbgu_printf("[Servo] WARNING: Trying to access servo %d which is outside of calibrated offsets!\r\n", id);
	}

	Servo_SpinLock();
	uart1_hintStartOfTransfer();
	uart1_putc(HEADER);
	uart1_putc(Data);
	uart1_putc((unsigned char)tmp);
	CheckSum = (Data ^ (unsigned char)tmp) & 0x7f;
	uart1_putc(CheckSum);
	uart1_hintEndOfTransfer();
	Servo_SpinUnLock();

	return 1;
}

//------------------------------------------------------------------------------
// Send a set command to wCK
// Input	: ID, Data1, Data2, Data3
// Output	: None
//------------------------------------------------------------------------------
void Servo_SendSetCmd(unsigned char id, char Data1, char Data2, char Data3) {
	char CheckSum;
	id = (7 << 5) | (id & 0x1F);
	CheckSum = (id ^ Data1 ^ Data2 ^ Data3) & 0x7f;

	Servo_SpinLock();
	uart1_hintStartOfTransfer();
	uart1_putc(HEADER);
	uart1_putc(id);
	uart1_putc(Data1);
	uart1_putc(Data2);
	uart1_putc(Data3);
	uart1_putc(CheckSum);
	uart1_hintEndOfTransfer();
	Servo_SpinUnLock();
}

void Servo_SendTGain(unsigned char id, char pGain, char iGain, char dGain) {
	Servo_SendSetCmd(id, 11, pGain, dGain);
	Servo_SendSetCmd(id, 24, iGain, iGain);
}

void Servo_SendTGainGlobal(char pGain, char iGain, char dGain) {
	for(int i = 0; i < SERVO_COUNT_MAX; i++) {			// for all wCK
		Servo_SendSetCmd(i, 11, pGain, dGain);
		Servo_SendSetCmd(i, 24, iGain, iGain);
	}
}

void Servo_SendExPortD(unsigned char id, char exPortD) {
	Servo_SendSetCmd(id, 100, exPortD, exPortD);
}

unsigned char Servo_SetPassive(unsigned char id, unsigned char state) {
	char Data1, Data2;
	char CheckSum;

	if (state != 0) return 0;

	Data1 = (6 << 5) | (id & 0x1F);
	Data2 = 0x10;
	CheckSum = (Data1 ^ Data2) & 0x7f;

	Servo_SpinLock();
	uart1_hintStartOfTransfer();
	uart1_putc(HEADER);
	uart1_putc(Data1);
	uart1_putc(Data2);
	uart1_putc(CheckSum);
	uart1_hintEndOfTransfer();
	Servo_SpinUnLock();
	return 1;
}

unsigned char Servo_GetPassive(unsigned char id) {
	return 0;
}

void Servo_SetID(unsigned char oldid, unsigned char newid) {
	char Data1, Data2, Data3, Data4;
	char CheckSum;

	Data1 = (7 << 5) | (oldid & 0x1F);
	Data2 = 0x0C;
	Data3 = (newid & 0x1F);
	Data4 = Data3;
	CheckSum = (Data1 ^ Data2 ^ Data3 ^ Data4) & 0x7f;

	Servo_SpinLock();
	uart1_hintStartOfTransfer();
	uart1_putc(HEADER);
	uart1_putc(Data1);
	uart1_putc(Data2);
	uart1_putc(Data3);
	uart1_putc(Data4);
	uart1_putc(CheckSum);
	uart1_hintEndOfTransfer();
	Servo_SpinUnLock();
}

void Servo_SetBaudrate(unsigned char id, unsigned int baud) {
}

void Servo_SetBoundaries(unsigned char id, unsigned short lower, unsigned short upper) {
	char Data1, Data2, Data3, Data4;
	char CheckSum;

	Data1 = (7 << 5) | (id & 0x1F);
	Data2 = 0x11;
	Data3 = (lower & 0xFF);
	Data4 = (upper & 0xFF);
	CheckSum = (Data1 ^ Data2 ^ Data3 ^ Data4) & 0x7f;

	Servo_SpinLock();
	uart1_putc(HEADER);
	uart1_putc(Data1);
	uart1_putc(Data2);
	uart1_putc(Data3);
	uart1_putc(Data4);
	uart1_putc(CheckSum);
	Servo_SpinUnLock();
}

void Servo_SetOverload(unsigned char id, unsigned char overload) {
	char Data1, Data2, Data3, Data4;
	char CheckSum;

	Data1 = (7 << 5) | (id & 0x1F);
	Data2 = 0x0F;
	Data3 = (overload & 0xFF);
	Data4 = Data3;
	CheckSum = (Data1 ^ Data2 ^ Data3 ^ Data4) & 0x7f;

	Servo_SpinLock();
	uart1_putc(HEADER);
	uart1_putc(Data1);
	uart1_putc(Data2);
	uart1_putc(Data3);
	uart1_putc(Data4);
	uart1_putc(CheckSum);
	Servo_SpinUnLock();
}

//------------------------------------------------------------------------------
// Read the current position
// Input	: ID
// Output	: Current Position (0~254)
//------------------------------------------------------------------------------
unsigned char Servo_ReadPosRaw(unsigned char id) {
	char	Data1, Data2;
	char	CheckSum;

	Servo_SpinLock();

	// Clear receive buffer
	while (uart1_char_available()) {
		uart1_getc();
	}

	Data1 = (5 << 5) | (id & 0x1F);
	Data2 = 0;
	CheckSum = (Data1 ^ Data2) & 0x7f;
	uart1_hintStartOfTransfer();
	uart1_putc(HEADER);
	uart1_putc(Data1);
	uart1_putc(Data2);
	uart1_putc(CheckSum);
	uart1_hintEndOfTransfer();

	unsigned char pos = Servo_ReadByte(); // Servo load
	if (pos != 255) {
		pos = uart1_getc(); // Servo position
	}

	Servo_SpinUnLock();
	return pos;
}

float Servo_ReadPos(unsigned char id) {
	unsigned char pos = Servo_ReadPosRaw(id);
	if (pos != 255) {
		return Servo_Value2Angle(pos);
	} else {
		return -1000;
	}
}

//------------------------------------------------------------------------------
// Read the current load
// Input	: ID
// Output	: Current load (0~254)
//------------------------------------------------------------------------------
unsigned char Servo_ReadLoad(unsigned char id) {
	char	Data1, Data2;
	char	CheckSum;

	Servo_SpinLock();

	// Clear receive buffer
	while (uart1_char_available()) {
		uart1_getc();
	}

	Data1 = (5 << 5) | (id & 0x1F);
	Data2 = 0;
	CheckSum = (Data1 ^ Data2) & 0x7f;
	uart1_hintStartOfTransfer();
	uart1_putc(HEADER);
	uart1_putc(Data1);
	uart1_putc(Data2);
	uart1_putc(CheckSum);
	uart1_hintEndOfTransfer();

	unsigned char pos = Servo_ReadByte(); // Servo load
	if (pos != 255) {
		uart1_getc(); // Servo position
	}

	Servo_SpinUnLock();
	return pos;
}

unsigned char Servo_ReadOverload(unsigned char id) {
	char	Data1, Data2, Data3, Data4;
	char	CheckSum;

	Servo_SpinLock();

	// Clear receive buffer
	while (uart1_char_available()) {
		uart1_getc();
	}

	Data1 = (7 << 5) | (id & 0x1F);
	Data2 = 0x10;
	Data3 = 0;
	Data4 = Data3;
	CheckSum = (Data1 ^ Data2 ^ Data3 ^ Data4) & 0x7f;
	uart1_hintStartOfTransfer();
	uart1_putc(HEADER);
	uart1_putc(Data1);
	uart1_putc(Data2);
	uart1_putc(Data3);
	uart1_putc(Data4);
	uart1_putc(CheckSum);
	uart1_hintEndOfTransfer();

	unsigned char overload = Servo_ReadByte();
	if (overload != 255) {
		uart1_getc();
	}

	Servo_SpinUnLock();
	return overload;
}

void Servo_ReadBounds(unsigned char id, unsigned short* bound_l, unsigned short* bound_u) {
	char	Data1, Data2, Data3, Data4;
	char	CheckSum;

	Servo_SpinLock();

	// Clear receive buffer
	while (uart1_char_available()) {
		uart1_getc();
	}

	Data1 = (7 << 5) | (id & 0x1F);
	Data2 = 0x12;
	Data3 = 0;
	Data4 = Data3;
	CheckSum = (Data1 ^ Data2 ^ Data3 ^ Data4) & 0x7f;
	uart1_hintStartOfTransfer();
	uart1_putc(HEADER);
	uart1_putc(Data1);
	uart1_putc(Data2);
	uart1_putc(Data3);
	uart1_putc(Data4);
	uart1_putc(CheckSum);
	uart1_hintEndOfTransfer();

	unsigned char lower = Servo_ReadByte();
	unsigned char upper = 255;
	if (lower != 255) {
		upper = uart1_getc();
	}
	*bound_l = lower;
	*bound_u = upper;

	Servo_SpinUnLock();
}

void Servo_ReadPID(unsigned char id, unsigned char* p, unsigned char* i, unsigned char* d) {
	char	Data1, Data2, Data3, Data4;
	char	CheckSum;

	Servo_SpinLock();

	// Clear receive buffer
	while (uart1_char_available()) {
		uart1_getc();
	}

	Data1 = (7 << 5) | (id & 0x1F);
	Data2 = 0x0A;
	Data3 = 0;
	Data4 = Data3;
	CheckSum = (Data1 ^ Data2 ^ Data3 ^ Data4) & 0x7f;
	uart1_hintStartOfTransfer();
	uart1_putc(HEADER);
	uart1_putc(Data1);
	uart1_putc(Data2);
	uart1_putc(Data3);
	uart1_putc(Data4);
	uart1_putc(CheckSum);
	uart1_hintEndOfTransfer();

	unsigned char p_gain = Servo_ReadByte();
	unsigned char d_gain = 255;
	unsigned char i_gain = 255;
	if (p_gain != 255) {
		d_gain = uart1_getc();
		if (d_gain != 255) {
			Data1 = (7 << 5) | (id & 0x1F);
			Data2 = 0x16;
			Data3 = 0;
			Data4 = Data3;
			CheckSum = (Data1 ^ Data2 ^ Data3 ^ Data4) & 0x7f;
			uart1_hintStartOfTransfer();
			uart1_putc(HEADER);
			uart1_putc(Data1);
			uart1_putc(Data2);
			uart1_putc(Data3);
			uart1_putc(Data4);
			uart1_putc(CheckSum);
			uart1_hintEndOfTransfer();

			i_gain = uart1_getc();
			if (i_gain != 255) {
				uart1_getc(); // Repetition of i gain
			}
		}
	}
	*p = p_gain;
	*d = d_gain;
	*i = i_gain;

	Servo_SpinUnLock();
}

unsigned char Servo_IsResponding(unsigned char id) {
	return Servo_ReadPosRaw(id) != 255 ? 1 : 0;
}

int Servo_Angle2Value(float angle) {
	return round(angle * SERVO_RATIO_ANGLE2VALUE) + SERVO_CENTER_VALUE;
}

float Servo_Value2Angle(int value) {
	return (value - SERVO_CENTER_VALUE) * SERVO_RATIO_VALUE2ANGLE;
}

void Servo_SetPowerState(unsigned char state) {}
unsigned char Servo_ReadVoltage(unsigned char id) { return 0; }
unsigned char Servo_ReadTemperature(unsigned char id) { return 0; }
void Servo_SetType(unsigned char id, unsigned char type) {}

void Servo_UpdateStatus(struct servoData_t servo_status[]) {}
void Servo_ResetMaxError(void) {}

void Servo_SendData(unsigned char* data, int length) {
	Servo_SpinLock();
	uart1_hintStartOfTransfer();

	for (int i = 0; i < length; i++) {
		uart1_putc(data[i]);
	}
	uart1_waitforsent();

	uart1_hintEndOfTransfer();
	Servo_SpinUnLock();
}

#endif
