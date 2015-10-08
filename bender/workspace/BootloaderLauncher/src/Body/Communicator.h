/*
 * Communicator.h
 *
 *  Created on: 24.06.2009
 *      Author: Stefan
 */

#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

using namespace std;

#include "AbstractCommunicator.h"
#include "../Thread.h"


class Communicator : public AbstractCommunicator, public Thread {
public:
	Communicator(string device);
	~Communicator();

	void GetStatusRequest(StatusItem item, double* data1, double* data2);

	const struct statusMessage_t* GetStatusMessage();
protected:
	void SendData(unsigned char type, unsigned char instruction, unsigned char controller, size_t commandLen, unsigned char* commandContent);
	int ReadByte();
	int ReadShort();
private:
	struct parser_message {
		unsigned char type;
		unsigned char instruction;
		unsigned short length;
		unsigned char data[100];
	};

	bool InitComPort(string device);

	void execute(void* arg);
	void ProcessChar(unsigned char ch);
	void ProcessMessage(const struct parser_message* message);

	int mComFileDescriptor;
	struct termios mOldComSettings;
	string mDeviceName;

	unsigned short parser_state;
	unsigned char parser_checksum;
	unsigned char parser_temp;
	struct parser_message parser_currentMessage;
	unsigned char parser_lastBytes[2];

	struct statusMessage_t mStatusMessage;
	bool mStatusMessageAvailable;
};

#endif /* COMMUNICATOR_H_ */
