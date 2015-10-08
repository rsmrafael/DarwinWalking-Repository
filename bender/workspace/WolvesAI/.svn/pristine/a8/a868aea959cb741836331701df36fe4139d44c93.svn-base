/*
 * GoalSideView.h
 *
 *  Created on: 14.07.2014
 *      Author: Oliver Krebs
 */

#ifndef GOALSIDEVIEW_H_
#define GOALSIDEVIEW_H_

#include "GoalSide.h"
#include <map>

class GoalSideView {
public:
	GoalSideView();
	virtual ~GoalSideView();

	/**
	 * sets angle step size in degree
	 */
	void setAngleStep(int step);

	/**
	 * updates goalSide to given angle in degree
	 */
	void updateGoalSide(int angle, const GoalSide &goalSide);

	/**
	 * returns goalSide to given angle in degree
	 */
	GoalSide getGoalSide(int angle) const;

private:
	typedef std::map<int, GoalSide> GoalSideMap;

	int mAngleStep;
	GoalSideMap mGoalSides;
};

#endif /* GOALSIDEVIEW_H_ */
