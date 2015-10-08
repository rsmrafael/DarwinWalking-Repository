/*
 * GoalSidePredictor_MD.h
 *
 *  Created on: 04.04.2014
 *      Author: Stefan Krupop
 */

#ifndef GOALSIDEPREDICTOR_MD_H_
#define GOALSIDEPREDICTOR_MD_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/BodyStatus.h"
#include "../../Representations/GameStatus.h"
#include "../../Representations/GoalSide.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(GoalSidePredictor_MD)
	REQUIRE(ObjectList,RemovedBallCandidates)
	REQUIRE(BodyStatus,BodyStatus)
	REQUIRE(GameStatus,GameStatus)
	PROVIDE(GoalSide,GoalSideMDPrediction)
END_DECLARE_MODULE(GoalSidePredictor_MD)

/**
 * GoalSidePredictor_MD was a goal side prediction for Magdeburg.
 * There was a red marker on the one side of the field (a possible ball, but not in the field)
 */
class GoalSidePredictor_MD
:	public GoalSidePredictor_MDBase,
	public ConfigChangeListener
{
public:
	GoalSidePredictor_MD();
	virtual ~GoalSidePredictor_MD();

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
	double mDecayValue;
	int mMinRemovedBallWidth;
	int mMinPanAngle;
	int mMaxPanAngle;
	double mConfidence;
	bool mObjectsOnRightInFirstHalf;
	double mAspectRatioRange;

	double mProbability;
};


#endif /* GOALSIDEPREDICTOR_MD_H_ */
