/*
 * Odometry.cpp
 *
 *  Created on: 20.02.2013
 *      Author: Jan Carstensen
 */

#include "Odometry.h"
#include "../Utils/Constants.h"

Odometry::Odometry() {
	mDeltaX = 0;
	mDeltaY = 0;
	mDeltaO = 0.0;
	mWeight = 0.0;
}

Odometry::~Odometry() {
}

void Odometry::update(int16_t deltax, int16_t deltay, int16_t deltao, uint8_t weight) {
	mDeltaX = (int)deltax;
	mDeltaY = (int)deltay;
	mDeltaO = ((double)deltao) * DEGREE_TO_RADIAN;
	mWeight = ((double)weight) / 100.0;
}

