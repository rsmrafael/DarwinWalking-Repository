/*
 * GoalSidePrediction.h
 *
 *  Created on: 20.06.2014
 *      Author: Oliver Krebs
 */

#ifndef GOALSIDEPREDICTION_H_
#define GOALSIDEPREDICTION_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/HeadAction.h"
#include "../../Representations/GoalSide.h"
#include "../../Representations/GoalSideView.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(GoalSidePrediction)
	REQUIRE(HeadAction,HeadAction)
	REQUIRE(GoalSide,GoalSideGoalkeeperPrediction)
	//REQUIRE(GoalSide,GoalSideBackgroundPrediction)
	//REQUIRE(GoalSide,GoalSideHistogramPrediction)
	//REQUIRE(GoalSide,GoalSideGoalieBallPrediction)
	PROVIDE(GoalSide,GoalSidePrediction)
	PROVIDE(GoalSideView,GoalSideView)
END_DECLARE_MODULE(GoalSidePrediction)

/**
 * GoalSidePrediction combines all goal side predictions and provides a weighted goal side
 */
class GoalSidePrediction
:	public GoalSidePredictionBase,
	public ConfigChangeListener
{
public:
	GoalSidePrediction();
	virtual ~GoalSidePrediction();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

	/**
	 * config has changed
	 * @see ConfigChangeListener::configChanged
	 */
	virtual void configChanged();

private:
	void addProbability( const GoalSide &prediction);

	double mAccuracy;
	double mAccuracySum;
	double mProbability;
	int mCountPredictions;
};

#endif /* GOALSIDEPREDICTION_H_ */
