/*
 * parser.h
 *
 *  Created on: 15.03.2010
 *      Author: Stefan
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stddef.h>

struct parser_message {
	unsigned char type;
	unsigned char instruction;
	unsigned short length;
	char data[50];
};

void Parser_Init(void);

void Parser_ProcessChar(char ch);
unsigned char Parser_IsMessageWaiting(void);
void Parser_SetMessageProcessed(void);
struct parser_message* Parser_GetMessage(void);

size_t Parser_CreatePacket(unsigned char controllerAddress, unsigned char instruction, unsigned char* data, size_t length, unsigned char* packetBuffer, size_t bufferSize);

#endif /* PARSER_H_ */
