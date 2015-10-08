/*
 * GoalSideHistogram.cpp
 *
 *  Created on: 01.07.2014
 *      Author: cpupower
 */

#include "GoalSideHistogram.h"

GoalSideHistogram::GoalSideHistogram(HistogramService* histogramService)
:	mHistogramService(histogramService) {
}

GoalSideHistogram::~GoalSideHistogram() {
	mHistogramService = NULL;
}

bool GoalSideHistogram::execute() {
	using protobuf::HistogramServiceCommand;
	HistogramServiceCommand command;
	if (!mHistogramService->getCurrentCommand(&command)) {
		return true;
	}
	if (getGoalPoles().getObjects().empty()) {
		Debugger::INFO("GoalSideHistogram",
				"Refusing histogram service command, as there is no goal pole in sight!");
		return true;
	}
	switch (command.desiredaction()) {
	//
	case HistogramServiceCommand::DELETE_ALL:
		getOpponentGoalHistograms().clear();
		getOwnGoalHistograms().clear();
		break;
	case HistogramServiceCommand::SAVE_OWN:
		getOwnGoalHistograms().add(getBackgroundYUV());
		break;
	case HistogramServiceCommand::SAVE_OPPONENT:
		getOpponentGoalHistograms().add(getBackgroundYUV());
		break;
	default:
		break;
	}
	return true;
}
