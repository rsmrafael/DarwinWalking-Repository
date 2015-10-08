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

/**
 * Bot Control Service
 */
class BotControlService : public PacketListener {
public:
	/**
	 * Constructor
	 * @param game
	 */
	BotControlService(Game* game);
	virtual ~BotControlService();

	/**
	 * GET
	 *  CONFI: get config
	 * SET
	 *  CONFI
	 *   SAVE: save config
	 *   {0};{1};{2}: change config value of {1} in section {0} to {2}
	 */
	virtual void newPacket(uint8_t* packet, size_t size);
	virtual std::string getPacketID();

private:
	static void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);

	Game* mGame;	//!< game
};

#endif /* CONTROLSERVICE_H_ */
