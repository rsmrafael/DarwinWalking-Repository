/*
 * LineModel.cpp
 *
 *  Created on: 07.01.2014
 *      Author: Oliver Krebs
 */

#include "LineModel.h"
#include "../../../Geometry/Geometry.h"
#include <cmath>

LineModel::LineModel()
:	AbstractModel(),
 	mStartPoint(),
 	mEndPoint(),
 	mSlope(0.0),
 	mYIntercept(0.0)
 {
	update();
}

LineModel::LineModel(const Point &startPoint, const Point &endPoint)
:	AbstractModel(),
 	mStartPoint(startPoint),
	mEndPoint(endPoint),
	mSlope(0.0),
	mYIntercept(0.0)
{
	update();
}

LineModel::~LineModel()
{
}

LineModel* LineModel::createNextModel(const list<Point>* data) {

	int s = (int)data->size();
	if (s >= 2) {

		int i1, i2;
		int counter = 0;
		i1 = rand() % s;
		do {
			i2 = rand() % s;
			counter++;
			if( counter > 100) {
				return NULL;
			}
		} while( i1 == i2);

		Point p1 = Point();
		Point p2 = Point();
		int i = 0;
		int found = 0;
		list<Point>::const_iterator it;
		for( it = data->begin(); it != data->end() && found < 2; ++it) {
			if( i == i1) {
				p1 = (*it);
				found++;
			} else if( i == i2) {
				p2 = (*it);
				found++;
			}
			i++;
		}

		if( found < 2) {
			return NULL;
		}

		//Debugger::DEBUG("LineModel", "Model: (%i|%i) - (%i|%i)",
			//			p1.getX(), p1.getY(), p2.getX(), p2.getY() );

		LineModel* newModel = new LineModel( p1, p2);

		return newModel;
	}
	return NULL;
}

double LineModel::calculateError(const list<Point>* data) {
	double sumOfErrors = 0;
	list<Point>::const_iterator pointIterator;
	for (pointIterator = data->begin(); pointIterator != data->end();
			++pointIterator) {
		sumOfErrors += calculateError(&(*pointIterator));
	}
	return sumOfErrors;
}

double LineModel::calculateError(const Point* data) {
	//return distancePointToLineSegment( data, mStartPoint, mEndPoint);
	double err = (double)(data->getY()) - (mSlope*(double)data->getX());
	err -= mYIntercept;
	err = abs(err) / sqrt((mSlope*mSlope) + 1.0);
	//Debugger::DEBUG("LineModel", "Err: %f", err);
	return err;
}

size_t LineModel::getNumberOfFittedDataPoints(const list<Point>* data, double maximumError) {
	size_t numberOfFittedPoints = 0;
	list<Point>::const_iterator pointIterator;
	for (pointIterator = data->begin(); pointIterator != data->end();
			++pointIterator) {
		if (calculateError(&(*pointIterator)) <= maximumError) {
			numberOfFittedPoints++;
		}
	}
	return numberOfFittedPoints;
}

list<Point>* LineModel::getNotFittedPoints(const list<Point>* data, double maximumError) {
	list<Point>* notFittedPoints = new list<Point>;
	list<Point>::const_iterator pointIterator;
	for (pointIterator = data->begin(); pointIterator != data->end();
			++pointIterator) {
		if (calculateError(&(*pointIterator)) > maximumError) {
			notFittedPoints->push_back((*pointIterator));
		}
	}
	return notFittedPoints;
}

list<Point>* LineModel::getFittedPoints(const list<Point>* data, double maximumError) {
	list<Point>* fittedPoints = new list<Point>;
	list<Point>::const_iterator pointIterator;
	for (pointIterator = data->begin(); pointIterator != data->end();
			++pointIterator) {
		if (calculateError(&(*pointIterator)) <= maximumError) {
			fittedPoints->push_back((*pointIterator));
		}
	}
	return fittedPoints;
}

void LineModel::eraseFittedPoints(list<Point>* data, double maximumError) {
	list<Point>::iterator it;
	for (it = data->begin(); it != data->end(); ) {
		if (calculateError(&(*it)) <= maximumError) {
			it = data->erase(it);
		} else {
			++it;
		}
	}
}

void LineModel::update() {
	int dx = mEndPoint.getX() - mStartPoint.getX();
	int dy = mEndPoint.getY() - mStartPoint.getY();
	if( dx == 0) {
		mSlope = 1e100;
	} else {
		mSlope = (double)(dy) / (double)(dx);
	}
	mYIntercept = (double)mStartPoint.getY() - mSlope * (double)mStartPoint.getX();
}

Point LineModel::calcRightPoint(const list<Point>* data, double maximumError) {
	Point rightPoint(-1,0);
	list<Point>::const_iterator pointIterator;
	for (pointIterator = data->begin(); pointIterator != data->end();
			++pointIterator) {
		if (calculateError(&(*pointIterator)) <= maximumError) {
			if( (*pointIterator).getX() > rightPoint.getX()) {
				rightPoint = (*pointIterator);
			}
		}
	}
	return rightPoint;
}

Point LineModel::calcLeftPoint(const list<Point>* data, double maximumError) {
	Point leftPoint(10000,0);
	list<Point>::const_iterator pointIterator;
	for (pointIterator = data->begin(); pointIterator != data->end();
			++pointIterator) {
		if (calculateError(&(*pointIterator)) <= maximumError) {
			if( (*pointIterator).getX() < leftPoint.getX()) {
				leftPoint = (*pointIterator);
			}
		}
	}
	return leftPoint;
}

int LineModel::getX(int y) const {
	return (int)(((double)y - mYIntercept) / mSlope);
}

int LineModel::getY(int x) const {
	return (int)(mSlope*(double)x + mYIntercept);
}

const Point LineModel::getStartPoint() const {
	return mStartPoint;
}
const Point LineModel::getEndPoint() const {
	return mEndPoint;
}
