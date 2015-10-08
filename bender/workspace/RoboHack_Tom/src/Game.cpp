/*
 * Game.cpp
 *
 *  Created on: 02.07.2009
 *      Author: Stefan
 */
#include <string>
#include <algorithm>
#include <cctype>

#include "Game.h"
#include "Debugging/Debugger.h"
#include "Config.h"

using namespace std;

Game::Game()
:	mGameState( Game::PLAYING),
	mTeamColor( Game::CYAN),
	mGameResult( Game::DRAW),
	mOwnScore( 0),
	mRivalScore( 0),
	mGoalDifference( 0),
	mTeamID( 23),
	mBotID( 1),
	mHaveKickOff( false),
	mIsAllowedToMove( true),
	mIsPenalized( false),
	mIsFirstHalf(true)
{
	//mTeamID = Config::GetValue<uint8_t>("Game", "teamID", 22);
	//mBotID = Config::GetValue<uint8_t>("Game", "botID", 1);
	mTeamID = (uint8_t)Config::GetInt("Game", "teamID", 22);
	mBotID = (uint8_t)Config::GetInt("Game", "botID", 1);
	string color = Config::GetString("Game", "teamColor", "CYAN");
	//lint --e{864}
	transform(color.begin(), color.end(), color.begin(), ::toupper);
	mTeamColor = color == "MAGENTA" ? Game::MAGENTA : Game::CYAN;

	mKickOffTime.tv_sec = 0;
	mKickOffTime.tv_usec = 0;
}

Game::~Game()
{
	mListenerList.clear();
}

uint8_t Game::getTeamID(void) const {
	return mTeamID;
}

uint8_t Game::getBotID(void) const {
	return mBotID;
}

Game::GameState Game::getGameState(void) const {
	return mGameState;
}

Game::GameResult Game::getGameResult(void) const {
	return mGameResult;
}

bool Game::isAllowedToMove(void) const {
	return mIsAllowedToMove;
}

bool Game::isPenalized(void) const {
	return mIsPenalized;
}

bool Game::isFirstHalf(void) const {
	return mIsFirstHalf;
}

Game::TeamColor Game::getTeamColor(void) const {
	return mTeamColor;
}

uint8_t Game::getOwnScore(void) const {
	return mOwnScore;
}

uint8_t Game::getRivalScore(void) const {
	return mRivalScore;
}

int8_t Game::getGoalDifference(void) const {
	return mGoalDifference;
}

void Game::setBotAllowedToMove(bool state) {
	mIsAllowedToMove = state;
	if (state) {
		Debugger::INFO("Game", "Bot is now allowed to move");
	} else {
		Debugger::WARN("Game", "Bot is now disallowed to move");
	}
}

void Game::setBotPenalized(bool state) {
	//lint --e{731}
	if( (mIsPenalized != state)) {
		mIsPenalized = state;
		if (state) {
			Debugger::WARN("Game", "Bot is now penalized");
		} else {
			Debugger::INFO("Game", "Bot is now un-penalized");
		}
		std::vector<GameEventListener*>::const_iterator it;
		for( it = mListenerList.begin(); it != mListenerList.end(); ++it) {
			(*it)->isPenalizedHasChanged(mIsPenalized);
		}
	}
}

void Game::setTeamColor(TeamColor color) {
	mTeamColor = color;
	Debugger::WARN("Game", "Now playing as team %s", mTeamColor == Game::CYAN ? "CYAN" : "MAGENTA");
}

void Game::setGameState(GameState state) {
	if( mGameState != state) {
		mGameState = state;
		std::vector<GameEventListener*>::const_iterator it;
		for( it = mListenerList.begin(); it != mListenerList.end(); ++it) {
			(*it)->gameStateHasChanged(mGameState);
		}
		Debugger::DEBUG("Game", "GameState changed to: %d", mGameState);
	}
}

void Game::setIsFirstHalf(bool state) {
	if( mIsFirstHalf != state) {
		mIsFirstHalf = state;
		if( mIsFirstHalf) {
			Debugger::INFO("Game", "First half");
		} else {
			Debugger::INFO("Game", "Second half");
		}
	}
}

void Game::setScore(uint8_t ownScore, uint8_t rivalScore) {
	// score has changed
	if( (mOwnScore != ownScore) || (mRivalScore != rivalScore)) {
		bool gameResultChanged = false;
		mOwnScore = ownScore;
		mRivalScore = rivalScore;
		mGoalDifference = (int8_t)mOwnScore - (int8_t)mRivalScore;
		Debugger::DEBUG("Game", "Score has changed to: %d : %d (%d)", mOwnScore, mRivalScore, mGoalDifference);
		if( mGoalDifference > 0) {
			if( mGameResult != Game::WIN) {
				mGameResult = Game::WIN;
				gameResultChanged = true;
				Debugger::INFO("Game", "GameResult has changed to: WIN");
			}
		} else if( mGoalDifference < 0) {
			if( mGameResult != Game::LOSE) {
				mGameResult = Game::LOSE;
				gameResultChanged = true;
				Debugger::INFO("Game", "GameResult has changed to: LOSE");
			}
		} else {
			if( mGameResult != Game::DRAW) {
				mGameResult = Game::DRAW;
				gameResultChanged = true;
				Debugger::INFO("Game", "GameResult has changed to: DRAW");
			}
		}
		if( gameResultChanged) {
			std::vector<GameEventListener*>::const_iterator it;
			for( it = mListenerList.begin(); it != mListenerList.end(); ++it) {
				(*it)->gameResultHasChanged(mGameResult);
			}
		}
	}
}

void Game::setKickoff(bool haveKickoff, const timeval& kickoffTime) {
	//lint --e{731}
	if( (mHaveKickOff != haveKickoff)) {
		mHaveKickOff = haveKickoff;
		mKickOffTime = kickoffTime;
		if( mHaveKickOff) {
			Debugger::INFO("Game", "We have KickOff!");
		} else {
			Debugger::INFO("Game", "Other team has KickOff!");
		}
		std::vector<GameEventListener*>::const_iterator it;
		for( it = mListenerList.begin(); it != mListenerList.end(); ++it) {
			(*it)->kickOffHasChanged(mHaveKickOff);
		}
	}
}

bool Game::haveKickOff(void) const {
	return mHaveKickOff;
}

timeval Game::getKickOffTime(void) const {
	return mKickOffTime;
}

bool Game::save() {
	Config::SetInt("Game", "botID", mBotID);
	Config::SetString("Game", "teamColor", mTeamColor == Game::CYAN ? "CYAN" : "MAGENTA");
	Config::GetInstance()->saveConfig();
	return true;
}

void Game::addGameEventListener(GameEventListener *listener)
{
	mListenerList.push_back(listener);
}
