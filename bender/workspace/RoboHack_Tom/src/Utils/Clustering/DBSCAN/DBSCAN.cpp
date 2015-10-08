/*
 * DBSCAN.cpp
 *
 *  Created on: April 26, 2013
 *      Author: Jan Carstensen
 */

#include "DBSCAN.h"
#include "../../../Debugging/Debugger.h"

DBSCAN::DBSCAN(size_t minPoints, int epsilonDistance)
:	MAX_CLUSTER_POINTS( 500)
{
	mPoints = map<Point, TYPE, mapComp>();
	mMinPoints = minPoints;
	mEpsilonDistance = epsilonDistance;
	mClusters = list<PointCloud>();
	mCluster = PointCloud();
}

DBSCAN::~DBSCAN()
{
}

list<PointCloud> DBSCAN::cluster(list<Point> *dataset)
{
	Debugger::DEBUG("DBSCAN", "Cluster points: %d", dataset->size());
	if(dataset->size() > MAX_CLUSTER_POINTS) {
		Debugger::DEBUG("DBSCAN", "Too many points: %d", dataset->size());
		list<Point>::iterator it = dataset->begin();
		while( it != dataset->end() ) {
			if( ++it != dataset->end()) {
				it = dataset->erase(it);
			}
		}
		return cluster( dataset);
	}

	mPoints.clear();

	for (list<Point>::const_iterator it = dataset->begin();
			it != dataset->end(); ++it) {
		mPoints.insert(pair<Point, TYPE>((*it), NOTVISITED));
	}

	for (map<Point, TYPE, mapComp>::iterator it = mPoints.begin();
			it != mPoints.end(); ++it) {

		if ((*it).second == NOTVISITED)
		{
			(*it).second = VISITED;
			PointCloud nei = PointCloud();
			getNeighbors((*it).first, &nei);
			if (nei.getSize() < mMinPoints) {
				(*it).second = NOISE;

			} else {

				if (mCluster.getSize() > mMinPoints) {
					mClusters.push_back(mCluster);
				}

				mCluster = PointCloud();
				mCluster.clearCloud();

				expandCluster((*it).first, &nei);
			}

		}
	}

	if (mCluster.getSize() > mMinPoints) {
		mClusters.push_back(mCluster);
	}
	return mClusters;
}

void DBSCAN::getNeighbors(const Point &point, PointCloud* neighbours) const
{
	neighbours->addPoint(point);

	//Debugger::DEBUG("DBSCAN", "x: %i, y: %i",point.getX(), point.getY());
	for (map<Point, TYPE, mapComp>::const_iterator it = mPoints.begin();
			it != mPoints.end(); ++it) {

		if ((*it).first.calcluateDistanceTo(point) < mEpsilonDistance) {
			//		Debugger::DEBUG("DBSCAN", "add Point");
			neighbours->addPoint((*it).first);
		}
	}

	//Debugger::DEBUG("DBSCAN", "neighbor size: %i", neighbours->getSize());
}

void DBSCAN::expandCluster(const Point &point, PointCloud* neighbours)
{
//	Debugger::DEBUG("DBSCAN", "expandCluster");

	mCluster.addPoint(point);

	for (list<Point>::const_iterator it = neighbours->getCloud()->begin();
			it != neighbours->getCloud()->end(); ++it) {
		if (mPoints[(*it)] == NOTVISITED) {
			mPoints[(*it)] = VISITED;

			PointCloud neighbours_ = PointCloud();

			getNeighbors((*it), &neighbours_);

			if (neighbours_.getSize() > mMinPoints) {
				neighbours->join(&neighbours_);
			}
		}

		if (!isMemberOfCluster((*it))) {
			mCluster.addPoint((*it));
		}
	}
}

bool DBSCAN::isMemberOfCluster(Point point)
{
	for (list<PointCloud>::const_iterator ci = mClusters.begin();
			ci != mClusters.end(); ++ci) {
		for (list<Point>::const_iterator pi = (*ci).getCloud()->begin();
				pi != (*ci).getCloud()->end(); ++pi) {
			//Debugger::DEBUG("DBSCAN", "check");
			if (point.getX() == (*pi).getX() && point.getY() == (*pi).getY()) {
				//Debugger::DEBUG("DBSCAN", "true");
				return true;
			}
		}
	}

	for (list<Point>::const_iterator pi = mCluster.getCloud()->begin();
			pi != mCluster.getCloud()->end(); ++pi) {
		//Debugger::DEBUG("DBSCAN", "check");
		if (point.getX() == (*pi).getX() && point.getY() == (*pi).getY()) {
			//Debugger::DEBUG("DBSCAN", "true");
			return true;
		}
	}

	return false;
}

