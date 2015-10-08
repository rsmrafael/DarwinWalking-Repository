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
	Config::GetInstance()->registerConfigChangeHandler(this);
	this->configChanged();
	mProbability = 0.5;
}

GoalSidePredictor_MD::~GoalSidePredictor_MD() {
	Config::GetInstance()->removeConfigChangeHandler(this);
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

						Debugger::DEBUG("GoalSidePredictor_MD", "Object votes for %s goal, new probability=%f", myPercent != 0.0 ? "my" : "opponent", mProbability);
						DRAW_BOX("GoalSidePredictor_MD", (*it).getBoundingBox().topLeft.getX(), (*it).getBoundingBox().topLeft.getY(), (*it).getBoundingBox().width, (*it).getBoundingBox().height, DebugDrawer::RED);
						found = true;
					} else {
						//DRAW_BOX("GoalSidePredictor_MD", (*it).getBoundingBox().topLeft.getX(), (*it).getBoundingBox().topLeft.getY(), (*it).getBoundingBox().width, (*it).getBoundingBox().height, DebugDrawer::LIGHT_GRAY);
						Debugger::DEBUG("GoalSidePredictor_MD", "Object ignored, aspect ratio=%f", aspect);
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
			//Debugger::DEBUG("GoalSidePredictor_MD", "No orientation objects seen, new probability=%f", mProbability);
		}
	}
	getGoalSideMDPrediction().setProbability(mProbability);

	return true;
}

void GoalSidePredictor_MD::configChanged() {
	mDecayValue = Config::GetDouble("GoalSidePredictor_MD", "DecayValue", 0.005, "Amount the current probability decays per iteration where no target objects are seen");
	mMinRemovedBallWidth = Config::GetInt("GoalSidePredictor_MD", "MinRemovedBallWidth", 30, "Amount the current probability decays per iteration where no target objects are seen");
	mMinPanAngle = Config::GetInt("GoalSidePredictor_MD", "MinPanAngle", 10, "Minimum pan angle in either direction to be valid");
	mMaxPanAngle = Config::GetInt("GoalSidePredictor_MD", "MaxPanAngle", 110, "Maximum pan angle in either direction to be valid");
	mConfidence = Config::GetDouble("GoalSidePredictor_MD", "Confidence", 0.1, "Amount the current probability is increased by each target object seen");
	mObjectsOnRightInFirstHalf = Config::GetBoolean("GoalSidePredictor_MD", "ObjectsOnRightInFirstHalf", true, "True when objects are on right side of robots when looking towards opponent goal");
	mAspectRatioRange = Config::GetDouble("GoalSidePredictor_MD", "AspectRatioRange", 1.2, "Range of aspect ratio for valid objects");
}
