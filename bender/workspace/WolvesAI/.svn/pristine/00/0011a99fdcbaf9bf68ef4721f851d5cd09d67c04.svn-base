/*
 * ConfigurationProvider.cpp
 *
 *  Created on: 29.08.2012
 *      Author: Stefan Krupop
 */

#include "ConfigurationProvider.h"
#include "../Config.h"
#include "../Debugging/Debugger.h"
#include "../Utils/Constants.h"

ConfigurationProvider::ConfigurationProvider(Body* body, Game* game) {
	mBody = body;
	mGame = game;
	if( mBody != NULL && mBody->PanTilt() != NULL) {
		ConfigFile *config = Config::getInstance();
		double panErr = config->get<double>("Robot", "panError", 0.0);
		double tiltErr = config->get<double>("Robot", "tiltError", 0.0);
		mBody->PanTilt()->SetPanTiltCorrection(panErr, tiltErr);
	}
	loadCameraSettings();
	mCameraSettingsChangedState = getCameraSettings().changedStatus;
}

ConfigurationProvider::~ConfigurationProvider() {
	mBody = NULL;
	mGame = NULL;
}

bool ConfigurationProvider::execute() {
	if (mBody != NULL) {
		getKickStatus().setKickingTime(mBody->getKickingTime());
		getBodyStatus().setPanTilt((int)mBody->PanTilt()->GetPan(), (int)mBody->PanTilt()->GetTilt());
		const ICommunicator::statusMessage_t* status = mBody->GetStatusMessage();
		if (status != NULL) {
			getBodyStatus().setVoltage((double)status->voltage / 100.0);
			for (int i = 0; i < SERVO_COUNT_MAX; ++i) {
				const ICommunicator::servoData_t* servo = &(status->servo_status[i]);
				getBodyStatus().setServoStatus(i, (double)servo->maxPositionError / 10.0, (double)servo->temperature, (double)servo->voltage / 10.0);
				getOdometry().update(status->odometry.xPos, status->odometry.yPos, status->odometry.viewDirection, status->odometry.weight);
			}
		}
	}
	if (mGame != NULL) {
		GameStatus& gs = getGameStatus();
		gs.haveKickOff = mGame->haveKickOff();
		gs.kickOffTime = mGame->getKickOffTime();
		gs.gameState = mGame->getGameState();
		gs.isFirstHalf = mGame->isFirstHalf();
	}
	if (getCameraSettings().changedStatus != mCameraSettingsChangedState) {
		saveCameraSettings();
		mCameraSettingsChangedState = getCameraSettings().changedStatus;
	}

	return true;
}

void ConfigurationProvider::loadCameraSettings() const {
	CameraSettings& settings = getCameraSettings();
	ConfigFile *config = Config::getInstance();
	settings.width 			= config->get<int>("Camera", "width", 800); //352
	settings.height 		= config->get<int>("Camera", "height", 600); //288
	settings.frameIntervalNumerator 	= config->get<int>("Camera", "frameIntervalNumerator", 1);
	settings.frameIntervalDenominator	= config->get<int>("Camera", "frameIntervalDenominator", 30);
	settings.brightness 	= config->get<double>("Camera", "brightness", 0.0); //4: 0.2876
	settings.contrast 		= config->get<double>("Camera", "contrast", 0.0); //4: 0.2765
	settings.saturation 	= config->get<double>("Camera", "saturation", 0.0); //4: 0.6123

	settings.autohue 		= config->get<double>("Camera", "autohue", 0.0);
	settings.hue 			= config->get<double>("Camera", "hue", 0.0);
	settings.redbalance 	= config->get<double>("Camera", "redbalance", 0.0);
	settings.bluebalance 	= config->get<double>("Camera", "bluebalance", 0.0);
	settings.gamma 			= config->get<double>("Camera", "gamma", 0.0);
	settings.exposure 		= config->get<double>("Camera", "exposure", 0.0);
	settings.autogain 		= config->get<double>("Camera", "autogain", 0.0);
	settings.gain 			= config->get<double>("Camera", "gain", 0.0);
	settings.sharpness 		= config->get<double>("Camera", "sharpness", 0.0);
	settings.powerlinefreq  = config->get<double>("Camera", "powerlinefreq", 0.0);
	settings.autowhitebalance 		= config->get<double>("Camera", "autowhitebalance", 0.0);
	settings.whitebalancetemp 		= config->get<double>("Camera", "whitebalancetemp", 0.0);
	settings.backlightcompensation 	= config->get<double>("Camera", "backlightcompensation", 0.0);
	settings.autoexposure 			= config->get<double>("Camera", "autoexposure", 0.0);

	settings.setChanged();
}

void ConfigurationProvider::saveCameraSettings() const {
	Debugger::INFO("ConfigurationProvider", "Saving camera settings...");
	CameraSettings& settings = getCameraSettings();
	ConfigFile *config = Config::getInstance();
	config->set<int>("Camera", "width", settings.width);
	config->set<int>("Camera", "height", settings.height);
	config->set<int>("Camera", "frameIntervalNumerator", settings.frameIntervalNumerator);
	config->set<int>("Camera", "frameIntervalDenominator", settings.frameIntervalDenominator);
	config->set<double>("Camera", "brightness", settings.brightness);
	config->set<double>("Camera", "contrast", settings.contrast);
	config->set<double>("Camera", "saturation", settings.saturation);
	config->set<double>("Camera", "autohue", settings.autohue);
	config->set<double>("Camera", "hue", settings.hue);
	config->set<double>("Camera", "autowhitebalance", settings.autowhitebalance);
	config->set<double>("Camera", "redbalance", settings.redbalance);
	config->set<double>("Camera", "bluebalance", settings.bluebalance);
	config->set<double>("Camera", "gamma", settings.gamma);
	config->set<double>("Camera", "exposure", settings.exposure);
	config->set<double>("Camera", "autogain", settings.autogain);
	config->set<double>("Camera", "gain", settings.gain);
	config->set<double>("Camera", "sharpness", settings.sharpness);
	config->set<double>("Camera", "powerlinefreq", settings.powerlinefreq);
	config->set<double>("Camera", "whitebalancetemp", settings.whitebalancetemp);
	config->set<double>("Camera", "backlightcompensation", settings.backlightcompensation);
	config->set<double>("Camera", "autoexposure", settings.autoexposure);
	config->save();
}
