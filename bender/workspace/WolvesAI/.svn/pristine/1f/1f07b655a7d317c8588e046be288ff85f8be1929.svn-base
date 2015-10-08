/*
 * PanTiltUnit.cpp
 *
 *  Created on: 25.06.2009
 *      Author: Stefan
 */

#include "PanTiltUnit.h"

//#include <cmath>
#include <iostream>
#include "../Debugging/Debugger.h"
#include "../External/SettingConfig.h"

double PanTiltUnit::sMinPanExt = -90.0;
double PanTiltUnit::sMaxPanExt = +90.0;
double PanTiltUnit::sMinPan = -45.0;
double PanTiltUnit::sMaxPan = +45.0;
double PanTiltUnit::sMinTiltExt = 0.0;
double PanTiltUnit::sMinTilt = 45.0;
double PanTiltUnit::sMaxTilt = 90.0;

double PanTiltUnit::sPanGainP =  0.1;
double PanTiltUnit::sPanGainD =  0.1;
double PanTiltUnit::sTiltGainP = 0.1;
double PanTiltUnit::sTiltGainD = 0.1;

PanTiltUnit::PanTiltUnit(ICommunicator* comm) {
	SettingConfig *config = SettingConfig::getInstance();
	sMinPanExt = config->get<double>("RobotData", "MinPanAngleExt", -90.0);
	sMaxPanExt = config->get<double>("RobotData", "MaxPanAngleExt", +90.0);
	sMinPan = config->get<double>("RobotData", "MinPanAngle", -45.0);
	sMaxPan = config->get<double>("RobotData", "MaxPanAngle", +45.0);
	sMinTiltExt = config->get<double>("RobotData", "MinTiltAngleExt", 0.0);
	sMinTilt = config->get<double>("RobotData", "MinTiltAngle", 45.0);
	sMaxTilt = config->get<double>("RobotData", "MaxTiltAngle", 90.0);
	sPanGainP = config->get<double>("RobotData", "PanGainP", 0.1);
	sPanGainD = config->get<double>("RobotData", "PanGainD", 0.1);
	sTiltGainP = config->get<double>("RobotData", "TiltGainP", 0.1);
	sTiltGainD = config->get<double>("RobotData", "TiltGainD", 0.1);
	mComm = comm;
	mPan = sMinPan;
	mTilt = sMinTilt;
	mPanCorrection = 0.0;
	mTiltCorrection = 0.0;
	mPanErr = 0;
	mPanErrDiff = 0;
	mTiltErr = 0;
	mTiltErrDiff = 0;
	MoveToHome();
}

PanTiltUnit::~PanTiltUnit() {
	mComm = NULL;
}

void PanTiltUnit::SetPosition(double panDeg, double tiltDeg) {
	bool extended = false;
	if (tiltDeg > sMaxTilt) {
		mTilt = sMaxTilt;
	} else if (tiltDeg < sMinTiltExt) {
		mTilt = sMinTiltExt;
		extended = true;
	} else if (tiltDeg < sMinTilt) {
		mTilt = sMinTilt;
	} else {
		mTilt = tiltDeg;
	}

	if(!extended) {
		if (panDeg > sMaxPan) {
			mPan = sMaxPan;
		} else if (panDeg < sMinPan) {
			mPan = sMinPan;
		} else {
			mPan = panDeg;
		}
	} else {
		if (panDeg > sMaxPanExt) {
			mPan = sMaxPanExt;
		} else if (panDeg < sMinPanExt) {
			mPan = sMinPanExt;
		} else {
			mPan = panDeg;
		}
	}
	Execute();
}

double PanTiltUnit::GetPan() const {
	return mPan;
}

void PanTiltUnit::SetPan(double panDeg) {
	if (panDeg > sMaxPan) {
		mPan = sMaxPan;
	} else if (panDeg < sMinPan) {
		mPan = sMinPan;
	} else {
		mPan = panDeg;
	}
	Execute();
}

double PanTiltUnit::GetTilt() const {
	return mTilt;
}

void PanTiltUnit::SetTilt(double tiltDeg) {
	if (tiltDeg > sMaxTilt) {
		mTilt = sMaxTilt;
	} else if (tiltDeg < sMinTilt) {
		mTilt = sMinTilt;
	} else {
		mTilt = tiltDeg;
	}
	Execute();
}

void PanTiltUnit::SetPanTiltCorrection(double panCorr, double tiltCorr) {
	mPanCorrection = panCorr;
	mTiltCorrection = tiltCorr;
}

void PanTiltUnit::Execute() {
//	Debugger::DEBUG("PanTiltUnit", "Setting pan/tilt to %i/%i", mPan, mTilt);
	int16_t pan = (int16_t)((mPan + mPanCorrection) * 100.f);
	int16_t tilt = (int16_t)((mTilt + mTiltCorrection) * 100.f);
	mComm->PanTiltCommand(pan, tilt);
}

void PanTiltUnit::InitTracking() {
	mPanErr = 0;
	mPanErrDiff = 0;
	mTiltErr = 0;
	mTiltErrDiff = 0;
}

void PanTiltUnit::MoveTracking(bool disablePan) {
	double pOffset, dOffset;

	if (disablePan) {
		mPan = 0;
	} else {
		pOffset = mPanErr * sPanGainP;
		dOffset = mPanErrDiff * sPanGainD;
		mPan += pOffset + dOffset;
	}

	pOffset = mTiltErr * sTiltGainP;
	dOffset = mTiltErrDiff * sTiltGainD;
	mTilt += pOffset + dOffset;

	SetPosition(mPan, mTilt);
}

void PanTiltUnit::MoveTracking(double errorX, double errorY, bool disablePan) {
	if (disablePan) {
		mPanErrDiff = 0;
		mPanErr = 0;
	} else {
		mPanErrDiff = errorX - mPanErr;
		mPanErr = errorX;
	}

	mTiltErrDiff = errorY - mTiltErr;
	mTiltErr = errorY;

	MoveTracking(disablePan);
}

void PanTiltUnit::MoveToHome() {
	SetPosition(0.0, 90.0);
}
