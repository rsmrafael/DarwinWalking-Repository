/*
 * GoalDetection.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef GoalDetection_H_
#define GoalDetection_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/PointCloud.h"
#include "../../Representations/PointCloudList.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/FieldLines.h"
#include "../../Representations/BodyStatus.h"
#include "../../Representations/RobotConfiguration.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(GoalDetection)
	REQUIRE(YUVImage,Image)
	REQUIRE(FieldLines,FieldLines)
	REQUIRE(PointCloud, GoalPoints)
	REQUIRE(BodyStatus, BodyStatus)
	REQUIRE(RobotConfiguration, RobotConfiguration)
	//PROVIDE(Object, GoalPoleLeft)
	//PROVIDE(Object, GoalPoleRight)
	//PROVIDE(Object, GoalCrossbar)
	PROVIDE(ObjectList, GoalPoles)
END_DECLARE_MODULE(GoalDetection)

class GoalDetection: public GoalDetectionBase,
public ConfigChangeListener {
public:
	GoalDetection();
	virtual ~GoalDetection();

	virtual bool execute();
	virtual void configChanged();

private:
	bool findCrossbar( int x, int y, int dirX) const;
	bool filterFieldLines(const BoundingBox &box) const;
	inline void drawDebugDrawings(Object obj, double len, double tilt, double pan);

	double mMinPointsAreaRatio;
	double mMinHeightRatio;
	double mMaxHeightRatio;
	size_t mMinCloudPoints;
	int mMaxErrorY;
	int mMinPoleWidth;
	int mMinPoleHeight;
	int mMinCloudDistance;
	bool mIsFilterFieldLine;
	int mDebugDrawings;

	int mMinY;
	int mMaxU;
	int mMinV;
	int mMaxV;
};

#endif /* GoalDetection_H_ */