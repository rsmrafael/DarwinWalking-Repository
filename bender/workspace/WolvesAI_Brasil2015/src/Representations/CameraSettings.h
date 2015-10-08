/*
 * CameraSettings.h
 *
 *  Created on: 21.08.2012
 *      Author: Stefan Krupop
 */

#ifndef CAMERASETTINGS_H_
#define CAMERASETTINGS_H_

#include "../Blackboard/Serializer.h"

//lint -e1711

#include <string>

using namespace std;

/**
 * Representation for camera settings
 */
class CameraSettings {
public:
	/**
	 * Constructor
	 */
	CameraSettings();

	/**
	 * Destructor
	 */
	virtual ~CameraSettings();

	/**
	 * call for every change of settings
	 */
	void setChanged();

	bool changedStatus;				//!< changed status
	int width;						//!< camera image width in pixel
	int height;						//!< camera image height in pixel
	int frameIntervalNumerator;		//!< numerator
	int frameIntervalDenominator;	//!< denominator
	double brightness;				//!< brightness
	double contrast;				//!< contrast
	double saturation;				//!< saturation
	double autohue;					//!< autohue
	double hue;						//!< hue
	double autowhitebalance;		//!< autowhitebalance
	double redbalance;				//!< redbalance
	double bluebalance;				//!< bluebalance
	double gamma;					//!< gamma
	double exposure;				//!< exposure
	double autogain;				//!< autogain
	double gain;					//!< gain
	double sharpness;				//!< sharpness
	double powerlinefreq;			//!< powerlinefreq
	double whitebalancetemp;		//!< whitebalancetemp
	double backlightcompensation;	//!< backlightcompensation
	double autoexposure;			//!< autoexposure
private:
};

/**
 * spezialization of Serializer for CameraSettings
 */
template<>
class Serializer<CameraSettings> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const CameraSettings& representation, ostream& stream);

	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(istream& stream, CameraSettings& representation);
};

#endif /* CAMERASETTINGS_H_ */
