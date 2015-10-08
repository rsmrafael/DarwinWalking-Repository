/*
 * GoalieDetection.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef GoalieDetection_H_
#define GoalieDetection_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/PointCloud.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/FieldLines.h"
#include "../../ConfigChangeListener.h"
#include "../../Game.h"

BEGIN_DECLARE_MODULE(GoalieDetection)
	REQUIRE(ObjectList, GoalPoles)
	REQUIRE(PointCloud, MagentaPoints)
	REQUIRE(PointCloud, CyanPoints)
	REQUIRE(FieldLines, FieldLines)
	REQUIRE(YUVImage,Image)
	PROVIDE(Object, GoalKeeper)
END_DECLARE_MODULE(GoalieDetection)

/**
 * GoalieDetection tries to detect the goal keeper
 */
class GoalieDetection: public GoalieDetectionBase,
public ConfigChangeListener {
public:
	GoalieDetection();
	virtual ~GoalieDetection();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

	/**
	 * config has changed
	 * @see ConfigChangeListener::configChanged
	 */
	virtual void configChanged();

private:
	void cleanUpList(vector<PointCloudObject_t> &cloudList) {
		for(auto obj : cloudList) {
			delete obj.cloud;
		}
	}
	bool detectGoalieByColor(BoundingBox goal, vector<PointCloudObject_t> &cloudList, int color);
	bool checkForColor(YUVImage::YUVData color);
	bool checkWholeFieldLine(int startX, int endX);
	bool checkFieldLineForFeet(int startX, int endX);
	void addPointToNearestCloud( const Point &point, vector<PointCloudObject_t> &list) const;
	void eraseSmallClouds(vector<PointCloudObject_t> &cloudList) const;

	int mDebugDrawings;
	int mGoalieDetectionEnabled;
	size_t mMinColorCloudPoints;
	int mMinCloudDistance;
	int mSteps;
	float mMinRobotPointsPercentage;
	float mRobotFeetWidthPercentage;
	int mMaxY_Black;
	int mMaxV_Gray;
	int mMinU_Cyan;
	int mMaxV_Cyan;
	int mMinU_Magenta;
	int mMinV_Magenta;

};

#endif /* GoalieDetection_H_ */
