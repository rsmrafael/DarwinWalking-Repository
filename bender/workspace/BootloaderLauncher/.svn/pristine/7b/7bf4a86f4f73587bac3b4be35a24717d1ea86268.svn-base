/*
 * Body.cpp
 *
 *  Created on: 17.06.2009
 *      Author: Stefan
 */

#include "Body.h"

Body::Body(string device, BodyType type) {
	mParalysis = false;

	mComm = new Communicator(device);
	Stop();

	// Wait till initial position reached
#ifdef WIN32
	Sleep(1000);
#else
	usleep(1000 * 1000);
#endif
}

Body::~Body() {
	delete mComm;
}

bool Body::isParalysed() const {
	return mParalysis;
}

void Body::setParalysis(bool value) {
	if (value && !mParalysis) {
		Stop();
	}
	mParalysis = value;
}

double Body::GetBatteryVoltage() {
	double data;

	const struct ICommunicator::statusMessage_t* status = mComm->GetStatusMessage();
	if (status != NULL) {
		return ((double)status->voltage) / 100.0;
	}
	mComm->GetStatusRequest(ICommunicator::BatteryVoltage, &data, NULL);
	return data;
}

void Body::GetIMUData(int* pitch, int* roll) {
	double data1;
	double data2;

	const struct ICommunicator::statusMessage_t* status = mComm->GetStatusMessage();
	if (status != NULL) {
		*pitch = (int)status->imu.fb_accel;
		*roll = (int)status->imu.rl_accel;
	}
	mComm->GetStatusRequest(ICommunicator::IMUData, &data1, &data2);
	*pitch = (int)data1;
	*roll = (int)data2;
}

const struct ICommunicator::statusMessage_t* Body::GetStatusMessage() {
	return mComm->GetStatusMessage();
}

void Body::setWalkerCommand(signed char x, signed char y, signed char r) {
	setWalkerCommand(x, y, r, false);
}

void Body::setWalkerCommand(signed char x, signed char y, signed char r, bool turningAim) {
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

	if (x > 100) {
		x = 100;
	} else if (x < -100) {
		x = -100;
	}
	if (y > 100) {
		y = 100;
	} else if (y < -100) {
		y = -100;
	}
	if (r > 100) {
		r = 100;
	} else if (r < -100) {
		r = -100;
	}

/*	if (turningAim) {
		Debugger::INFO("Body", "Walker command: %d/%d/%d, turningAim on", x, y, r);
	} else {
		Debugger::INFO("Body", "Walker command: %d/%d/%d", x, y, r);
	}
*/
    mComm->WalkerCommand(x, y, r, turningAim);
}

void Body::Stop(void) {
    mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
}

void Body::KickLeft() {
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

	mComm->StrengthDirectionMovementCommand(ICommunicator::KickLeft, true, 127, 127);
}

void Body::KickRight() {
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

    mComm->StrengthDirectionMovementCommand(ICommunicator::KickRight, true, 127, 127);
}

void Body::GoaliMoveRight(){
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

	mComm->RepeatedMovementCommand(ICommunicator::GoaliMoveRight, true, 1);
}

void Body::GoaliMoveLeft(){
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

	mComm->RepeatedMovementCommand(ICommunicator::GoaliMoveLeft, true, 1);
}

void Body::MoveHi(){
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

	mComm->RepeatedMovementCommand(ICommunicator::MoveHi, true, 1);
}

void Body::YeahGoal(){
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

	mComm->RepeatedMovementCommand(ICommunicator::YeahGoal, true, 1);
}

void Body::startBootloader() {
	mComm->StartBootloaderCommand();
}
