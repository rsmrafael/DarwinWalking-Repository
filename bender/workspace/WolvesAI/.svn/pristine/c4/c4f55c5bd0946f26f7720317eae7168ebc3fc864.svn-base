/*
 * FileProcessor.h
 *
 *  Created on: 18.01.2010
 *      Author: Stefan
 */

#ifndef FILEPROCESSOR_H_
#define FILEPROCESSOR_H_

#include <string>
#include "../PacketListener.h"
#include "../../Thread.h"

/**
 * File Service
 */
class FileService : public Thread, public PacketListener {
public:
	FileService();
	virtual ~FileService();
	virtual void newPacket(uint8_t* packet, size_t size);
	virtual std::string getPacketID();

private:
	void execute(void* arg);
	void sendData(uint32_t blockNr, const char* data, size_t length);
	void sendACK(uint32_t blockNr);
	void sendError(uint32_t errorCode, std::string errorMessage);
	void transferFileData(bool resend);

	static const int PACKET_BUFFER_SIZE;

	uint8_t *mPacketBuffer;
	size_t mPacketSize;
	bool mPacketInBuffer;

	uint8_t mState;
	timeval mTimeoutStart;
	FILE* mFile;
	uint32_t mPacketNum;
	uint8_t mPacketRepeatCnt;
};

#endif /* FILEPROCESSOR_H_ */
