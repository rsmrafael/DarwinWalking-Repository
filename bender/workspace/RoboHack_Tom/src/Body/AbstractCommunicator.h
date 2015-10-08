/*
 * AbstractCommunicator.h
 *
 *  Created on: 20.10.2010
 *      Author: Adam
 */

#ifndef ABSTRACTCOMMUNICATOR_H_
#define ABSTRACTCOMMUNICATOR_H_

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
/*! \file */

#define START_BYTE1				0xFF/*!<Startbyte of Message to Body, signal for a message*/
#define START_BYTE2				0xFE/*!<Startbyte of Message to Body, signal for a message*/
#define START_BYTE3				0xFD/*!<Startbyte of Message to Body, signal for a message */
#define COMMAND					0x10/*!<Signals that a Command is send like setPantilt */
#define REQUEST 				0x20/*!<Signals that this is a Request for informations */
//#define REPLY					0x40

#define CONTROLLER_MOVEMENT		4 /*!< The Address of the Body Controller, used in the comm Package to identify the resever*/

#define INSTRUCTION_MOVEMENT		0x10 /*!<This is a Instruction for a movement like a motion, used by comm Package to identify it */
#define INSTRUCTION_PANTILT			0x12 /*!<This is a Instruction to set the PanTilt only, used by comm Package to identify it*/
#define INSTRUCTION_KICKER			0x14 /*!<This is a Instruction for the Kickengine to kick specificlly, used by comm Package to identify it*/
#define INSTRUCTION_WALKER			0x15 /*!<This Instruction is for the Walker to walk in a direction, used by comm Package to identify it*/
#define INSTRUCTION_GETSTATUS		0x1b /*!<This Instruction is to get StatusMessagefrom the Body, used by comm Package to identify it*/
#define INSTRUCTION_STATUSMESSAGE	0x20 /*!<This instruction specify the package send by the body for statusupdates like batterystatus, used by comm Package to identify it*/

/**
 * AbstractCommunicator is the abstract class for sending messages and reciving them from the Body. The methods can be called to send commands to the body.
 * Implements ICommunicator the main Communicator implementations of this interface are written here.
 */
class AbstractCommunicator : public ICommunicator {

public:
	AbstractCommunicator();//<! The Constructor initalising pan and tilt.
	virtual ~AbstractCommunicator();//<! Destructor dont need to do something

	virtual void RepeatedMovementCommand(Motion page, bool overwriteRunning, uint16_t repetitions);//See ICommunicator
	virtual void StrengthDirectionMovementCommand(Motion page, bool overwriteRunning, uint16_t strength, uint16_t direction);//See ICommunicator
	virtual void KickerCommand(vect3d_t ball);//See ICommunicator
	virtual void KickerCommand(vect3d_t ball, vect3d_t target);//See ICommunicator
	virtual void KickerCommand(vect3d_t ball, vect3d_t target, uint8_t kicktype);//See ICommunicator
	virtual void PanTiltCommand(int16_t pan, int16_t tilt);//See ICommunicator
	virtual void WalkerCommand(int8_t x, int8_t y, int8_t r, bool turningAim);//See ICommunicator

	virtual const struct statusMessage_t* GetStatusMessage() = 0;//See ICommunicator

	virtual void SendData(uint8_t type, uint8_t instruction, uint8_t controller, size_t commandLen, uint8_t* commandContent) = 0;//See ICommunicator

protected:
	int16_t mLastPan;//<! The last pan position send to the body, used for check if the PanTilt changed so only if they changed a new value is send
	int16_t mLastTilt;//<! The last tilt position send to the body, used for check if the PanTilt changed so only if they changed a new value is send
};

#endif /* ABSTRACTCOMMUNICATOR_H_ */
