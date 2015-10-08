/*
 * servo.h
 *
 *  Created on: 15.03.2010
 *      Author: Stefan
 */

#ifndef SERVOWCK_H_
#define SERVOWCK_H_

#ifndef DYNAMIXEL

#define SERVO_MIN_VALUE					0
#define SERVO_CENTER_VALUE				127
#define SERVO_MAX_VALUE					254
#define SERVO_MIN_ANGLE					-134.0 // degree
#define SERVO_MAX_ANGLE					135.0 // degree

void Servo_Init_WCK(void);
void Servo_SetPositionsTorque(unsigned char torque);
void Servo_SendPositions(void);
unsigned char Servo_SendPosition(unsigned char id, float pos);
void Servo_SendTGain(unsigned char id, char pGain, char iGain, char dGain);
void Servo_SendTGainGlobal(char pGain, char iGain, char dGain);
void Servo_GetTGainGlobal(char* pGain, char* iGain, char* dGain);
void Servo_SendExPortD(unsigned char id, char exPortD);
unsigned char Servo_SetPassive(unsigned char id, unsigned char state);
unsigned char Servo_GetPassive(unsigned char id);
void Servo_SetID(unsigned char oldid, unsigned char newid);
void Servo_SetBaudrate(unsigned char id, unsigned int baud);
void Servo_SetBoundaries(unsigned char id, unsigned short lower, unsigned short upper);
void Servo_SetOverload(unsigned char id, unsigned char overload);
float Servo_ReadPos(unsigned char id);
unsigned char Servo_ReadLoad(unsigned char id);
unsigned char Servo_ReadOverload(unsigned char id);
void Servo_ReadBounds(unsigned char id, unsigned short* bound_l, unsigned short* bound_u);
void Servo_ReadPID(unsigned char id, unsigned char* p, unsigned char* i, unsigned char* d);
unsigned char Servo_IsResponding(unsigned char id);

void Servo_SetPowerState(unsigned char state);

unsigned char Servo_ReadVoltage(unsigned char id);
unsigned char Servo_ReadTemperature(unsigned char id);
void Servo_SetType(unsigned char id, unsigned char type);

void Servo_UpdateStatus(struct servoData_t servo_status[]);
void Servo_ResetMaxError(void);

void Servo_SendData(unsigned char* data, int length);
#endif

#endif /* SERVOWCK_H_ */
