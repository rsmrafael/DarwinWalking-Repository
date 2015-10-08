/*
 * ICommunicator.cpp
 *
 *  Created on: 21.10.2010
 *      Author: Adam
 */

#include "AbstractCommunicator.h"
#include "../Debugging/Debugger.h"
#include "../Utils/DataConversion.h"

AbstractCommunicator::AbstractCommunicator()
:	mLastPan( 0),
	mLastTilt( 0)
{
}

AbstractCommunicator::~AbstractCommunicator()
{

}


void AbstractCommunicator::RepeatedMovementCommand(Motion page, bool overwriteRunning, uint16_t repetitions) {
	uint8_t data[5];

	data[0] = 0;
	if (overwriteRunning) {
		data[0] |= 1;
	}
    //lint -e{641} Enum to int
	data[1] = page;
	data[2] = 1; // 1 parameter
	U16ToU8( repetitions, data[3], data[4]);

	SendData(COMMAND, INSTRUCTION_MOVEMENT, CONTROLLER_MOVEMENT, (size_t)5, (uint8_t*) &data[0]);
}


void AbstractCommunicator::StrengthDirectionMovementCommand(Motion page, bool overwriteRunning, uint16_t strength, uint16_t direction) {
	uint8_t data[7];

	data[0] = 0;
	if (overwriteRunning) {
		data[0] |= 1;
	}
    //lint -e{641} Enum to int
	data[1] = page;
	data[2] = 2; // 2 parameters
	U16ToU8( strength, data[3], data[4]);
	U16ToU8( direction, data[5], data[6]);

	SendData(COMMAND, INSTRUCTION_MOVEMENT, CONTROLLER_MOVEMENT, (size_t)7, &data[0]);
}

void AbstractCommunicator::KickerCommand(vect3d_t ball) {
	uint8_t data[7];

	data[0] = 1;
	S16ToU8( ball.x, data[1], data[2]);
	S16ToU8( ball.y, data[3], data[4]);
	S16ToU8( ball.z, data[5], data[6]);

	SendData(COMMAND, INSTRUCTION_KICKER, CONTROLLER_MOVEMENT, (size_t)7, &data[0]);
}

void AbstractCommunicator::KickerCommand(vect3d_t ball, vect3d_t target) {
	uint8_t data[13];

	data[0] = 5;
	S16ToU8( ball.x, data[1], data[2]);
	S16ToU8( ball.y, data[3], data[4]);
	S16ToU8( ball.z, data[5], data[6]);
	S16ToU8( target.x, data[7], data[8]);
	S16ToU8( target.y, data[9], data[10]);
	S16ToU8( target.z, data[11], data[12]);

	SendData(COMMAND, INSTRUCTION_KICKER, CONTROLLER_MOVEMENT, (size_t)13, &data[0]);
}

void AbstractCommunicator::KickerCommand(vect3d_t ball, vect3d_t target, uint8_t kicktype) {
	uint8_t data[14];

	data[0] = 6;
	S16ToU8( ball.x, data[1], data[2]);
	S16ToU8( ball.y, data[3], data[4]);
	S16ToU8( ball.z, data[5], data[6]);
	S16ToU8( target.x, data[7], data[8]);
	S16ToU8( target.y, data[9], data[10]);
	S16ToU8( target.z, data[11], data[12]);
	data[13] = kicktype;

	SendData(COMMAND, INSTRUCTION_KICKER, CONTROLLER_MOVEMENT, (size_t)14, &data[0]);
}


void AbstractCommunicator::PanTiltCommand(int16_t pan, int16_t tilt) {

	if (pan == mLastPan && tilt == mLastTilt) {
		return;
	}

	mLastPan = pan;
	mLastTilt = tilt;

	//Debugger::INFO("AbstractCommunicator", "PanTilt: %d/%d", pan, tilt);

	uint8_t data[4];
	S16ToU8( pan, data[0], data[1]);
	S16ToU8( tilt, data[2], data[3]);

	//Debugger::INFO("AbstractCommunicator", "Converted PanTilt: %d,%d;%d,%d", data[0], data[1], data[2], data[3]);

	SendData(COMMAND, INSTRUCTION_PANTILT, CONTROLLER_MOVEMENT, (size_t)4, &data[0]);
}

void AbstractCommunicator::WalkerCommand(int8_t x, int8_t y, int8_t r, bool turningAim) {
	uint8_t data[4];
	data[0] = (uint8_t) x;
	data[1] = (uint8_t) y;
	data[2] = (uint8_t) r;
	data[3] = (uint8_t) turningAim;

	SendData(COMMAND, INSTRUCTION_WALKER, CONTROLLER_MOVEMENT, (size_t)4, &data[0]);
}
