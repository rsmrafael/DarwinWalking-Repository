/*
 * GameEventListener.h
 *
 *  Created on: 25.02.2014
 *      Author: Oliver Krebs
 */

#ifndef GAMEEVENTLISTENER_H_
#define GAMEEVENTLISTENER_H_

#include "IGame.h"

class GameEventListener
{
public:
	GameEventListener() {}
	virtual ~GameEventListener() {}

	virtual void isPenalizedHasChanged(bool penalized) = 0;
	virtual void gameResultHasChanged(IGame::GameResult result) = 0;
	virtual void gameStateHasChanged(IGame::GameState state) = 0;
	virtual void kickOffHasChanged(bool haveKickOff) = 0;
};

#endif /* GAMEEVENTLISTENER_H_ */
