/*
 * Communicator.h
 *
 *  Created on: 24.06.2009
 *      Author: Stefan
 */

#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

#include "AbstractCommunicator.h"
#include "../Thread.h"

/**
 * The BodyCommunicator is the Implementation of the Communication sending to the Real robot.
 * The BodyCommunicator implements Thread and start it so it runs the hole time to recive and send messages.
 */
class BodyCommunicator : public AbstractCommunicator, public Thread {
public:
	BodyCommunicator(std::string device, bool noComms);//!< The Constructor to initilize the device and start the Thread
	~BodyCommunicator();//!< Stop the Thread and close the Com Port

	const struct statusMessage_t* GetStatusMessage();//!< Return the last StatusMessage
	void SendData(uint8_t type, uint8_t instruction, uint8_t controller, size_t commandLen, uint8_t* commandContent);//!< Send a Package to a module(Body) with a instruction and command infos.

private:
	/**
	 * This struct is the structure of the protocoll beetween the components (Body<->AI)
	 */
	struct parser_message {
		uint8_t type;//!< Here the type (Request or Command) is saved
		uint8_t instruction;//!< The Instruction like Statusmessage,Kicker,Walker...
		uint16_t length;//!< Lenght of the Data that is send in the package
		uint8_t data[100];//!< Data of the package like pan and tilt
	};

	bool InitComPort(std::string device);//!< inital the Com Port

	void execute(void* arg);//!< execute the Thread waiting for the package on the com.
	void ProcessChar(uint8_t ch);//!< parse the chars on the Com to a message.
	void ProcessMessage(const struct parser_message* message);//!< process after finished message the message recived

	int mComFileDescriptor;//!< FileDiscriptor used for reading the package
	struct termios mOldComSettings;//!< Settings in WIN_32 conf for the Com.
	std::string mDeviceName;//!< Device name for Debugging output

	uint16_t mParserState;//!< Parse state used to build the Package
	uint8_t mParserChecksum;//!< ParserChecksum used to calculate chechsum for the Package
	uint8_t mParserTemp;//!< saved intruction
	struct parser_message mParserCurrentMessage;//!< last message recived saved by parser
	uint8_t mParserLastBytes[2];//!< Used for identifie the 3! Startbytes. Temporaly save the last bytes to check one time the last 3.

	struct statusMessage_t mStatusMessage;//!< last StatusMessage recived.
	bool mStatusMessageAvailable;//!< false if no message avaidable.

	bool mNoComms;//!< Determines if the thread started and so communication avaidable.
};

#endif /* COMMUNICATOR_H_ */
