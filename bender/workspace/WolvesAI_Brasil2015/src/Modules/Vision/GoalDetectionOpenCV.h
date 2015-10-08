/*
 * GoalDetectionOpenCV.h
 *
 *  Created on: 20.02.2015
 *      Author: Oliver Krebs
 */

#ifndef GOALDETECTIONOPENCV_H_
#define GOALDETECTIONOPENCV_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/STLVector.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/OpenCVImage.h"
#include "../../Representations/PointCloud.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/FieldLines.h"
#include "../../Representations/BodyStatus.h"
#include "../../Representations/RobotConfiguration.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(GoalDetectionOpenCV)
	REQUIRE(OpenCVImage, OpenCVImage)
	REQUIRE(FieldLines, FieldLines)
	REQUIRE(YUVImage, Image);
	//REQUIRE(PointCloud, GoalPoints)
	//REQUIRE(BodyStatus, BodyStatus)
	//REQUIRE(RobotConfiguration, RobotConfiguration)
	//PROVIDE(Object, GoalPoleLeft)
	//PROVIDE(Object, GoalPoleRight)
	//PROVIDE(Object, GoalCrossbar)
	PROVIDE(ObjectList, GoalPoles)
END_DECLARE_MODULE(GoalDetectionOpenCV)

/**
 * GoalDetection tries to detect the goal with OpenCV
 */
class GoalDetectionOpenCV : public GoalDetectionOpenCVBase, public ConfigChangeListener {
public:
	GoalDetectionOpenCV();
	virtual ~GoalDetectionOpenCV();

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
	int minY;
};

#endif /* GOALDETECTIONOPENCV_H_ */
