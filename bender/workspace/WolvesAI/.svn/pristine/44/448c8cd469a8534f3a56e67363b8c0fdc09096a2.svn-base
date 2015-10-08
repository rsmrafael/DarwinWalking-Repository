/*
 * ControlService.h
 *
 *  Created on: 08.06.2010
 *      Author: Stefan
 */

#ifndef CONTROLSERVICE_H_
#define CONTROLSERVICE_H_

#include <string>
#include <vector>
#include "../PacketListener.h"
#include "../../Game.h"

class BotControlService : public PacketListener {
public:
	BotControlService(Game* game);
	virtual ~BotControlService();
	virtual void newPacket(uint8_t* packet, size_t size);
	virtual std::string getPacketID();

private:
	static void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);

	Game* mGame;
};

#endif /* CONTROLSERVICE_H_ */
