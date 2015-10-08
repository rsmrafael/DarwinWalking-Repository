/*
 * GoalSideHistogram.h
 *
 *  Created on: 01.07.2014
 *      Author: cpupower
 */

#ifndef GOALSIDEHISTOGRAM_H_
#define GOALSIDEHISTOGRAM_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/Histogram.h"
#include "../../Representations/ObjectList.h"
//#include "../../Representations/STLVector.h"
#include "../../Representations/HistogramList.h"
#include "../../Messages/HistogramServiceCommand.pb.h"
#include "../../Debugging/Services/HistogramService.h"

BEGIN_DECLARE_MODULE(GoalSideHistogram)
	REQUIRE(ObjectList,GoalPoles)
	REQUIRE(Histogram,BackgroundYUV)
	//PROVIDE(STLVector<Histogram>,OpponentGoalHistograms)
	//PROVIDE(STLVector<Histogram>,OwnGoalHistograms)
	PROVIDE(HistogramList,OpponentGoalHistograms)
	PROVIDE(HistogramList,OwnGoalHistograms)
END_DECLARE_MODULE(GoalSideHistogram)

class GoalSideHistogram : public GoalSideHistogramBase {
public:
	GoalSideHistogram(HistogramService* histogramService);
	virtual ~GoalSideHistogram();

	virtual bool execute();
private:
	HistogramService* mHistogramService;
};

#endif /* GOALSIDEHISTOGRAM_H_ */
