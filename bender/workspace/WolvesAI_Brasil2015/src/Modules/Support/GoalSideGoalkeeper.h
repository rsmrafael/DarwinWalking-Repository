/*
 * GoalSideGoalkeeper.h
 *
 *  Created on: 23.04.2015
 *      Author: Oliver
 */

#ifndef GOALSIDEGOALKEEPER_H_
#define GOALSIDEGOALKEEPER_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/Object.h"
#include "../../Representations/GoalSide.h"
#include "../../IGame.h"
#include "../../ConfigChangeListener.h"
#include <string>

BEGIN_DECLARE_MODULE(GoalSideGoalkeeper)
	REQUIRE(Object,GoalKeeper)
	PROVIDE(GoalSide,GoalSideGoalkeeperPrediction)
END_DECLARE_MODULE(GoalSideGoalkeeper)

class GoalSideGoalkeeper
:	public GoalSideGoalkeeperBase,
	public ConfigChangeListener
{
  public:
	/**
	 * Constructor
	 */
	GoalSideGoalkeeper();

	/**
	 * Destructor
	 */
  	virtual ~GoalSideGoalkeeper();

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
  	std::string mTeamColor;
};


#endif /* GOALSIDEGOALKEEPER_H_ */
