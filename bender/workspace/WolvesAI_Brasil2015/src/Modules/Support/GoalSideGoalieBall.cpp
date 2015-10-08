/*
 * GoalSideGoalieBall.cpp
 *
 *  Created on: 21.06.2014
 *      Author: Oliver Krebs
 */

#include "GoalSideGoalieBall.h"
#include "../../Config.h"
#include <cmath>

GoalSideGoalieBall::GoalSideGoalieBall()
:	mKickBegun(0),
 	mMaxDeltaTime(100),
	mMinDeltaTime(200),
	mGoalieBallDistance(0),
	mMinDeltaDistance(150.0),
	mMaxKickDistance(500.0)
{
	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

GoalSideGoalieBall::~GoalSideGoalieBall()
{
	Config::getInstance()->removeConfigChangeHandler(this);
}

bool GoalSideGoalieBall::execute()
{
	// cancel if goalie is not connected
	if( !getGoalie().isConnected()) {
		getGoalSideGoalieBall().setProbability(0.5);
		return true;
	}

	uint64_t currentTime = getCurrentTime();
	if( mKickBegun == 0) {
		uint64_t maxTime = getKickStatus().getKickingTime() + mMaxDeltaTime;
		if( currentTime < maxTime) {
			mKickBegun = currentTime;
			int32_t x = getGoalie().getValue(BALL_RELATIVE_X);
			int32_t y = getGoalie().getValue(BALL_RELATIVE_Y);
			mGoalieBallDistance = sqrt((double)(x * x) + (y * y));
		}
	} else {
		uint64_t minTime = mKickBegun + mMinDeltaTime;
		if( currentTime > minTime) {
			int32_t x = getGoalie().getValue(BALL_RELATIVE_X);
			int32_t y = getGoalie().getValue(BALL_RELATIVE_Y);
			double dist = sqrt((double)(x * x) + (y * y)) - mGoalieBallDistance;
			double prob = 0.5;
			if( abs(dist) > mMinDeltaDistance) {
				prob += dist / mMaxKickDistance;
			}
			getGoalSideGoalieBall().setProbability(prob);
#ifdef _DEBUG
			Debugger::DEBUG("GoalSideGoalieBall", "Prob: %.4f (%.1fmm)", prob, dist);
#endif
			mKickBegun = 0;
		}
	}
	return true;
}

void GoalSideGoalieBall::configChanged()
{
	ConfigFile *config = Config::getInstance();
	mMaxKickDistance = 2.0 * config->get<double>("Robot", "kickDistMax", 300.0);
	mMaxDeltaTime = config->get<uint64_t>("GoalSideGoalieBall", "maxDeltaTime", 100L);
	mMinDeltaTime = config->get<uint64_t>("GoalSideGoalieBall", "minDeltaTime", 200L);
	mMinDeltaDistance = config->get<double>("GoalSideGoalieBall", "minDeltaDistance", 150.0);
	double accuracy = config->get<double>("GoalSideGoalieBall", "Accuracy", 0.8);
	getGoalSideGoalieBall().setAccuracy(accuracy);
}
