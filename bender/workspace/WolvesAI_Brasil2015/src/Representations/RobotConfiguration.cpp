/*
 * RobotConfiguration.cpp
 *
 *  Created on: 26.11.2012
 *      Author: Stefan Krupop
 */

#include "RobotConfiguration.h"
#include "../External/SettingConfig.h"
#include "../Config.h"
#include "../Utils/Constants.h"
#include <cmath>

RobotConfiguration::RobotConfiguration() {
	SettingConfig *config = SettingConfig::getInstance();
	minPanAngle = config->get<double>("RobotData", "MinPanAngle", -90.0);
	maxPanAngle = config->get<double>("RobotData", "MaxPanAngle", +90.0);
	minTiltAngle = config->get<double>("RobotData", "MinTiltAngle", 0.0);
	maxTiltAngle = config->get<double>("RobotData", "MaxTiltAngle", 90.0);

	panRange = maxPanAngle - minPanAngle;
	tiltRange = maxTiltAngle - minTiltAngle;

	cameraHeight = config->get<double>("RobotData", "CameraHeight", 450.0,
			"distance of camera to ground");
	cameraOffsetX = config->get<double>("RobotData", "CameraOffsetX", 50.0,
			"offset of camera to center of robot in x (front)");

	focalPixelLengthX = config->get<int>("RobotData", "FocalPixelLengthX", 362);
	focalPixelLengthY = config->get<int>("RobotData", "FocalPixelLengthY", 435);

	std::string val = config->getString("RobotData", "PrinciplePointX", "w/2",
			"default: 'w/2'; int value in pixel");
	if( val == "w/2") {
		principlePointX = Config::getInstance()->get<int>("Camera", "width", 800) / 2;
	} else {
		std::stringstream stream;
		stream << val;
		stream >> principlePointX;
	}
	val = config->getString("RobotData", "PrinciplePointY", "h/2",
			"default: 'h/2'; int value in pixel");
	if( val == "h/2") {
		principlePointY = Config::getInstance()->get<int>("Camera", "height", 600) / 2;
	} else {
		std::stringstream stream;
		stream << val;
		stream >> principlePointY;
	}

	cameraOpeningHorizontal = (2.0 * atan((double)principlePointX / (double)focalPixelLengthX)) * RADIAN_TO_DEGREE;
	cameraOpeningVertical = (2.0 * atan((double)principlePointY / (double)focalPixelLengthY)) * RADIAN_TO_DEGREE;
}

RobotConfiguration::~RobotConfiguration() {
}

