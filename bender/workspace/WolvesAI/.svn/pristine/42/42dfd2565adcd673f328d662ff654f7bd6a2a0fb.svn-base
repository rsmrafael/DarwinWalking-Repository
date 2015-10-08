/*
 * RelativePositionDetection.h
 *
 *  Created on: 06.03.2015
 *      Author: Oliver Krebs
 */

#ifndef RELATIVEPOSITIONDETECTION_H_
#define RELATIVEPOSITIONDETECTION_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/BodyStatus.h"
#include "../../Representations/CameraSettings.h"
#include "../../Representations/RobotConfiguration.h"
#include "../../Representations/Object.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/Position.h"
#include "../../Representations/STLVector.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(RelativePositionDetection)
	REQUIRE(BodyStatus, BodyStatus)
	REQUIRE(RobotConfiguration, RobotConfiguration)
	REQUIRE(CameraSettings,CameraSettings)
	REQUIRE(Object, Ball)
	REQUIRE(ObjectList, GoalPoles)
	REQUIRE(ObjectList, CyanRobots)
	REQUIRE(ObjectList, MagentaRobots)
	REQUIRE(ObjectList, Robots)
	//------------------------------------------------------
	PROVIDE(Vector, BallVector)
	PROVIDE(STLVector<Vector>, GoalPolesVectors)
	PROVIDE(STLVector<Vector>, CyanRobotsVectors)
	PROVIDE(STLVector<Vector>, MagentaRobotsVectors)
	PROVIDE(STLVector<Vector>, RobotsVectors)
END_DECLARE_MODULE(RelativePositionDetection)

/**
 * RelativePositionDetection creates the vectors of the objects relative to the robot
 */
class RelativePositionDetection : public RelativePositionDetectionBase {
public:
	RelativePositionDetection();
	virtual ~RelativePositionDetection();

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
	void updateVectors(const std::vector<Object> &objects, std::vector<Vector> *vectors);
	void updateVector(const Object &object, Vector *vec);
	void showDebugInformations(const Object &object, Vector *pos, double tilt);

	double mPanAngle;
	double mTiltAngle;

	double mRadiansPerPixelH; 	//!< radians per pixel horizontal
	double mRadiansPerPixelV; 	//!< radians per pixel vertical

	int mPrinciplePointX;		//!< principle point x
	int mPrinciplePointY;		//!< principle point y

	double mSizeToDistanceMultiplier;
	double mDistanceFromAngleWeight;
	double mDistanceFromSizeWeight;
	double mCameraHeight;
	double mCameraOffsetX;

	bool mUsePrinciplePoint;

	int mDebugDrawings;
};

#endif /* RELATIVEPOSITIONDETECTION_H_ */
