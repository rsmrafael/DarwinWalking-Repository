/*
 * TestCommunicator.h
 *
 *  Created on: 29.12.2013
 *      Author: Oliver Krebs
 */

#ifndef TESTCOMMUNICATOR_H_
#define TESTCOMMUNICATOR_H_

#include "AbstractCommunicator.h"

class TestCommunicator : public AbstractCommunicator {

public:
	enum TransmissionStatus {
		OKAY,
		INVALID_TYPE,
		INVALID_INSTRUCTION,
		INVALID_CONTROLLER,
		INVALID_LENGTH,
		INVALID_CONTENT
	};

	TestCommunicator();
	~TestCommunicator();

	const struct statusMessage_t* GetStatusMessage();
	void SendData(uint8_t type, uint8_t instruction, uint8_t controller, size_t commandLen, uint8_t* commandContent);

	TransmissionStatus GetTransmissionStatus() const;

	void SetVoltage(uint16_t voltage);
	void SetImuData(const imuData_t &imu);
	void SetOdometryData(const odometryData_t &odometry);

	int16_t GetPan() const;
	int16_t GetTilt() const;

	int8_t GetWalkX() const;
	int8_t GetWalkY() const;
	int8_t GetWalkR() const;
	bool GetWalkTurningAim() const;

	vect3d_t GetKickerBall() const;
	vect3d_t GetKickerTarget() const;

	Motion GetMotionPage() const;
	uint16_t GetMovementRepetitions() const;
	uint16_t GetMovementStrength() const;
	uint16_t GetMovementDirection() const;

private:
	void ReceiveWalkerCommand(size_t commandLen, const uint8_t* commandContent);
	void ReceivePanTiltCommand(size_t commandLen, const uint8_t* commandContent);
	void ReceiveKickerCommand(size_t commandLen, const uint8_t* commandContent);
	void ReceiveStrengthDirectionMovementCommand(size_t commandLen, const uint8_t* commandContent);
	void ReceiveRepeatedMovementCommand(size_t commandLen, const uint8_t* commandContent);

	TransmissionStatus transmissionStatus;

	struct statusMessage_t statusMessage;

	int16_t pan;
	int16_t tilt;

	int8_t walkX;
	int8_t walkY;
	int8_t walkR;
	bool walkTurningAim;

	vect3d_t kickerBall;
	vect3d_t kickerTarget;

	Motion motionPage;
	uint16_t movementRepetitions;
	uint16_t movementStrength;
	uint16_t movementDirection;

};

#endif /* TESTCOMMUNICATOR_H_ */
