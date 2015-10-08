/*
 * FileService.cpp
 *
 *  Created on: 18.01.2010
 *      Author: Stefan
 */

#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include "FileService.h"
#include "../Debugger.h"

#define RRQ		1 // Read request
#define WRQ		2 // Write request
#define ACK		3 // Acknowledgment
#define DATA	4 // Data
#define ERRORV	5 // Error

#define TIMEOUT	900 // ms
#define MAX_REPEAT_CNT	3

#define TFTPSTATE_IDLE			0
#define TFTPSTATE_RECEIVING		1
#define TFTPSTATE_SENDING		2
#define TFTPSTATE_WAITFORACK	3
#define TFTPSTATE_COMPLETE		4
#define TFTPSTATE_WAITFORDATA	5
#define TFTPSTATE_ERROR			99

const int FileService::PACKET_BUFFER_SIZE = 20 + 512;

FileService::FileService()
:	mPacketSize(0),
	mPacketInBuffer(false),
	mState(TFTPSTATE_IDLE),
	mFile(NULL),
	mPacketNum(0),
	mPacketRepeatCnt(0)
{
	mTimeoutStart.tv_sec = 0;
	mTimeoutStart.tv_usec = 0;
	mPacketBuffer = new uint8_t[PACKET_BUFFER_SIZE];
	memset(mPacketBuffer, 0, PACKET_BUFFER_SIZE);
	this->start(this);
}

FileService::~FileService() {
	this->stop();
	if (mFile != NULL) {
		fclose(mFile);
		mFile = NULL;
	}
	delete [] mPacketBuffer;
}

void FileService::execute(void* arg) {
	timeval current;
	do {
		if (mPacketInBuffer) {
			mPacketInBuffer = false;
			int opcode = ((mPacketBuffer[0] << 8) + mPacketBuffer[1]);

			switch (opcode) {
				case RRQ: { // Brackets are necessary when declaring variables in cases
					if (mState == TFTPSTATE_IDLE) {
						struct stat stFileInfo;

						std::string filename = (char*)(mPacketBuffer + 2);
						Debugger::DEBUG("FileService", "Incoming file read request for '%s'", filename.c_str());
						if (stat(filename.c_str(), &stFileInfo) == 0) { // File exists
							if ((mFile = fopen(filename.c_str(), "rb")) != NULL) {
								mState = TFTPSTATE_SENDING;
								mPacketNum = 0;
								mPacketRepeatCnt = 0;
								transferFileData(false);
							} else {
								sendError(2, "Could not open file!");
							}
						} else {
							sendError(1, "File not found!");
						}
					} else {
						sendError(4, "Last transfer not yet completed!");
					}
					break;
				}
				case WRQ: {
					if (mState == TFTPSTATE_IDLE) {
						std::string filename = (char*)(mPacketBuffer + 2);
						Debugger::DEBUG("FileService", "Incoming file write request for '%s'", filename.c_str());
						if ((mFile = fopen(filename.c_str(), "wb")) != NULL) {
							mPacketNum = 1;
							mPacketRepeatCnt = 0;
							mState = TFTPSTATE_WAITFORDATA;
							gettimeofday(&mTimeoutStart, 0);
							sendACK(0);
						} else {
							sendError(2, "Could not open file!");
						}
					} else {
						sendError(4, "Last transfer not yet completed!");
					}
					break;
				}
				case ACK:
					if (mState == TFTPSTATE_WAITFORACK) {
						uint32_t packetID = (((uint8_t)mPacketBuffer[2] << 8) + (uint8_t)mPacketBuffer[3]);
						if (packetID == mPacketNum) {
							mState = TFTPSTATE_SENDING;
							transferFileData(false);
						} else {
							Debugger::WARN("FileService", "Received ACK with non-current packet ID (expected: %i, received: %i)!", mPacketNum, packetID);
						}
					}
					break;
				case DATA:
					if (mState == TFTPSTATE_WAITFORDATA) {
						uint32_t packetID = (((uint8_t)mPacketBuffer[2] << 8) + (uint8_t)mPacketBuffer[3]);
						if (packetID == mPacketNum) {
							if (mFile != NULL) {
								fwrite(&mPacketBuffer[4], (size_t)1, mPacketSize - 4, mFile);
							} else {
								mState = TFTPSTATE_ERROR;
								sendError(0, "File not open!");
							}

							if (ferror(mFile)) {
								mState = TFTPSTATE_ERROR;
								sendError(0, "Error writing to file!");
							}

							if (mPacketSize < 516) {
								fclose(mFile);
								mFile = NULL;
								mState = TFTPSTATE_IDLE;
							} else {
								mState = TFTPSTATE_WAITFORDATA;
								gettimeofday(&mTimeoutStart, 0);
							}
							sendACK(packetID);
							mPacketNum = packetID + 1;
						} else {
							Debugger::WARN("FileService", "Received DATA with non-current packet ID (expected: %i, received: %i)!", mPacketNum, packetID);
						}
					}
					break;
				case ERRORV:
					mState = TFTPSTATE_ERROR;
					break;
				default:
					Debugger::ERR("FileService", "Received unknown opcode");
					break;
			}
		}
		if (mState == TFTPSTATE_IDLE) {
#ifdef WIN32
			Sleep(100);
#else
			usleep(100 * 1000);
#endif
		}
		if (mState == TFTPSTATE_WAITFORACK || mState == TFTPSTATE_WAITFORDATA) {
			gettimeofday(&current, 0);
			long cur_time = ((current.tv_sec - mTimeoutStart.tv_sec) * 1000000) + (current.tv_usec - mTimeoutStart.tv_usec);
			if (cur_time >= TIMEOUT * 1000) {

	  			if (mPacketRepeatCnt <= MAX_REPEAT_CNT) {
	  				mPacketRepeatCnt++;
	  				Debugger::WARN("FileService", "Last packet timed out, RepeatCount=%i", mPacketRepeatCnt);
					if (mState == TFTPSTATE_WAITFORDATA) {
						sendACK(mPacketNum - 1);
					} else {
						transferFileData(true);
					}
	  				gettimeofday(&mTimeoutStart, 0);
	  			} else {
	  				if (mFile != NULL) {
	  					fclose(mFile);
	  					mFile = NULL;
	  				}
					if (mState == TFTPSTATE_WAITFORDATA) {
						Debugger::ERR("FileService", "Timeout while waiting for DATA");
						sendError(4, "Timeout while waiting for DATA");
					} else {
						Debugger::ERR("FileService", "Timeout while waiting for ACK");
						sendError(4, "Timeout while waiting for ACK");
					}
					mState = TFTPSTATE_IDLE;
	  			}
			}
		}
	} while (IsRunning());
}

void FileService::sendData(uint32_t blockNr, const char* data, size_t length) {
	char senddata[516];

	senddata[0] = 0;
	senddata[1] = DATA & 0xFF;

	senddata[2] = (char)((blockNr >> 8) & 0xFF);
	senddata[3] = (char)(blockNr & 0xFF);

	memcpy((char* )&senddata[4], data, length);

	Debugger::GetInstance()->sendPacket(getPacketID(), senddata, 4 + length);
}

void FileService::sendACK(uint32_t blockNr) {
	char senddata[4];

	senddata[0] = 0;
	senddata[1] = ACK & 0xFF;

	senddata[2] = (char)((blockNr >> 8) & 0xFF);
	senddata[3] = (char)(blockNr & 0xFF);

	Debugger::GetInstance()->sendPacket(getPacketID(), senddata, (size_t)4);
}

void FileService::sendError(uint32_t errorCode, std::string errorMessage) {
	char data[256];

	data[0] = 0;
	data[1] = ERRORV & 0xFF;

	data[2] = (char)((errorCode >> 8) & 0xFF);
	data[3] = (char)(errorCode & 0xFF);

	strcpy((char* )&data[4], errorMessage.c_str());

	Debugger::GetInstance()->sendPacket(getPacketID(), data, 4 + errorMessage.length() + 1);
}

void FileService::transferFileData(bool resend) {
	bool isLastPacket = false;
	size_t count;
	char buffer[512];

	if (mFile == NULL || feof(mFile)) {
		buffer[0] = 0;
		count = 0;
	} else {
		if (resend) {
			if (mPacketNum < 1) {
				mState = TFTPSTATE_ERROR;
				sendError(0, "Invalid packet num on resend!");
				return;
			}
			fseek(mFile, ((long)(mPacketNum - 1) * 512L), SEEK_SET);
		}
		count = fread(buffer, (size_t)1, (size_t)512, mFile);

		if (ferror(mFile)) {
			mState = TFTPSTATE_ERROR;
			sendError(0, "Error reading from file!");
			return;
		}
	}
	if (count < 512) {
		isLastPacket = true;
	}
	if (!resend) {
		mPacketNum++;
	}

	//Debugger::DEBUG("FileService", "Sending data packet nr. %i", mPacketNum);

	sendData(mPacketNum, buffer, count);

	mState = TFTPSTATE_WAITFORACK;
	gettimeofday(&mTimeoutStart, 0);

	if (isLastPacket) {
		fclose(mFile);
		mFile = NULL;
		mState = TFTPSTATE_IDLE;
	}
}

void FileService::newPacket(uint8_t* packet, size_t size) {
	if (size > (size_t)0) {
		memcpy(&mPacketBuffer[0], packet, size);
		mPacketSize = size;
		mPacketInBuffer = true;
	}
}

std::string FileService::getPacketID() {
	return "FTP";
}
