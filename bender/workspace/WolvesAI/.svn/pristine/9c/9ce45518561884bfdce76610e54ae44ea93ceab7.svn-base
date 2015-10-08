/*
 * TestCommunicator.cpp
 *
 *  Created on: 29.12.2013
 *      Author: Oliver Krebs
 */

#include "TestCommunicator.h"
#include "../Utils/DataConversion.h"

TestCommunicator::TestCommunicator() {
	transmissionStatus = OKAY;
	pan = 0;
	tilt = 0;
	walkX = 0;
	walkY = 0;
	walkR = 0;
	walkTurningAim = false;
	kickerBall.x = 0;
	kickerBall.y = 0;
	kickerBall.z = 0;
	kickerTarget.x = 0;
	kickerTarget.y = 0;
	kickerTarget.z = 0;
	motionPage = ICommunicator::Stop;
	movementRepetitions = 0;
	movementStrength = 0;
	movementDirection = 0;
	statusMessage.currentState = 0;
	statusMessage.imu.fb_accel = 0;
	statusMessage.imu.rl_accel = 0;
	statusMessage.imu.fb_gyro = 0;
	statusMessage.imu.rl_gyro = 0;
	statusMessage.odometry.xPos = 0;
	statusMessage.odometry.yPos = 0;
	statusMessage.odometry.viewDirection = 0;
	statusMessage.odometry.weight = 0;
	statusMessage.voltage = 0;
	for( int i = 0; i < SERVO_COUNT_MAX; i++) {
		statusMessage.servo_status[i].maxPositionError = 0;
		statusMessage.servo_status[i].temperature = 0;
		statusMessage.servo_status[i].voltage = 0;
	}
}

TestCommunicator::~TestCommunicator() {

}

const ICommunicator::statusMessage_t* TestCommunicator::GetStatusMessage() {
	return &statusMessage;
}

void TestCommunicator::SendData(uint8_t type, uint8_t instruction, uint8_t controller, size_t commandLen, uint8_t* commandContent) {
	// reset status
	transmissionStatus = OKAY;
	if( type == COMMAND) {
		switch( instruction) {
		case INSTRUCTION_MOVEMENT:
			if( commandLen >= 7) {
				ReceiveStrengthDirectionMovementCommand(commandLen, commandContent);
			} else {
				ReceiveRepeatedMovementCommand(commandLen, commandContent);
			}
			break;
		case INSTRUCTION_PANTILT:
			ReceivePanTiltCommand(commandLen, commandContent);
			break;
		case INSTRUCTION_KICKER:
			ReceiveKickerCommand(commandLen, commandContent);
			break;
		case INSTRUCTION_WALKER:
			ReceiveWalkerCommand(commandLen, commandContent);
			break;
		default:
			transmissionStatus = INVALID_INSTRUCTION;
			break;
		}
	} else if( type == REQUEST) {

	} else {
		transmissionStatus = INVALID_TYPE;
	}
}

void TestCommunicator::ReceiveWalkerCommand(size_t commandLen, const uint8_t* commandContent) {
	if( commandLen != 4) {
		transmissionStatus = INVALID_LENGTH;
		return;
	}
	walkX = (int8_t) commandContent[0];
	walkY = (int8_t) commandContent[1];
	walkR = (int8_t) commandContent[2];
	walkTurningAim = (bool) commandContent[3];
}

void TestCommunicator::ReceivePanTiltCommand(size_t commandLen, const uint8_t* commandContent) {
	if( commandLen != 4) {
		transmissionStatus = INVALID_LENGTH;
		return;
	}
	U8ToS16( commandContent[0], commandContent[1], pan);
	U8ToS16( commandContent[2], commandContent[3], tilt);
}

void TestCommunicator::ReceiveKickerCommand(size_t commandLen, const uint8_t* commandContent) {
	if( commandLen != 7 && commandLen != 13) {
		transmissionStatus = INVALID_LENGTH;
		return;
	}
	U8ToS16( commandContent[1], commandContent[2], kickerBall.x);
	U8ToS16( commandContent[3], commandContent[4], kickerBall.y);
	U8ToS16( commandContent[5], commandContent[6], kickerBall.z);
	if( commandLen >= 13) {
		U8ToS16( commandContent[7], commandContent[8], kickerTarget.x);
		U8ToS16( commandContent[9], commandContent[10], kickerTarget.y);
		U8ToS16( commandContent[11], commandContent[12], kickerTarget.z);
	}
}

void TestCommunicator::ReceiveStrengthDirectionMovementCommand(size_t commandLen, const uint8_t* commandContent) {
	if( commandLen != 7) {
		transmissionStatus = INVALID_LENGTH;
		return;
	}
	if( commandContent[2] != 2) {
		transmissionStatus = INVALID_CONTENT;
		return;
	}
	motionPage = (ICommunicator::Motion) commandContent[1];
	U8ToU16( commandContent[3], commandContent[4], movementStrength);
	U8ToU16( commandContent[5], commandContent[6], movementDirection);
}

void TestCommunicator::ReceiveRepeatedMovementCommand(size_t commandLen, const uint8_t* commandContent) {
	if( commandLen != 5) {
		transmissionStatus = INVALID_LENGTH;
		return;
	}
	if( commandContent[2] != 1) {
		transmissionStatus = INVALID_CONTENT;
		return;
	}
	motionPage = (ICommunicator::Motion) commandContent[1];
	U8ToU16( commandContent[3], commandContent[4], movementRepetitions);
}

TestCommunicator::TransmissionStatus TestCommunicator::GetTransmissionStatus() const {
	return transmissionStatus;
}

void TestCommunicator::SetVoltage(uint16_t voltage) {
	statusMessage.voltage = voltage;
}

void TestCommunicator::SetImuData(const imuData_t &imu) {
	statusMessage.imu = imu;
}

void TestCommunicator::SetOdometryData(const odometryData_t &odometry) {
	statusMessage.odometry = odometry;
}

int16_t TestCommunicator::GetPan() const {
	return pan;
}

int16_t TestCommunicator::GetTilt() const {
	return tilt;
}

int8_t TestCommunicator::GetWalkX() const {
	return walkX;
}

int8_t TestCommunicator::GetWalkY() const {
	return walkY;
}

int8_t TestCommunicator::GetWalkR() const {
	return walkR;
}

bool TestCommunicator::GetWalkTurningAim() const {
	return walkTurningAim;
}

vect3d_t TestCommunicator::GetKickerBall() const {
	return kickerBall;
}

vect3d_t TestCommunicator::GetKickerTarget() const {
	return kickerTarget;
}

ICommunicator::Motion TestCommunicator::GetMotionPage() const {
	return motionPage;
}

uint16_t TestCommunicator::GetMovementRepetitions() const {
	return movementRepetitions;
}

uint16_t TestCommunicator::GetMovementStrength() const {
	return movementStrength;
}

uint16_t TestCommunicator::GetMovementDirection() const {
	return movementDirection;
}
