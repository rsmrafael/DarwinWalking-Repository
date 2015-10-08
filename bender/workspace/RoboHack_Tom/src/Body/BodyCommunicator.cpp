/*
 * Communicator.cpp
 *
 *  Created on: 24.06.2009
 *      Author: Stefan
 */

#include "BodyCommunicator.h"
#include "../Debugging/Debugger.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#ifndef WIN32
#include <sys/signal.h>
#include <errno.h>
#endif
#include <sys/types.h>
#include <string.h>

#define TIMEOUT_US	250000

#define CONTROLLER_ADDRESS	1

#define TYPE_INSTRUCTION	1

#define STATE_IDLE			0xfff0
#define STATE_START			0xfff1
#define STATE_ADDRESS		0xfff2
#define STATE_INSTRUCTION	0xfff3
#define STATE_LENGTH1		0xfff4

using namespace std;

/**
 * Initalise all Parameters and start the Thread to get Packets from the body.
 * @param device is the device Typ as String for debug messages
 * @param noComms is a boolean defines if Communication should be possible. If true thread started, else the thread dont start and no communication is possible.
 */
BodyCommunicator::BodyCommunicator(string device, bool noComms) {
	Debugger::INFO("BodyCommunicator", "Init with device: %s %s", device.c_str(), noComms ? "noComms" : ".");
	mNoComms = noComms;
	mDeviceName = device;
	mComFileDescriptor = -1;
#ifndef WIN32
	mOldComSettings.c_cflag = 0;
	mOldComSettings.c_iflag = 0;
	mOldComSettings.c_ispeed = 0;
	mOldComSettings.c_ospeed = 0;
#else
	mOldComSettings.dummy = 0;
#endif
	if (!mNoComms) {
		InitComPort(mDeviceName);
	}

	memset(&mParserCurrentMessage, 0, sizeof(mParserCurrentMessage));
	mParserState = STATE_IDLE;
	mParserLastBytes[0] = 0; mParserLastBytes[1] = 0;
	mParserTemp = 0;
	mParserChecksum = 0;
	memset(&mStatusMessage, 0, sizeof(mStatusMessage));
	mStatusMessageAvailable = false;

	if (!mNoComms) {
		this->start(this);
	}
}

BodyCommunicator::~BodyCommunicator() {
	if (mComFileDescriptor >= 0) {
#ifndef WIN32
		tcsetattr(mComFileDescriptor, TCSANOW, &mOldComSettings); // Restore old com port settings
#endif
		close(mComFileDescriptor); //close the com port
	}
	if (!mNoComms) {
		this->stop();
	}
}

/**
 * Thread runs and waiting for a message to proceed. It looks at the Comport for a message in every run.
 */
void BodyCommunicator::execute(void* arg) {
#define MESSAGE_BUFFER_LEN	200
	uint8_t buf[MESSAGE_BUFFER_LEN];

	while (IsRunning()) {
		if (mComFileDescriptor != -1) {
			fd_set setRead;
			int notused = 63;
			int cnt;

			FD_ZERO(&setRead);
			FD_SET(mComFileDescriptor, &setRead);

			struct timeval timeout;
			timeout.tv_usec = TIMEOUT_US;
			timeout.tv_sec  = 0;
			cnt = select(notused, &setRead, NULL, NULL, &timeout);
			if (cnt > 0 && mComFileDescriptor != -1) {
				if (FD_ISSET(mComFileDescriptor, &setRead)) {
					ssize_t res = read(mComFileDescriptor, (char *)buf, sizeof(buf));
					if (res > 0) {
						//Debugger::DEBUG("[Communicator]", "Received %u bytes", res);
						for (uint32_t i = 0; i < (size_t)res; ++i) {
							ProcessChar(buf[i]);
						}
					}
				}
			}
		}
	}

   	Debugger::DEBUG("Communicator", "Listening thread ending");
}

/**
 * Process Char from the USB port and check if it the startbye, instruction .. and get it state by state.
 * Starting with the STATE_START,STATE_ADDRESS it saves the message in the local variables.
 * At the end it call the ProcessMessage method to manage the hole message.
 * @param ch the char get from the port
 */
void BodyCommunicator::ProcessChar(uint8_t ch) {
	if (ch == START_BYTE3 && mParserLastBytes[1] == START_BYTE2 && mParserLastBytes[0] == START_BYTE1) {
		mParserState = STATE_START;
		mParserChecksum = 0;
		mParserTemp = 0;
		memset(&mParserCurrentMessage, 0, sizeof(mParserCurrentMessage));
	} else if (mParserState == STATE_START) {
		if (ch == (0x10 | CONTROLLER_ADDRESS) || ch == (0x20 | CONTROLLER_ADDRESS)) {
			mParserCurrentMessage.type = ((ch & 0xF0) >> 4);
			mParserState = STATE_ADDRESS;
		} else {
			//Debugger::DEBUG("[Communicator]", "Message is not for this controller");
			mParserState = STATE_IDLE; // Message is not for this controller
		}
		mParserChecksum += ch;
	} else if (mParserState == STATE_ADDRESS) {
		mParserCurrentMessage.instruction = ch;
		mParserState = STATE_INSTRUCTION;
		mParserChecksum += ch;
	} else if (mParserState == STATE_INSTRUCTION) {
		mParserTemp = ch;
		mParserState = STATE_LENGTH1;
		mParserChecksum += ch;
	} else if (mParserState == STATE_LENGTH1) {
		mParserCurrentMessage.length = (mParserTemp << 8) | ch;
		mParserState = 0;
		mParserChecksum += ch;
	} else if (mParserState < mParserCurrentMessage.length) {
		if (mParserState < sizeof(mParserCurrentMessage.data)) {
			mParserCurrentMessage.data[mParserState] = ch;
		}
		mParserState++;
		mParserChecksum += ch;
	} else if (mParserState == mParserCurrentMessage.length) {
		if (mParserChecksum == ch) {
			ProcessMessage(&mParserCurrentMessage);
		} else {
			Debugger::ERR("[Communicator]", "Invalid checksum");
		}
		mParserState = STATE_IDLE;
	}

	mParserLastBytes[0] = mParserLastBytes[1];
	mParserLastBytes[1] = ch;
}

/**
 * Process the message if the instruction is a statusmessage it is written in mStatusMessage and mStatusMessageAvailable is set to true.
 * @param message the message that comes directly from the robot.
 */
void BodyCommunicator::ProcessMessage(const struct parser_message* message) {
	if (message->instruction == INSTRUCTION_STATUSMESSAGE) {
		if (message->length == sizeof(statusMessage_t)) {
			memcpy(&mStatusMessage, message->data, sizeof(statusMessage_t));
			mStatusMessageAvailable = true;
		} else {
			Debugger::ERR("Communicator", "statusMessage_t size mismatch!");
		}
	}
}
/**
 * Return the last insertet statusmessage if the Statusmessages is avaidable.
 * @return A Reference of the Statusmessage  ICommunicator::statusMessage_t.
 */
const struct ICommunicator::statusMessage_t* BodyCommunicator::GetStatusMessage() {
	if (mStatusMessageAvailable) {
		return &mStatusMessage;
	}
	return NULL;
}
/**
 * Init CommPort is used in WIN32 Def for opening the Com Port.
 * @param device is used for print out debug informations.
 */
bool BodyCommunicator::InitComPort(string device) {
#ifndef WIN32
	// open the device(com port)
	mComFileDescriptor = open(device.c_str(), O_RDWR | O_NOCTTY);
	if (mComFileDescriptor < 0) {
		Debugger::ERR("Communicator", "Could not open %s: %s", device.c_str(), strerror(errno));
		return false;
	}

    struct termios newComSettings;
    if (tcgetattr(mComFileDescriptor, &mOldComSettings) != 0) { // save current port settings
    	Debugger::ERR("Communicator", "Error reading current port settings!");
    	close(mComFileDescriptor);
    	mComFileDescriptor = -1;
    	return false;
    }
    memcpy(&newComSettings, &mOldComSettings, sizeof(struct termios));
    // set new port settings for canonical input processing
    newComSettings.c_cflag = B19200 | CS8 | CLOCAL | CREAD;
    newComSettings.c_iflag = IGNPAR;
    newComSettings.c_oflag = 0;

    newComSettings.c_lflag = 0;       //ICANON;

    newComSettings.c_cc[VMIN] = 1;
    newComSettings.c_cc[VTIME] = 0;

    tcflush(mComFileDescriptor, TCIFLUSH);
    tcsetattr(mComFileDescriptor, TCSANOW, &newComSettings);
    return true;
#else
    mComFileDescriptor = -1;
    return false;
#endif
}

/**
 * Send Data send a Package to the Body Controller. Every Command use this Method to send the Package to the Real robot over the com port.
 * @param type is the type of the package if it is a Command or a Request
 * @param instruction signals what the body do main with the package and where it is played. If it is a motion,walker,kicker...
 * @param commandLen is the leangth of the Command content in bytes.
 * @param commandContent is the data that is added to the Command like the pan tilt values.
 */
void BodyCommunicator::SendData(uint8_t type, uint8_t instruction, uint8_t controller, size_t commandLen, uint8_t* commandContent) {
	uint8_t header[7];
	uint8_t checksum = 0;

	if (mNoComms) return;

	if (mComFileDescriptor < 0) {
#ifndef WIN32
		Debugger::WARN("Communicator", "COM-Port not open, trying to re-open...");
		InitComPort(mDeviceName);
#endif
		return;
	}

	header[0] = START_BYTE1;
	header[1] = START_BYTE2;
	header[2] = START_BYTE3;
	header[3] = ((type & 0xF0) | (controller & 0x0F));
	header[4] = instruction;
	header[5] = ((commandLen >> 8) & 0xFF);
	header[6] = (commandLen & 0xFF);

	for (uint8_t i = 3; i < 7; i++) { // 3 = without startbytes
		checksum += header[i];
	}
	for (uint8_t i = 0; i < commandLen; i++) {
		checksum += commandContent[i];
	}

	ssize_t written = write(mComFileDescriptor, &header[0], (size_t)7);
	if (commandLen > 0) {
		written = write(mComFileDescriptor, commandContent, commandLen);
	}
	if (written <= 0) {
		Debugger::ERR("Communicator", "Error writing, re-opening port...");
		close(mComFileDescriptor); //close the com port
		InitComPort(mDeviceName);
		return;
	}
	written = write(mComFileDescriptor, &checksum, (size_t)1);
	if (written <= 0) {
		Debugger::ERR("Communicator", "Error writing, re-opening port...");
		close(mComFileDescriptor); //close the com port
		InitComPort(mDeviceName);
		return;
	}
}
