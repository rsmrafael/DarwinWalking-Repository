/*
 * HistogramService.cpp
 *
 *  Created on: 01.07.2014
 *      Author: da-v1n
 */

#include "HistogramService.h"

HistogramService::HistogramService() {

}

HistogramService::~HistogramService() {
}

void HistogramService::newPacket(uint8_t* packet, size_t size) {
	using protobuf::HistogramServiceCommand;
	HistogramServiceCommand command;
	command.ParseFromArray(packet, size);

	switch (command.desiredaction()) {
	case HistogramServiceCommand::SAVE_OPPONENT:
		Debugger::INFO("HistogramService", "SaveOpponent");
		break;
	case HistogramServiceCommand::SAVE_OWN:
		Debugger::INFO("HistogramService", "SaveOwn");
		break;
	case HistogramServiceCommand::DELETE_ALL:
		Debugger::INFO("HistogramService", "DeleteAll");
		break;
	}

	mLastCommand = command;
}

std::string HistogramService::getPacketID() {
	return "HIS";
}

bool HistogramService::getCurrentCommand(
		protobuf::HistogramServiceCommand* outCommand) {
	if (mLastCommand.IsInitialized()) {
		*outCommand = mLastCommand;
		mLastCommand.Clear();
		return true;
	} else {
		return false;
	}
}
