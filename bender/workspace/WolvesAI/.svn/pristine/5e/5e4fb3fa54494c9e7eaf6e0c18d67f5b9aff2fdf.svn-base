/*
 * GoalSideGoalieBall.h
 *
 *  Created on: 21.06.2014
 *      Author: Oliver Krebs
 */

#ifndef GOALSIDEGOALIEBALL_H_
#define GOALSIDEGOALIEBALL_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/GoalSide.h"
#include "../../Representations/KickStatus.h"
#include "../../Representations/TeamMate.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(GoalSideGoalieBall)
	REQUIRE(KickStatus,KickStatus)
	REQUIRE(TeamMate,Goalie)
	PROVIDE(GoalSide,GoalSideGoalieBall)
END_DECLARE_MODULE(GoalSideGoalieBall)

/**
 * GoalSideGoalieBall tries to detect the goal side with the distance of the ball to the goalie.
 * The distance of the ball to the goalie is compared before and after a kick.
 */
class GoalSideGoalieBall
:	public GoalSideGoalieBallBase,
 	public ConfigChangeListener
{
public:
	GoalSideGoalieBall();
	virtual ~GoalSideGoalieBall();

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
	uint64_t mKickBegun;
	uint64_t mMaxDeltaTime;
	uint64_t mMinDeltaTime;
	double mGoalieBallDistance;
	double mMinDeltaDistance;
	double mMaxKickDistance;
};

#endif /* GOALSIDEGOALIEBALL_H_ */
