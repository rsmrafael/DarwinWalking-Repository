/*
 * GoalSideBackground.cpp
 *
 *  Created on: 25.03.2014
 *      Author: Oliver Krebs
 */

#include "GoalSideBackground.h"
#include "../../Config.h"

const double COLOR_VARIANCE_MAX = 384.0; //384 = 3*128

GoalSideBackground::GoalSideBackground()
:	mMyGoalBackgroundY( 0),
 	mMyGoalBackgroundU( 0),
 	mMyGoalBackgroundV( 0),
 	mOpGoalBackgroundY( 0),
 	mOpGoalBackgroundU( 0),
 	mOpGoalBackgroundV( 0)
{
	mLastFirstHalf = true;
	Config::GetInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

GoalSideBackground::~GoalSideBackground()
{
	Config::GetInstance()->removeConfigChangeHandler(this);
}

bool GoalSideBackground::execute()
{
	if( !getGoalPoles().getObjects().empty())
	{
		int y = getBackgroundY().calculateMode();
		int u = getBackgroundU().calculateMode();
		int v = getBackgroundV().calculateMode();

		//DRAW_TEXT("Background", 10, 20, DebugDrawer::LIGHT_YELLOW, "Y: %d, U: %d, V: %d", y, u, v);
		Debugger::DEBUG("GoalSideBackground", "Y: %d, U: %d, V: %d", y, u, v);

		if( getGameStatus().gameState == IGame::READY) {
			bool firstHalf = getGameStatus().isFirstHalf;
			if( firstHalf != mLastFirstHalf) {
				Debugger::INFO("GoalSideBackground", "Change goal sides!");
				mMyGoalBackgroundY = mOpGoalBackgroundY;
				mMyGoalBackgroundU = mOpGoalBackgroundU;
				mMyGoalBackgroundV = mOpGoalBackgroundV;
				Debugger::DEBUG("GoalSideBackground", "My: %d %d %d",
						mMyGoalBackgroundY, mMyGoalBackgroundU, mMyGoalBackgroundV);
				mOpGoalBackgroundY = mMyGoalBackgroundY;
				mOpGoalBackgroundU = mMyGoalBackgroundU;
				mOpGoalBackgroundV = mMyGoalBackgroundV;
				Debugger::DEBUG("GoalSideBackground", "Op %d %d %d",
						mOpGoalBackgroundY, mOpGoalBackgroundU, mOpGoalBackgroundV);
				mLastFirstHalf = firstHalf;
			}
			int pan = getBodyStatus().getPan();
			if( pan >= -45 && pan <= +45) {
				mOpGoalBackgroundY = (3*mOpGoalBackgroundY + y) / 4;
				mOpGoalBackgroundU = (3*mOpGoalBackgroundU + u) / 4;
				mOpGoalBackgroundV = (3*mOpGoalBackgroundV + v) / 4;
				Debugger::INFO("GoalSideBackground", "OpGoal updated!");
				Debugger::DEBUG("GoalSideBackground", "OpGoal: %d %d %d",
						mOpGoalBackgroundY, mOpGoalBackgroundU, mOpGoalBackgroundV);
			} else if( pan <= -135 || pan >= +135) {
				mMyGoalBackgroundY = (3*mMyGoalBackgroundY + y) / 4;
				mMyGoalBackgroundU = (3*mMyGoalBackgroundU + u) / 4;
				mMyGoalBackgroundV = (3*mMyGoalBackgroundV + v) / 4;
				Debugger::INFO("GoalSideBackground", "MyGoal updated!");
				Debugger::DEBUG("GoalSideBackground", "MyGoal: %d %d %d",
						mMyGoalBackgroundY, mMyGoalBackgroundU, mMyGoalBackgroundV);
			}
			return true;
		}

		int varMy = abs(mMyGoalBackgroundY - y)
				+ abs(mMyGoalBackgroundU - u)
				+ abs(mMyGoalBackgroundV - v);
		int varOp = abs(mOpGoalBackgroundY - y)
				+ abs(mOpGoalBackgroundU - u)
				+ abs(mOpGoalBackgroundV - v);

		double chanceMy = 0.0;
		if( varMy < COLOR_VARIANCE_MAX) {
			chanceMy = (COLOR_VARIANCE_MAX - (double)varMy) / COLOR_VARIANCE_MAX;
		}

		double chanceOp = 0.0;
		if( varOp < COLOR_VARIANCE_MAX) {
			chanceOp = (COLOR_VARIANCE_MAX - (double)varOp) / COLOR_VARIANCE_MAX;
		}

		chanceOp = chanceOp / (chanceOp + chanceMy);
		chanceMy = chanceMy / (chanceOp + chanceMy);

		Debugger::DEBUG("GoalSideBackground", "My: %.2f (%d), Op: %.2f (%d)", chanceMy, varMy, chanceOp, varOp);

		getGoalSideBackgroundPrediction().setProbability(chanceOp);
	} else {
		getGoalSideBackgroundPrediction().setProbability(0.5);
	}

	return true;
}

void GoalSideBackground::configChanged()
{
	double accuracy = Config::GetDouble("GoalSideBackground", "Accuracy", 0.5);
	double minInterval = Config::GetDouble("GoalSideBackground", "MinInterval", 0.1);
	mMyGoalBackgroundY = Config::GetInt("GoalSideBackground", "Background_myY", 0);
	mMyGoalBackgroundU = Config::GetInt("GoalSideBackground", "Background_myU", 0);
	mMyGoalBackgroundV = Config::GetInt("GoalSideBackground", "Background_myV", 0);
	mOpGoalBackgroundY = Config::GetInt("GoalSideBackground", "Background_opY", 250);
	mOpGoalBackgroundU = Config::GetInt("GoalSideBackground", "Background_opU", 250);
	mOpGoalBackgroundV = Config::GetInt("GoalSideBackground", "Background_opV", 250);
	getGoalSideBackgroundPrediction().setMinInterval( minInterval);
	getGoalSideBackgroundPrediction().setAccuracy(accuracy);
}


