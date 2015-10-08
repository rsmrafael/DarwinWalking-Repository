/*
 * BlackboardService.h
 *
 *  Created on: 14.08.2012
 *      Author: Stefan
 */

#ifndef BLACKBOARDSERVICE_H_
#define BLACKBOARDSERVICE_H_

#include "../PacketListener.h"
#include <vector>
#include <string>

/**
 * Blackboard Service
 */
class BlackboardService : public PacketListener {
public:
	BlackboardService();
	virtual ~BlackboardService();

	/**
	 * send partial blackboard
	 */
	void sendPartialBlackboard();

	/**
	 * GET
	 *  ALL: get full serialized blackboard
	 *  {a}[,{b}]: get these representations of blackboard
	 * SET
	 * LST: get serialized blackboard
	 */
	virtual void newPacket(uint8_t* packet, size_t size);
	virtual std::string getPacketID();

private:
	static void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);

	class BlackboardBroadcast {
	public:
		BlackboardBroadcast(std::string name_, int repeatCycles_)
		:	name(name_),
			repeatCycles(repeatCycles_),
			remainingCycles(repeatCycles_) {
		}

		std::string name;
		int repeatCycles;
		int remainingCycles;
	};

	std::vector<BlackboardBroadcast> mBroadcasts;
};

#endif /* BLACKBOARDSERVICE_H_ */
