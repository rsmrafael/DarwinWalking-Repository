/*
 * VisionStatus.cpp
 *
 *  Created on: 29.04.2014
 *      Author: Oliver Krebs
 */

#include "VisionStatus.h"

VisionStatus::VisionStatus() {
	mObjectStatus = OBJECT_SEARCHING;
}

VisionStatus::~VisionStatus() {
}

VisionStatus::ObjectStatus VisionStatus::getObjectStatus() const {
	return mObjectStatus;
}

void VisionStatus::setObjectStatus(VisionStatus::ObjectStatus status) {
	mObjectStatus = status;
}
