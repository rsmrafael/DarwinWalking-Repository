/*
 * PointCloud.cpp
 *
 *  Created on: 29.08.2012
 *      Author: Stefan Krupop
 */

#include "PointCloud.h"
#include "../Messages/Point.pb.h"
#include "../Messages/PointCloud.pb.h"
#include "../Messages/Representations.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"

PointCloud::PointCloud() {
	mData = new list<Point>();
}

PointCloud::PointCloud(const PointCloud& pC) :
		Printable(pC) {
	list<Point>* new_data = new list<Point>();
	for (list<Point>::const_iterator pointIterator = pC.getCloud()->begin();
			pointIterator != pC.getCloud()->end(); ++pointIterator) {
		new_data->push_back((*pointIterator));
	}

	mData = new_data;
}

PointCloud::~PointCloud() {
	if (mData->size() > 0) {
		clearCloud();
	}
	delete mData;
}

PointCloud& PointCloud::operator=(const PointCloud& cSource) {
	if (this != &cSource) // protect against invalid self-assignment
			{
		// 1: allocate new memory and copy the elements
		list<Point>* new_data = new list<Point>();
		for (list<Point>::iterator pointIterator = mData->begin();
				pointIterator != mData->end(); ++pointIterator) {
			new_data->push_back((*pointIterator));
		}

		// 2: deallocate old memory
		clearCloud();
		delete mData;

		// 3: assign the new memory to the object
		mData = new_data;
	}
	// by convention, always return *this
	return *this;
}

void PointCloud::clearCloud() {
	mData->clear();
}

void PointCloud::addPoint(Point point) {
	mData->push_back(point);
}

void PointCloud::addPoint(int x, int y) {
	mData->push_back(Point(x, y));
}

void PointCloud::removeFirst() {
	mData->pop_front();
}

void PointCloud::removeLast() {
	mData->pop_back();
}

void Serializer<PointCloud>::serialize(const PointCloud& representation,
		ostream& stream) {
	protobuf::PointCloud pointCloud;

	for (list<Point>::const_iterator pointIterator =
			representation.getCloud()->begin();
			pointIterator != representation.getCloud()->end();
			++pointIterator) {
		protobuf::Point* point = pointCloud.add_point();
		point->set_x((*pointIterator).getX());
		point->set_y((*pointIterator).getY());
	}

	OstreamOutputStream buf(&stream);
	pointCloud.SerializeToZeroCopyStream(&buf);
}

void Serializer<PointCloud>::deserialize(istream& stream,
		PointCloud& representation) {
	protobuf::PointCloud pointCloud;

	IstreamInputStream buf(&stream);
	pointCloud.ParseFromZeroCopyStream(&buf);

	for (int i = 0; i < pointCloud.point_size(); ++i) {
		representation.addPoint(pointCloud.point(i).x(),
				pointCloud.point(i).y());
	}
}

BoundingBox PointCloud::calculateBoundingBox() const {

	if( mData->empty()) {
		BoundingBox box = BoundingBox(Point(0,0),0,0);
		return box;
	}

	list<Point>::const_iterator it = mData->begin();
	int maxX = (*it).getX();
	int minX = (*it).getX();
	int minY = (*it).getY();
	int maxY = (*it).getY();

	++it;

	for (; it != mData->end();
			++it) {
		Point p = (*it);
		if (p.getX() < minX) {
			minX = p.getX();
		} else if (p.getX() > maxX) {
			maxX = p.getX();
		}

		if (p.getY() < minY) {
			minY = p.getY();
		} else if (p.getY() > maxY) {
			maxY = p.getY();
		}
	}

	BoundingBox retBounding = BoundingBox(Point(minX, minY),
			maxX - minX, maxY - minY);
	return retBounding;
}

Point PointCloud::getCenterPoint() const {

	if (mData->empty()) {
		return Point(0, 0);
	}

	int x = 0;
	int y = 0;

	for (list<Point>::const_iterator it = mData->begin(); it != mData->end();
			++it) {
		x += (*it).getX();
		y += (*it).getY();
	}

	//Debugger::DEBUG("PointCloud", "mData size: %i", mData->size());

	int xx = x / (int) (mData->size());
	int yy = y / (int) (mData->size());

	return Point(xx, yy);
}

int PointCloud::getMaximumDeviation() const {
	Point center = getCenterPoint();

	double maxDistance = 0;

	for (list<Point>::const_iterator it = mData->begin(); it != mData->end();
			++it) {
		double dist = center.calculateDistanceTo((*it));

		if (dist > maxDistance) {
			maxDistance = dist;
		}
	}

	return (int) (maxDistance);
}

void PointCloud::join(const PointCloud *pc) {
	for (list<Point>::const_iterator pi = pc->getCloud()->begin();
			pi != pc->getCloud()->end(); ++pi) {
		if(!contains((*pi))){
			mData->insert(mData->end(),(*pi));
		}
	}
}

void PointCloud::joinWithoutCheck(const PointCloud *pc) {
	mData->insert(mData->end(), pc->getCloud()->begin(), pc->getCloud()->end());
}

bool PointCloud::contains(Point p) const {

	for (list<Point>::const_iterator it = mData->begin(); it != mData->end();
			++it) {
		if (p == (*it)) {
			return true;
		}
	}
	return false;
}

bool PointCloud::doBoundingBoxesOverlap( const BoundingBox &box) const {
	BoundingBox myBox = calculateBoundingBox();
	bool noOverlap = myBox.topLeft.getX()
			> box.topLeft.getX() + box.width
			|| box.topLeft.getX() > myBox.topLeft.getX() + myBox.width
			|| myBox.topLeft.getY()
					> box.topLeft.getY() + box.height
			|| box.topLeft.getY() > myBox.topLeft.getY() + myBox.width;

	return !noOverlap;
}
