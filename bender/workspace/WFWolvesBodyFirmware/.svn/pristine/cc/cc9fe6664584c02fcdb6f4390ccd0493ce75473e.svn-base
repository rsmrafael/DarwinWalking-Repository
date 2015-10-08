/*
 * servo_dynamixel.h
 *
 *  Created on: 16.08.2010
 *      Author: Stefan
 */

#ifndef SERVO_DYNAMIXEL_H_
#define SERVO_DYNAMIXEL_H_

#ifdef DYNAMIXEL

#define SERVO_MIN_VALUE					0
#define SERVO_CENTER_VALUE				512
#define SERVO_CENTER_VALUE_MX			2048
#define SERVO_MAX_VALUE					1023
#define SERVO_MAX_VALUE_MX				4096
#define SERVO_MIN_ANGLE					-150.0 // degree
#define SERVO_MAX_ANGLE					150.0 // degree

void Servo_Init_DYN(void);
unsigned char Servo_SendPosition(unsigned char id, float pos);
void Servo_SendPositions(void);
void Servo_SetForwardingMode(unsigned char state);

float Servo_ReadPos(unsigned char id);
int Servo_TxRxPacket(unsigned char *dataTX, unsigned char *dataRX);
unsigned char Servo_SetPassive(unsigned char id, unsigned char state);
unsigned char Servo_GetPassive(unsigned char id);
void Servo_SetPositionsTorque(unsigned char torque);
void Servo_SendTGain(unsigned char id, char pGain, char iGain, char dGain);
void Servo_SendTGainGlobal(float pGain, float iGain, float dGain);
void Servo_GetTGainGlobal(float* pGain, float* iGain, float* dGain);
unsigned char Servo_IsResponding(unsigned char id);
void Servo_SetID(unsigned char oldid, unsigned char newid);
void Servo_SetBaudrate(unsigned char id, unsigned int baud);
unsigned char Servo_ReadLoad(unsigned char id);
void Servo_ReadPID(unsigned char id, unsigned char* p, unsigned char* i, unsigned char* d);
unsigned char Servo_ReadAddress8(unsigned char id,unsigned char address);

void Servo_SendExPortD(unsigned char id, char exPortD);
void Servo_SetOverload(unsigned char id, unsigned char overload);
void Servo_SetBoundaries(unsigned char id, unsigned short lower, unsigned short upper);
void Servo_ReadBounds(unsigned char id, unsigned short* bound_l, unsigned short* bound_u);
unsigned char Servo_ReadOverload(unsigned char id);

void Servo_SendData(unsigned char* data, int length);

void Servo_SetPowerState(unsigned char state);
unsigned char Servo_ReadVoltage(unsigned char id);
unsigned char Servo_ReadTemperature(unsigned char id);

void Servo_UpdateStatus(struct servoData_t servo_status[]);
void Servo_ResetMaxError(void);

void Servo_SetType(unsigned char id, unsigned char type);
#endif

#endif /* SERVO_DYNAMIXEL_H_ */
