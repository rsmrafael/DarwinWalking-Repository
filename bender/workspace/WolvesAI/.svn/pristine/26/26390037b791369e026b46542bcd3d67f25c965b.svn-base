/*
 * RobotConfiguration.cpp
 *
 *  Created on: 26.11.2012
 *      Author: Stefan Krupop
 */

#include "RobotConfiguration.h"
#include "../RobotData.h"
#include <math.h>

RobotConfiguration::RobotConfiguration() {
	minPanAngle = MIN_PAN_ANGLE;
	maxPanAngle = MAX_PAN_ANGLE;

	minTiltAngle = MIN_TILT_ANGLE;
	maxTiltAngle = MAX_TILT_ANGLE;

	panRange = maxPanAngle - minPanAngle;
	tiltRange = maxTiltAngle - minTiltAngle;

	cameraHeight = CAMERA_HEIGHT;
	cameraOffsetX = 50.0;

	focalPixelLengthX = 362; 	// CALIBRATED DATA IN PIXEL
	focalPixelLengthY = 435;	// CALIBRATED DATA IN PIXEL

	principlePointX = 320;//160;	// CALIBRATED DATA IN PIXEL (image-width / 2)
	principlePointY = 240;//144;	// CALIBRATED DATA IN PIXEL (image-height / 2)

	cameraOpeningHorizontal = (2.0 * atan((double)principlePointX / (double)focalPixelLengthX)) * (180.0 / M_PI);
	cameraOpeningVertical = (2.0 * atan((double)principlePointY / (double)focalPixelLengthY)) * (180.0 / M_PI);
}

RobotConfiguration::~RobotConfiguration() {
}

