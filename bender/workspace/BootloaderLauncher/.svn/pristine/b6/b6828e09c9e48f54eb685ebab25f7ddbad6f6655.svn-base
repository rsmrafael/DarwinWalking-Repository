/*
 * ICommunicator.h
 *
 *  Created on: 07.03.2011
 *      Author: Stefan
 */

#ifndef ICOMMUNICATOR_H_
#define ICOMMUNICATOR_H_

#include <string>

using namespace std;

class ICommunicator {

public:
	enum Motion { 	Stop = 0x00,
					KickRight = 0x07, KickLeft = 0x06,
					GoaliMoveLeft = 0x0B, GoaliMoveRight = 0x0C,
					MoveHi= 0x0E, YeahGoal=0x0F
				};

	enum StatusItem { BatteryVoltage = 0x01, IMUData = 0x02 };

#define SERVO_COUNT_MAX		21

	struct servoData_t {
		unsigned char maxPositionError;
		unsigned char temperature;
		unsigned char voltage;
	} __attribute__((packed));

	struct imuData_t {
		int fb_accel;
		int rl_accel;
		int fb_gyro;
		int rl_gyro;
	} __attribute__((packed));

	struct statusMessage_t {
		unsigned short voltage;
		struct servoData_t servo_status[SERVO_COUNT_MAX];
		struct imuData_t imu;
	} __attribute__((packed));

    virtual ~ICommunicator() {};

	virtual void RepeatedMovementCommand(Motion page, bool overwriteRunning, unsigned short repetitions) = 0;
	virtual void StrengthDirectionMovementCommand(Motion page, bool overwriteRunning, unsigned short strength, unsigned short direction) = 0;
	virtual void PanTiltCommand(signed char pan, signed char tilt) = 0;
	virtual void GetStatusRequest(StatusItem item, double* data1, double* data2) = 0;
	virtual void WalkerCommand(signed char x, signed char y, signed char r, bool turningAim) = 0;
	virtual void StartBootloaderCommand() = 0;

	virtual const struct statusMessage_t* GetStatusMessage() = 0;
};

#endif /* ICOMMUNICATOR_H_ */
