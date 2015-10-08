/*
 * Game.h
 *
 *  Created on: 02.07.2009
 *      Author: Stefan
 */

#ifndef GAME_H_
#define GAME_H_

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <sys/time.h>
#include "GameEventListener.h"
#include "IGame.h"

class Game : public IGame {
public:
	Game();
	virtual ~Game();

	void addGameEventListener(GameEventListener *listener);

	uint8_t getTeamID() const;
	uint8_t getBotID() const;
	uint8_t getOwnScore() const;
	uint8_t getRivalScore() const;
	int8_t getGoalDifference() const;
	timeval getKickOffTime() const;
	GameState getGameState() const;
	GameResult getGameResult() const;
	TeamColor getTeamColor() const;
	bool haveKickOff() const;
	bool isAllowedToMove() const;
	bool isPenalized() const;
	bool isFirstHalf() const;

	void setBotAllowedToMove(bool state);
	void setBotPenalized(bool state);
	void setTeamColor(TeamColor color);
	void setGameState(GameState state);
	void setScore(uint8_t ownScore, uint8_t rivalScore);
	void setKickoff(bool haveKickoff, const timeval& kickoffTime);
	void setIsFirstHalf(bool state);

	bool save();

private:
	std::vector<GameEventListener*> mListenerList;
	GameState mGameState;
	TeamColor mTeamColor;
	GameResult mGameResult;
	timeval mKickOffTime;
	uint8_t mOwnScore;
	uint8_t mRivalScore;
	int8_t mGoalDifference;
	uint8_t mTeamID;
	uint8_t mBotID;
	bool mHaveKickOff;
	bool mIsAllowedToMove;
	bool mIsPenalized;
	bool mIsFirstHalf;
};

#endif /* GAME_H_ */
