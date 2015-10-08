/*
 * GameStatus.h
 *
 *  Created on: 27.06.2013
 *      Author: Stefan Krupop
 */

#ifndef GAMESTATUS_H_
#define GAMESTATUS_H_

//lint -e1711

#include <string>
#include <sys/time.h>
#include "../Game.h"

using namespace std;

/**
 * Representation for status of game
 */
class GameStatus {
public:

	/**
	 * Constructor
	 */
	GameStatus();

	/**
	 * Destructor
	 */
	virtual ~GameStatus();

	bool haveKickOff;			//!< own team has kick off
	timeval kickOffTime;		//!< time of kick off
	Game::GameState gameState;	//!< current game state
	bool isFirstHalf;			//!< playing in first half
private:
};

#endif /* VISIONSETTINGS_H_ */
