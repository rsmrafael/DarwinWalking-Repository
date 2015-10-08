/*
 * HeadAction.cpp
 *
 *  Created on: 24.04.2014
 *      Author: Oliver Krebs
 */

#include "HeadAction.h"

HeadAction::HeadAction() {
	mSearchObjectType = Object::UNKNOWN;
	mPan = 0;
	mTilt = 0;
	mErrorX = 0;
	mErrorY = 0;
	mDisablePanError = false;
	mWithError = false;
	mPanTiltSetDirect = true;
	mResetSearch = false;
	mPanTiltChangeID = 0;
}

HeadAction::~HeadAction() {
}

void HeadAction::setPanTilt(int PanDeg, int TiltDeg) {
	mPan = PanDeg;
	mTilt = TiltDeg;
	mPanTiltSetDirect = true;
	mPanTiltChangeID++;
}

void HeadAction::PanTiltTracking(bool disablePan) {
	mErrorX = 0;
	mErrorY = 0;
	mDisablePanError = disablePan;
	mWithError = false;
	mPanTiltSetDirect = false;
	mPanTiltChangeID++;
}

void HeadAction::PanTiltTracking(double errorX, double errorY, bool disablePan) {
	mErrorX = errorX;
	mErrorY = errorY;
	mDisablePanError = disablePan;
	mWithError = true;
	mPanTiltSetDirect = false;
	mPanTiltChangeID++;
}

bool HeadAction::getResetSearch() const {
	return mResetSearch;
}


void HeadAction::searchObject( Object::ObjectType type, bool resetSearch) {
	mSearchObjectType = type;
	mResetSearch = resetSearch;
}

Object::ObjectType HeadAction::getSearchObjectType() {
	return mSearchObjectType;
}
