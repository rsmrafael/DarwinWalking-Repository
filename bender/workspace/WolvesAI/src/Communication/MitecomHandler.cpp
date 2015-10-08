/*
 * MitecomHandler.cpp
 *
 *  Created on: 24.02.2014
 *      Author: Oliver Krebs
 */

#include "MitecomHandler.h"
#include "MitecomData.h"
#include "../Debugging/Debugger.h"

#include <assert.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

/*------------------------------------------------------------------------------------------------*/

MixedTeamMate Mitecom::parseIncoming(const void *messageData, size_t messageLength, uint16_t teamID) {
	// test endianness, the default code only supports little endian, so a conversion between host byte order
	// and network byte order should cause a mismatch to the original value
	assert(htonl(0x12345678) != 0x12345678);

	MixedTeamMate mate;
	mate.robotID = 0; // mark as invalid

	const MixedTeamCommMessage *message = (const MixedTeamCommMessage*)messageData;

	// check magic bytes in header
	if (MITECOM_MAGIC != message->messageMagic) {
		Debugger::WARN("Mitecom", "Parse: Magic value mismatch (%X:%X)", message->messageMagic, MITECOM_MAGIC);
		return mate;
	}

	// check if correct protocol version
	if (MITECOM_VERSION != message->messageVersion) {
		Debugger::WARN("Mitecom", "Parse: Unsupported protocol version (%d:%d)", message->messageVersion, MITECOM_VERSION);
		return mate;
	}

	// check that we got the full message
	size_t len = sizeof(MixedTeamCommMessage) + sizeof(MixedTeamCommValueStruct) * (message->messageLength - 1);
	if (messageLength != len) {
		Debugger::WARN("Mitecom", "Parse: Mismatched message length (%d:%d)", len, messageLength);
		return mate;
	}

	// only handle messages from members of our own team
	if (message->teamID != teamID) {
		Debugger::INFO("Mitecom", "Parse: Message not from my team (%d:%d)", message->teamID, teamID);
		return mate;
	}

	/*
	 * create a mate and store its corresponding values
	 */
	mate.robotID = message->robotID;

	for (uint16_t index = 0; index < message->messageLength; index++)
	{
		MITECOM_KEYTYPE  key   = message->values[index].key;
		MITECOM_DATATYPE value = message->values[index].value;
		mate.data[key] = value;
	}
	return mate;
}



/*------------------------------------------------------------------------------------------------*/
MixedTeamCommMessage *Mitecom::create(size_t &messageSizePtr, const MixedTeamMate &mate, uint16_t teamID, uint16_t robotID)
{
	size_t messageSize = sizeof(MixedTeamCommMessage) + (mate.data.size() - 1) * sizeof(MixedTeamCommValue);
	messageSizePtr = messageSize;

	MixedTeamCommMessage *msgPtr = (MixedTeamCommMessage*)malloc(messageSize);

	if( msgPtr != NULL) {
		msgPtr->messageMagic    = MITECOM_MAGIC;
		msgPtr->messageVersion  = MITECOM_VERSION;

		msgPtr->messageLength   = (uint16_t)mate.data.size();
		msgPtr->messageFlags    = 0;

		msgPtr->teamID          = teamID;
		msgPtr->robotID         = robotID;

		uint32_t index = 0;
		for (MixedTeamMateData::const_iterator it = mate.data.begin(); it != mate.data.end(); ++it) {
			msgPtr->values[index].key      = it->first;
			msgPtr->values[index].value    = it->second;
			index++;
		}
	}
	return msgPtr;
}
