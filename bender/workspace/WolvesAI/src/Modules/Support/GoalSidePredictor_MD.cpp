/*
 * GoalSidePredictor_MD.cpp
 *
 *  Created on: 04.04.2014
 *      Author: Stefan Krupop
 */

#include "GoalSidePredictor_MD.h"
#include "../../Config.h"
#include "../../Debugging/DebugDrawer.h"


GoalSidePredictor_MD::GoalSidePredictor_MD() : mDecayValue(0.01), mMinRemovedBallWidth(30), mMinPanAngle(10), mMaxPanAngle(110), mConfidence(0.7), mObjectsOnRightInFirstHalf(true), mAspectRatioRange(0.2) {
	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
	mProbability = 0.5;
}

GoalSidePredictor_MD::~GoalSidePredictor_MD() {
	Config::getInstance()->removeConfigChangeHandler(this);
}

bool GoalSidePredictor_MD::execute() {
	bool found = false;

	if (!getRemovedBallCandidates().getObjects().empty()) {
		const vector<Object>& balls = getRemovedBallCandidates().getObjects();
		for (vector<Object>::const_iterator it = balls.begin(); it != balls.end(); ++it) {
			if ((*it).getBoundingBox().width >= mMinRemovedBallWidth) {
				int pan = getBodyStatus().getPan();
				if (abs(pan) >= mMinPanAngle && abs(pan) <= mMaxPanAngle) {
					double aspect = (double)(*it).getBoundingBox().width / (double)(*it).getBoundingBox().height;
					if (1.0 - mAspectRatioRange <= aspect && aspect <= 1.0 + mAspectRatioRange) {
						//Debugger::DEBUG("GoalSidePredictor_MD", "Detected valid orientation object at pan=%d", pan);

						double myPercent = 0;
						double opPercent = 0;
						if (pan > 0) {
							if (mObjectsOnRightInFirstHalf) {
								opPercent = mConfidence;
							} else {
								myPercent = mConfidence;
							}
						} else {
							if (mObjectsOnRightInFirstHalf) {
								myPercent = mConfidence;
							} else {
								opPercent = mConfidence;
							}
						}
						if (!getGameStatus().isFirstHalf) {
							double tmp = myPercent;
							myPercent = opPercent;
							opPercent = tmp;
						}

						if (myPercent != 0.0) {
							mProbability -= myPercent;
						} else {
							mProbability += opPercent;
						}
						if (mProbability < 0) {
							mProbability = 0;
						}
						if (mProbability > 1) {
							mProbability = 1;
						}
#ifdef _DEBUG
						Debugger::DEBUG("GoalSidePredictor_MD", "Object votes for %s goal, new probability=%f", myPercent != 0.0 ? "my" : "opponent", mProbability);
						DRAW_BOX("GoalSidePredictor_MD", (*it).getBoundingBox().topLeft.getX(), (*it).getBoundingBox().topLeft.getY(), (*it).getBoundingBox().width, (*it).getBoundingBox().height, DebugDrawer::RED);
#endif
						found = true;
					} else {
#ifdef _DEBUG
						DRAW_BOX("GoalSidePredictor_MD", (*it).getBoundingBox().topLeft.getX(), (*it).getBoundingBox().topLeft.getY(), (*it).getBoundingBox().width, (*it).getBoundingBox().height, DebugDrawer::LIGHT_GRAY);
						Debugger::DEBUG("GoalSidePredictor_MD", "Object ignored, aspect ratio=%f", aspect);
#endif
					}
				}
			}
		}
	}
	if (!found) {
		if (mProbability != 0.5) {
			if (mProbability > 0.5) {
				mProbability -= mDecayValue;
			} else {
				mProbability += mDecayValue;
			}
#ifdef _DEBUG
			Debugger::DEBUG("GoalSidePredictor_MD", "No orientation objects seen, new probability=%f", mProbability);
#endif
		}
	}
	getGoalSideMDPrediction().setProbability(mProbability);

	return true;
}

void GoalSidePredictor_MD::configChanged() {
	ConfigFile *config = Config::getInstance();

	mDecayValue = config->get<double>("GoalSidePredictor_MD", "DecayValue", 0.005,
			"Amount the current probability decays per iteration where no target objects are seen");
	mMinRemovedBallWidth = config->get<int>("GoalSidePredictor_MD", "MinRemovedBallWidth", 30,
			"Amount the current probability decays per iteration where no target objects are seen");
	mMinPanAngle = config->get<int>("GoalSidePredictor_MD", "MinPanAngle", 10,
			"Minimum pan angle in either direction to be valid");
	mMaxPanAngle = config->get<int>("GoalSidePredictor_MD", "MaxPanAngle", 110,
			"Maximum pan angle in either direction to be valid");
	mConfidence = config->get<double>("GoalSidePredictor_MD", "Confidence", 0.1,
			"Amount the current probability is increased by each target object seen");
	mObjectsOnRightInFirstHalf = config->get<bool>("GoalSidePredictor_MD", "ObjectsOnRightInFirstHalf", true,
			"True when objects are on right side of robots when looking towards opponent goal");
	mAspectRatioRange = config->get<double>("GoalSidePredictor_MD", "AspectRatioRange", 1.2,
			"Range of aspect ratio for valid objects");
}
