/*
 * ICommunicator.cpp
 *
 *  Created on: 21.10.2010
 *      Author: Adam
 */

#include "AbstractCommunicator.h"

static signed char lastPan = 0;
static signed char lastTilt = 0;

void AbstractCommunicator::RepeatedMovementCommand(Motion page, bool overwriteRunning, unsigned short repetitions) {
	unsigned char data[5];

	data[0] = 0;
	if (overwriteRunning) {
		data[0] |= 1;
	}
    //lint -e{641} Enum to int
	data[1] = page;
	data[2] = 1; // 1 parameter
	data[3] = (unsigned char) ((repetitions >> 8) & 0xFF);
	data[4] = (unsigned char) (repetitions & 0xFF);

	SendData(COMMAND, INSTRUCTION_MOVEMENT, CONTROLLER_MOVEMENT, (size_t)5, (unsigned char*) &data[0]);
}

void AbstractCommunicator::StrengthDirectionMovementCommand(Motion page, bool overwriteRunning, unsigned short strength, unsigned short direction) {
	unsigned char data[7];

	data[0] = 0;
	if (overwriteRunning) {
		data[0] |= 1;
	}
    //lint -e{641} Enum to int
	data[1] = page;
	data[2] = 2; // 2 parameters
	data[3] = (unsigned char) ((strength >> 8) & 0xFF);
	data[4] = (unsigned char) (strength & 0xFF);
	data[5] = (unsigned char) ((direction >> 8) & 0xFF);
	data[6] = (unsigned char) (direction & 0xFF);

	SendData(COMMAND, INSTRUCTION_MOVEMENT, CONTROLLER_MOVEMENT, (size_t)7, (unsigned char*) &data[0]);
}

void AbstractCommunicator::PanTiltCommand(signed char pan, signed char tilt) {

	if (pan == lastPan && tilt == lastTilt) {
		return;
	}

	lastPan = pan;
	lastTilt = tilt;

	char data[2];
	data[0] = pan;
	data[1] = tilt;

	SendData(COMMAND, INSTRUCTION_PANTILT, CONTROLLER_MOVEMENT, (size_t)2, (unsigned char*) &data[0]);
}

void AbstractCommunicator::WalkerCommand(signed char x, signed char y, signed char r, bool turningAim) {
	char data[4];
	data[0] = x;
	data[1] = y;
	data[2] = r;
	data[3] = turningAim;

	SendData(COMMAND, INSTRUCTION_WALKER, CONTROLLER_MOVEMENT, (size_t)4, (unsigned char*) &data[0]);
}

void AbstractCommunicator::StartBootloaderCommand() {
	unsigned char data[4];
	data[0] = 0xDE;
	data[1] = 0xAD;
	data[2] = 0xBE;
	data[3] = 0xEF;

	SendData(COMMAND, INSTRUCTION_BOOTLOADER, CONTROLLER_MOVEMENT, (size_t)4, (unsigned char*) &data[0]);
}
