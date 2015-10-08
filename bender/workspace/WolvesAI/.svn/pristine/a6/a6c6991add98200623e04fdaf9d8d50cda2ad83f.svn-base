/*
 * GoalieDetection.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#include "GoalieDetection.h"
#include "../../Debugging/Debugger.h"
#include "../../Debugging/DebugDrawer.h"
#include "../../Config.h"

GoalieDetection::GoalieDetection() {
	this->configChanged();

	Config::GetInstance()->registerConfigChangeHandler(this);
}

GoalieDetection::~GoalieDetection() {
	Config::GetInstance()->removeConfigChangeHandler(this);
}

bool GoalieDetection::execute() {

	return true;
}

void GoalieDetection::configChanged() {
	//Debugger::INFO("GoalieDetection", "config changed");

}
