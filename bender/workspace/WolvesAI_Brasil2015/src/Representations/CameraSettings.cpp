/*
 * CameraSettings.cpp
 *
 *  Created on: 21.08.2012
 *      Author: Stefan Krupop
 */

#include "CameraSettings.h"
#include "../Messages/Representations.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"

CameraSettings::CameraSettings() {
	changedStatus = false; // Will toggle between false and true on each change
	width = 800;
	height = 600;
	frameIntervalNumerator = 1;
	frameIntervalDenominator = 30;
	brightness = -1.0; // -1 = default value from driver
	contrast = -1.0;
	saturation = -1.0;
	autohue = -1.0;
	hue = -1.0;
	autowhitebalance = -1.0;
	redbalance = -1.0;
	bluebalance = -1.0;
	gamma = -1.0;
	exposure = -1.0;
	autogain = -1.0;
	gain = -1.0;
	sharpness = -1.0;
	powerlinefreq = -1.0;
	whitebalancetemp = -1.0;
	backlightcompensation = -1.0;
	autoexposure = -1.0;
}

CameraSettings::~CameraSettings() {
}

void CameraSettings::setChanged() {
	changedStatus = !changedStatus;
}

void Serializer<CameraSettings>::serialize(const CameraSettings& representation, ostream& stream) {
	protobuf::CameraSettings settings;

	settings.set_width((int32)representation.width);
	settings.set_height((int32)representation.height);
	settings.set_frameintervalnumerator((int32)representation.frameIntervalNumerator);
	settings.set_frameintervaldenominator((int32)representation.frameIntervalDenominator);
	settings.set_brightness(representation.brightness);
	settings.set_contrast(representation.contrast);
	settings.set_saturation(representation.saturation);
	settings.set_autohue(representation.autohue);
	settings.set_hue(representation.hue);
	settings.set_autowhitebalance(representation.autowhitebalance);
	settings.set_redbalance(representation.redbalance);
	settings.set_bluebalance(representation.bluebalance);
	settings.set_gamma(representation.gamma);
	settings.set_exposure(representation.exposure);
	settings.set_autogain(representation.autogain);
	settings.set_gain(representation.gain);
	settings.set_sharpness(representation.sharpness);
	settings.set_powerlinefreq(representation.powerlinefreq);
	settings.set_whitebalancetemp(representation.whitebalancetemp);
	settings.set_backlightcompensation(representation.backlightcompensation);
	settings.set_autoexposure(representation.autoexposure);

	OstreamOutputStream buf(&stream);
	settings.SerializeToZeroCopyStream(&buf);
}

void Serializer<CameraSettings>::deserialize(istream& stream, CameraSettings& representation) {
	protobuf::CameraSettings settings;

	IstreamInputStream buf(&stream);
	if (!settings.ParseFromZeroCopyStream(&buf)) {
		Debugger::ERR("CameraSettings", "Failed to deserialize: %s", settings.InitializationErrorString().c_str());
	}

	representation.width = settings.width();
	representation.height = settings.height();
	representation.frameIntervalNumerator = settings.frameintervalnumerator();
	representation.frameIntervalDenominator = settings.frameintervaldenominator();
	representation.brightness = settings.brightness();
	representation.contrast = settings.contrast();
	representation.saturation = settings.saturation();
	representation.autohue = settings.autohue();
	representation.hue = settings.hue();
	representation.autowhitebalance = settings.autowhitebalance();
	representation.redbalance = settings.redbalance();
	representation.bluebalance = settings.bluebalance();
	representation.gamma = settings.gamma();
	representation.exposure = settings.exposure();
	representation.autogain = settings.autogain();
	representation.gain = settings.gain();
	representation.sharpness = settings.sharpness();
	representation.powerlinefreq  = settings.powerlinefreq();
	representation.whitebalancetemp = settings.whitebalancetemp();
	representation.backlightcompensation = settings.backlightcompensation();
	representation.autoexposure = settings.autoexposure();

	representation.setChanged();
}
