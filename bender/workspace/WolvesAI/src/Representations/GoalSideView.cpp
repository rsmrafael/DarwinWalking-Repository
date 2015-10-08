/*
 * GoalSideView.cpp
 *
 *  Created on: 14.07.2014
 *      Author: Oliver Krebs
 */

#include "GoalSideView.h"
#include <stddef.h>

GoalSideView::GoalSideView()
:	mAngleStep(15)
{

}

GoalSideView::~GoalSideView()
{

}

void GoalSideView::setAngleStep(int step)
{
	if( step > 0 && step < 360) {
		mAngleStep = step;
	}
}

void GoalSideView::updateGoalSide(int angle, const GoalSide &goalSide)
{
	// round to step size
	angle /= mAngleStep;
	angle *= mAngleStep;
	mGoalSides.insert(std::pair<int, GoalSide>(angle, goalSide));
}

GoalSide GoalSideView::getGoalSide(int angle) const
{
	// round to step size
	angle /= mAngleStep;
	angle *= mAngleStep;
	GoalSideMap::const_iterator it = mGoalSides.find(angle);
	if( it != mGoalSides.end()) {
		return it->second;
	}
	return GoalSide();
}
