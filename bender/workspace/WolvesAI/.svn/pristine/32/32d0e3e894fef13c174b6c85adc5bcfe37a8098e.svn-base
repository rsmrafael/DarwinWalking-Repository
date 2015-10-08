/*
 * LocalModeling.cpp
 *
 *  Created on: 20.02.2013
 *      Author: Jan Carstensen
 */

#include "LocalModeling.h"
#include "../../Config.h"
#include "../../Utils/Constants.h"

#define _USE_MATH_DEFINES
#include <math.h>

LocalModeling::LocalModeling() {
	mLocalModel = ObjectList();
	mMaxMatchDistance = 20.0;//0.5;
	mGain = 0.9;
	mMaxNotSeen = 10;

	mOdomOrientation = 0.0;
	mOdomX = 0;
	mOdomY = 0;

	mLastOdomOrientation = 0.0;
	mLastOdomX = 0;
	mLastOdomY = 0;

	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

LocalModeling::~LocalModeling() {
}

bool LocalModeling::execute() {

	// update odometry
	Odometry odom = getOdometry();

	mOdomX = odom.getX() - mLastOdomX;
	mOdomY = odom.getY() - mLastOdomY;
	mOdomOrientation = odom.getOrientation() - mLastOdomOrientation;

	mLastOdomX = odom.getX();
	mLastOdomY = odom.getY();
	mLastOdomOrientation = odom.getOrientation();
	//Debugger::DEBUG("LocalModeling", "Odometrie Update X:%i, Y:%i, Orientation:%f",mLastOdomX,mLastOdomY,mLastOdomOrientation);
	//updateOdometry( getBallRelative());

	const vector<Vector> &localPoles = getPolesRelative().get();
	for (Vector vec : localPoles) {
		//updateOdometry( (*it));
		//it->lastSeenCounter--;
	}

	getPolesRelative().data.clear();

	// update Local Model

	const vector<Vector> &goalList = getGoalPolesVectors().get();
	for (Vector vec : goalList) {
		//Object pole = (*it);
		//pole.lastVector = calculateVector(pole);
		//Vector vec = (*it);

		// test how it works, but is probably very bad
		const Vector *bestMatch = NULL;
		double bestScore = 0.0;

		for (Vector locVec : localPoles) {
			//Vector locVec = (*ot);
			double currentScore = calculateMatchScoring(locVec, vec);
			if (currentScore > bestScore) {
				bestScore = currentScore;
				bestMatch = &locVec;
			}
		}

		if (bestMatch != NULL) {
			//Filter Update (switch to Kalmann...)
			double length = vec.getLength() * mGain
					+ bestMatch->getLength() * (1.0 - mGain);
			double angle = vec.getAngle() * mGain
					+ bestMatch->getAngle() * (1.0 - mGain);
			vec = Vector(angle, length);
			/*pole.lastSeenCounter += 2;
			if( pole.lastSeenCounter > 200) {
				pole.lastSeenCounter = 200;
			}
			bestMatch->matched = true;*/
			//mLocalModelData[]
			getPolesRelative().data.push_back(vec);
		} else {
			getPolesRelative().data.push_back(vec);
		}

	}

	//for (vector<Vector>::const_iterator it = localPoles.begin(); it != localPoles.end(); ++it) {
	for (Vector vec : localPoles) {
		//Vector vec = (*it);
		//if( !it->matched && it->lastSeenCounter >= 0) {
			getPolesRelative().data.push_back(vec);
		//}
	}

	getLocalModel().clearList();
	//getLocalModel().addObject(getBallRelative());
	//getLocalModel().addObjects(objects);

	return true;
}

void LocalModeling::updateOdometry(Vector& vec) {
	// 1) Rotate Objects
	vec = Vector(vec.getAngle() + mOdomOrientation, vec.getLength());

	// 2) Move Objects
	Vector vecOdom(atan2((double)mOdomX, (double)mOdomY),
			sqrt((double)mOdomX*mOdomX + (double)mOdomY*mOdomY));
	vec = vec.sum(vecOdom);
}

double LocalModeling::calculateMatchScoring(Vector &one, Vector &two) const {
	CompareData data1, data2;
	auto it = mLocalModelData.find(&one);
	if( it != mLocalModelData.end()) {
		data1 = it->second;
	}
	it = mLocalModelData.find(&two);
	if( it != mLocalModelData.end()) {
		data2 = it->second;
	}
	if (data1.matched || data2.matched || data1.objectType != data2.objectType) {
		return -1.0;
	} else {
		double dist = one.calcluateDistanceTo(two);
		if (dist < mMaxMatchDistance) {
			if (dist > 0.01) {
				return 1.0 / dist;
			} else {
				return 100.0;
			}
		} else {
			return -1.0;
		}
	}
}

void LocalModeling::configChanged() {
	Debugger::INFO("LocalModeling", "config changed");

	ConfigFile *config = Config::getInstance();
	mMaxMatchDistance = config->get<double>("Localization", "Local_maxMatchDistance", 500.0);
	mGain = config->get<double>("Localization", "Local_gain", 0.9);
	mMaxNotSeen = config->get<int>("Localization", "Local_maxNotSeen", 10);
}

