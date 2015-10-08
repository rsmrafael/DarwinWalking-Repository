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

/**
 * Representation for goal sides in relation to angle
 */
class GoalSideView {
public:

	/**
	 * Constructor
	 */
	GoalSideView();

	/**
	 * Destructor
	 */
	virtual ~GoalSideView();

	/**
	 * sets angle step size in degree
	 * @param step		step size in degree
	 */
	void setAngleStep(int step);

	/**
	 * updates goalSide to given angle in degree
	 * @param angle		angle in degree
	 * @param goalSide	goal side to angle
	 */
	void updateGoalSide(int angle, const GoalSide &goalSide);

	/**
	 * returns goalSide to given angle in degree
	 * @param angle		angle int degree
	 * @return goal side to given angle
	 */
	GoalSide getGoalSide(int angle) const;

private:
	typedef std::map<int, GoalSide> GoalSideMap;

	int mAngleStep;			//!< step size for angles
	GoalSideMap mGoalSides;	//!< goal sides in relation to angle
};

#endif /* GOALSIDEVIEW_H_ */
