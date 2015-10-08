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
 * The BodyCommunicator is the Implementation of the Communication sending to the real robot.
 * The BodyCommunicator implements Thread and start it so it runs the hole time to receive and send messages.
 */
class BodyCommunicator : public AbstractCommunicator, public Thread {
public:

	/**
	 * Constructor, initialize all Parameters and start the thread to get packets from the body.
	 * @param device 	is the device for communication
	 * @param noComms 	defines if real communication is available (false) or not (true).
	 */
	BodyCommunicator(std::string device, bool noComms);

	/**
	 * Destructor
	 * close open ports and clean up
	 */
	~BodyCommunicator();

	/**
	 * Get newest status message
	 * @see ICommunicator::GetStatusMessage
	 */
	virtual const statusMessage_t* GetStatusMessage();

	/**
	 * Send a package to body with an instruction and command information.
	 * @see AbstractCommunicator::SendData
	 */
	void SendData(uint8_t type, uint8_t instruction, uint8_t controller, size_t commandLen, uint8_t* commandContent);

private:
	/**
	 * This struct is the structure of the protocol between the components (Body<->AI)
	 */
	struct parser_message {
		uint8_t type;			//!< Here the type (Request or Command) is saved
		uint8_t instruction;	//!< instruction like status-message, Kicker, Walker...
		uint16_t length;		//!< Length of the Data that is send in the package
		uint8_t data[100];		//!< Data of the package like pan and tilt
	};

	/**
	 * Init CommPort is used for initializing and opening the COM Port.
	 * @param device 	is used for print out debug informations.
	 */
	bool InitComPort(std::string device);

	/**
	 * Thread runs and waiting for a message to proceed.
	 */
	void execute(void* arg);

	/**
	 * Process a single char from the COM port and check if it the startbyte, instruction .. and get it state by state.
	 * Starting with the STATE_START,STATE_ADDRESS it saves the message in the local variables.
	 * At the end it call the ProcessMessage method to manage the hole message.
	 * @param ch 	the char get from the port
	 */
	void ProcessChar(uint8_t ch);

	/**
	 * Process the next message of the robot.
	 * @param message the message from the robot.
	 */
	void ProcessMessage(const struct parser_message* message);

	int mComFileDescriptor;			//!< FileDiscriptor used for reading the package
	struct termios mOldComSettings;	//!< Settings in WIN_32 conf for the Com.
	std::string mDeviceName;		//!< Device name for Debugging output

	uint16_t mParserState;			//!< Parse state used to build the Package
	uint8_t mParserChecksum;		//!< ParserChecksum used to calculate checksum for the Package
	uint8_t mParserTemp;			//!< saved instruction
	struct parser_message mParserCurrentMessage; //!< last message received saved by parser
	uint8_t mParserLastBytes[2];	//!< Used for identify the startbytes. Temporary save the last bytes to check one time the last 3.

	statusMessage_t mStatusMessage;	//!< last StatusMessage received.
	bool mStatusMessageAvailable;	//!< false if no message available.

	bool mNoComms;	//!< Determines if the thread started and so communication available.
};

#endif /* COMMUNICATOR_H_ */
