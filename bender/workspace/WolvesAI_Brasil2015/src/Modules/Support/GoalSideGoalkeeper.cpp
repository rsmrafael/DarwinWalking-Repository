/*
 * GoalSideGoalkeeper.cpp
 *
 *  Created on: 23.04.2015
 *      Author: Oliver
 */

#include "GoalSideGoalkeeper.h"
#include "../../Config.h"

GoalSideGoalkeeper::GoalSideGoalkeeper()
{
	Config::getInstance()->registerConfigChangeHandler(this);
	configChanged();
}

GoalSideGoalkeeper::~GoalSideGoalkeeper()
{
	Config::getInstance()->removeConfigChangeHandler(this);
}

bool GoalSideGoalkeeper::execute()
{
	if(!getGoalKeeper().lastImageSeen) {
		getGoalSideGoalkeeperPrediction().setProbability(0.5);
		return true;
	}
	Object::ObjectType type = getGoalKeeper().type;
	if(type == Object::ROBOT_CYAN) {
		if(mTeamColor == "CYAN") {
			getGoalSideGoalkeeperPrediction().setProbability(0.0);
		} else {
			getGoalSideGoalkeeperPrediction().setProbability(1.0);
		}
	} else if (type == Object::ROBOT_MAGENTA) {
		if (mTeamColor == "MAGENTA") {
			getGoalSideGoalkeeperPrediction().setProbability(0.0);
		} else {
			getGoalSideGoalkeeperPrediction().setProbability(1.0);
		}
	} else {
		getGoalSideGoalkeeperPrediction().setProbability(0.5);
	}
	return true;
}

void GoalSideGoalkeeper::configChanged()
{
	ConfigFile *config = Config::getInstance();
	mTeamColor = config->getString("Game", "teamColor", "CYAN");
	double accuracy = config->get<double>("GoalSideGoalkeeper", "Accuracy", 0.5);
	getGoalSideGoalkeeperPrediction().setAccuracy(accuracy);
}
