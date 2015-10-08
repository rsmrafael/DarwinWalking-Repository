/*
 * PointCloudList.cpp
 *
 *  Created on: 17.12.2013
 *      Author: Oliver
 */

#include "PointCloudList.h"

PointCloudList::PointCloudList() {

}

PointCloudList::~PointCloudList() {
	clearList();
}

void PointCloudList::clearList() {
	vector<PointCloud*>::const_iterator it;
	for (it = mClouds.begin(); it != mClouds.end(); ++it) {
		delete (*it);
	}
	mClouds.clear();
}

PointCloud *PointCloudList::addPointCloud( PointCloud *cloud) {
	mClouds.push_back(cloud);
	return cloud;
}

const vector<PointCloud*>& PointCloudList::getPointClouds() const {
	return mClouds;
}

void PointCloudList::print(ostream& stream) const {
	vector<PointCloud*>::const_iterator it;
	for (it = mClouds.begin(); it != mClouds.end(); ++it) {
		(*it)->print(stream);
	}
}
