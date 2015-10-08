/*
 * HistogramService.h
 *
 *  Created on: 01.07.2014
 *      Author: da-v1n
 */

#ifndef HISTOGRAMSERVICE_H_
#define HISTOGRAMSERVICE_H_

#include "../PacketListener.h"
#include <string>
#include <vector>
#include "../../Messages/HistogramServiceCommand.pb.h"
#include "../Debugger.h"

/**
 * Provides an interface to the debugger, to allow to capture histograms on the robot
 * from the current vision image or delete all of them.
 * These are stored for the own and opponent side.
 */
class HistogramService : public PacketListener {
public:
	HistogramService();

	virtual ~HistogramService();
	virtual void newPacket(uint8_t* packet, size_t size);
	virtual std::string getPacketID();

	/**
	 * get the current command
	 * @param outCommand	the current command
	 * @return true if there was a command
	 */
	bool getCurrentCommand(protobuf::HistogramServiceCommand* outCommand);
private:
	protobuf::HistogramServiceCommand mLastCommand;
};

#endif /* HISTOGRAMSERVICE_H_ */
