/*
 * RobotDetection.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef RobotDetection_H_
#define RobotDetection_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/Object.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/FieldLines.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(RobotDetection)
	REQUIRE(PointCloud,MagentaPoints)
	REQUIRE(PointCloud,CyanPoints)
	REQUIRE(PointCloud,ObstaclePoints)
	//------------------------------------
	PROVIDE(ObjectList,CyanRobots)
	PROVIDE(ObjectList,MagentaRobots)
	PROVIDE(ObjectList,Robots)
END_DECLARE_MODULE(RobotDetection)

/**
 * RobotDetection tries to detect robots
 */
class RobotDetection: public RobotDetectionBase,
public ConfigChangeListener {
public:
	RobotDetection();
	virtual ~RobotDetection();

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
	void addPointToNearestCloud( const Point &point, vector<PointCloudObject_t> &list) const;
	void eraseSmallClouds(vector<PointCloudObject_t> &cloudList) const;
	PointCloudObject_t getNearestCloudToPoint(const Point &point, vector<PointCloudObject_t> &cloudList) const;

	int mDebugDrawings;
	double mMinPointsAreaRatio;
	double mMinColorBlackRatio;
	size_t mMinCloudPoints;
	size_t mMinColorCloudPoints;
	int mMinCloudDistance;
};

#endif /* RobotDetection_H_ */
