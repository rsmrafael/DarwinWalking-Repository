/*
 * RobotConfiguration.h
 *
 *  Created on: 26.11.2012
 *      Author: Stefan Krupop
 */

#ifndef ROBOTCONFIGURATION_H_
#define ROBOTCONFIGURATION_H_

/**
 * Representation of robot configuration
 */
class RobotConfiguration {
public:
	/**
	 * Constructor
	 */
	RobotConfiguration();

	/**
	 * Destructor
	 */
	virtual ~RobotConfiguration();

	double minPanAngle;		//!< minimal pan angle
	double maxPanAngle;		//!< maximal pan angle
	double panRange;		//!< pan range angle (max - min)

	double minTiltAngle;	//!< minimal tilt angle
	double maxTiltAngle;	//!< maximal tilt angle
	double tiltRange;		//!< tilt range angle (max - min)

	double cameraHeight;	//!< height of camera
	double cameraOffsetX;	//!< offset of camera in x

	double cameraOpeningHorizontal;	//!< camera opening horizontal
	double cameraOpeningVertical;	//!< camera opening vertical

	int focalPixelLengthX;	//!< focal pixel length in x
	int focalPixelLengthY;	//!< focal pixel length in y
	int principlePointX;	//!< principle point x
	int principlePointY;	//!< principle point y

};

#endif /* ROBOTCONFIGURATION_H_ */
