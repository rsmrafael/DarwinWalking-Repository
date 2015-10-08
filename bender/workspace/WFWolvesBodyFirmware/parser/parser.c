/*
 * parser.c
 *
 *  Created on: 15.03.2010
 *      Author: Stefan
 */

#include "parser.h"
#include <string.h>
#include "dbgu/dbgu.h"

#define CONTROLLER_ADDRESS	4

#define START_BYTE1			0xff
#define START_BYTE2			0xfe
#define START_BYTE3			0xfd
#define TYPE_INSTRUCTION	1

#define STATE_IDLE			0xfff0
#define STATE_START			0xfff1
#define STATE_ADDRESS		0xfff2
#define STATE_INSTRUCTION	0xfff3
#define STATE_LENGTH1		0xfff4

unsigned short parser_state = STATE_IDLE;
unsigned char parser_checksum;
char parser_temp;
struct parser_message parser_currentMessage;
char parser_lastBytes[2];

#define CIRCULAR_BUFFER_SIZE	5
struct parser_message parser_buffer[CIRCULAR_BUFFER_SIZE];
unsigned char parser_writePointer;
unsigned char parser_readPointer;

void Parser_Init(void) {
	memset(&parser_currentMessage, 0, sizeof(parser_currentMessage));
	memset(&parser_buffer[0], 0, sizeof(parser_currentMessage) * CIRCULAR_BUFFER_SIZE);
	parser_writePointer = 0;
	parser_readPointer  = 0;
	parser_state = STATE_IDLE;
}

void Parser_ProcessChar(char ch) {
	if (ch == START_BYTE3 && parser_lastBytes[1] == START_BYTE2 && parser_lastBytes[0] == START_BYTE1) {
		parser_state = STATE_START;
		parser_checksum = 0;
		parser_temp = 0;
		memset(&parser_currentMessage, 0, sizeof(parser_currentMessage));
	} else if (parser_state == STATE_START) {
		if (ch == (0x10 | CONTROLLER_ADDRESS) || ch == (0x20 | CONTROLLER_ADDRESS)) {
			parser_currentMessage.type = ((ch & 0xF0) >> 4);
			parser_state = STATE_ADDRESS;
		} else {
			parser_state = STATE_IDLE; // Message is not for this controller
		}
		parser_checksum += ch;
	} else if (parser_state == STATE_ADDRESS) {
		parser_currentMessage.instruction = ch;
		parser_state = STATE_INSTRUCTION;
		parser_checksum += ch;
	} else if (parser_state == STATE_INSTRUCTION) {
		parser_temp = ch;
		parser_state = STATE_LENGTH1;
		parser_checksum += ch;
	} else if (parser_state == STATE_LENGTH1) {
		parser_currentMessage.length = (parser_temp << 8) | ch;
		parser_state = 0;
		parser_checksum += ch;
	} else if (parser_state < parser_currentMessage.length) {
		parser_currentMessage.data[parser_state] = ch;
		parser_state++;
		parser_checksum += ch;
	} else if (parser_state == parser_currentMessage.length) {
		if (parser_checksum == ch) {
		    if(!(((parser_writePointer + 1) % CIRCULAR_BUFFER_SIZE) == parser_readPointer)) {
		    	memcpy(&parser_buffer[parser_writePointer], &parser_currentMessage, sizeof(parser_currentMessage)); // Copy buffer
		        parser_writePointer++;
		        parser_writePointer %= CIRCULAR_BUFFER_SIZE;
		    } else {
				dbgu_printf("[Parser] WARNING: Buffer full, message lost!\r\n");
			}
		}
		parser_state = STATE_IDLE;
	}

	parser_lastBytes[0] = parser_lastBytes[1];
	parser_lastBytes[1] = ch;
}

unsigned char Parser_IsMessageWaiting() {
	return !(parser_readPointer == parser_writePointer);
}

void Parser_SetMessageProcessed() {
    if(!(parser_readPointer == parser_writePointer)) {
        parser_readPointer++;
        parser_readPointer %= CIRCULAR_BUFFER_SIZE;
    }
}

struct parser_message* Parser_GetMessage() {
	struct parser_message* msg = NULL;
    if(!(parser_readPointer == parser_writePointer)) {
        msg = &parser_buffer[parser_readPointer];
    }
    return msg;
}

size_t Parser_CreatePacket(unsigned char controllerAddress, unsigned char instruction, unsigned char* data, size_t length, unsigned char* packetBuffer, size_t bufferSize) {
#define PACKET_FORMAT_LENGTH	8
#define CHECKSUMMED_LENGTH		4

	if (bufferSize < (length + PACKET_FORMAT_LENGTH)) {
		return 0;
	}

	packetBuffer[0] = START_BYTE1;
	packetBuffer[1] = START_BYTE2;
	packetBuffer[2] = START_BYTE3;
	packetBuffer[3] = (TYPE_INSTRUCTION << 4) | (controllerAddress & 0x0F);
	packetBuffer[4] = instruction;
	packetBuffer[5] = (length >> 8) & 0xFF;
	packetBuffer[6] = length & 0xFF;

	memcpy(&packetBuffer[7], data, length);

	unsigned char chksum = 0;
	for (int i = 3; i < 3 + length + CHECKSUMMED_LENGTH; ++i) {
		chksum += packetBuffer[i];
	}
	packetBuffer[7 + length] = chksum;

	return length + PACKET_FORMAT_LENGTH;
}
