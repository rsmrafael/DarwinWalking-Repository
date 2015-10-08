/*
 * GoalSideBackground.h
 *
 *  Created on: 25.03.2014
 *      Author: Oliver Krebs
 */

#ifndef GOALSIDEBACKGROUND_H_
#define GOALSIDEBACKGROUND_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/Histogram.h"
#include "../../Representations/GameStatus.h"
#include "../../Representations/BodyStatus.h"
#include "../../Representations/GoalSide.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(GoalSideBackground)
	REQUIRE(ObjectList,GoalPoles)
	REQUIRE(Histogram,BackgroundY)
	REQUIRE(Histogram,BackgroundU)
	REQUIRE(Histogram,BackgroundV)
	REQUIRE(GameStatus,GameStatus)
	REQUIRE(BodyStatus,BodyStatus)
	PROVIDE(GoalSide,GoalSideBackgroundPrediction)
END_DECLARE_MODULE(GoalSideBackground)

/**
 * GoalSideBackground compares the background color components with the given goal side backgrounds
 */
class GoalSideBackground
:	public GoalSideBackgroundBase,
	public ConfigChangeListener
{
public:
	GoalSideBackground();
	virtual ~GoalSideBackground();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

	/**
	 * called when config has changed
	 * @see ConfigChangeListener::configChanged
	 */
	virtual void configChanged();

private:
	int mMyGoalBackgroundY;
	int mMyGoalBackgroundU;
	int mMyGoalBackgroundV;

	int mOpGoalBackgroundY;
	int mOpGoalBackgroundU;
	int mOpGoalBackgroundV;

	bool mLastFirstHalf;
};


#endif /* GOALSIDEBACKGROUND_H_ */