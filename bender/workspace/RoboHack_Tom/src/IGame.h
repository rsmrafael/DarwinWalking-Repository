/*
 * IGame.h
 *
 *  Created on: 25.02.2014
 *      Author: Oliver
 */

#ifndef IGAME_H_
#define IGAME_H_


class IGame
{
public:
	enum TeamColor {
		CYAN,
		MAGENTA
	};

	enum GameState {
		INITIAL = 0,
		READY = 1,
		SET = 2,
		PLAYING = 3,
		FINISHED = 4
	};

	enum GameResult {
		LOSE = -1,
		DRAW = 0,
		WIN = 1
	};

	virtual ~IGame() {}

	virtual uint8_t getTeamID() const = 0;
	virtual uint8_t getBotID() const = 0;
	virtual uint8_t getOwnScore() const = 0;
	virtual uint8_t getRivalScore() const = 0;
	virtual int8_t getGoalDifference() const = 0;
	virtual timeval getKickOffTime() const = 0;
	virtual GameState getGameState() const = 0;
	virtual GameResult getGameResult() const = 0;
	virtual TeamColor getTeamColor() const = 0;
	virtual bool haveKickOff() const = 0;
	virtual bool isAllowedToMove() const = 0;
	virtual bool isPenalized() const = 0;

	virtual void setBotAllowedToMove(bool state) = 0;
	virtual void setBotPenalized(bool state) = 0;
	virtual void setTeamColor(TeamColor color) = 0;
	virtual void setGameState(GameState state) = 0;
	virtual void setScore(uint8_t ownScore, uint8_t rivalScore) = 0;
	virtual void setKickoff(bool haveKickoff, const timeval& kickoffTime) = 0;
};

#endif /* IGAME_H_ */
