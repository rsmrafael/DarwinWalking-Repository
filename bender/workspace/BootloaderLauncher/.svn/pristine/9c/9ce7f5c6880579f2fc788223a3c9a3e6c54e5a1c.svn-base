/*
 * ICommunicator.h
 *
 *  Created on: 20.10.2010
 *      Author: Adam
 */

#pragma once

#ifdef WIN32
#ifndef TERMIOS_DEFINED
#define TERMIOS_DEFINED
struct termios {
	int dummy;
};
#endif
#else
#include <termios.h>
#endif
#include <string>
#include "ICommunicator.h"

using namespace std;

#define START_BYTE1				0xFF
#define START_BYTE2				0xFE
#define START_BYTE3				0xFD
#define COMMAND					0x10
#define REQUEST 				0x20
//#define REPLY					0x40

#define CONTROLLER_MOVEMENT		4

#define INSTRUCTION_MOVEMENT		0x10
#define INSTRUCTION_PANTILT			0x12
#define INSTRUCTION_WALKER			0x15
#define INSTRUCTION_GETSTATUS		0x1b
#define INSTRUCTION_STATUSMESSAGE	0x20
#define INSTRUCTION_BOOTLOADER		0x99

class AbstractCommunicator : public ICommunicator {

public:
	AbstractCommunicator() {
	};
	virtual ~AbstractCommunicator() {
	};

	virtual void RepeatedMovementCommand(Motion page, bool overwriteRunning, unsigned short repetitions);
	virtual void StrengthDirectionMovementCommand(Motion page, bool overwriteRunning, unsigned short strength, unsigned short direction);
	virtual void PanTiltCommand(signed char pan, signed char tilt);
	virtual void WalkerCommand(signed char x, signed char y, signed char r, bool turningAim);
	virtual void StartBootloaderCommand();

	virtual void GetStatusRequest(StatusItem item, double* data1, double* data2) = 0;

	virtual const struct statusMessage_t* GetStatusMessage() = 0;
private:

protected:
	virtual void SendData(unsigned char type, unsigned char instruction, unsigned char controller, size_t commandLen, unsigned char* commandContent) = 0;
	virtual int ReadByte() = 0;
	virtual int ReadShort() = 0;
};
