/*
 * CircleModel.cpp
 *
 *  Created on: Feb 22, 2012
 *      Author: Jan Carstensen
 */

#include "CircleModel.h"
#include <cmath>
#include <algorithm>

#include "../Data/Matrix3x3.h"

CircleModel::CircleModel()
:	AbstractModel(),
 	mCenter(),
 	mRadius(0)
{
}

CircleModel::CircleModel(const Point &center, int radius)
:	AbstractModel(),
 	mCenter(center),
 	mRadius(radius)
{
	if( mRadius < 0) {
		mRadius = 0;
	}
}


CircleModel::~CircleModel() {
}

CircleModel* CircleModel::createNextModel(const list<Point>* data) {

	int s = (int)data->size();
	if (s >= 3) {

		int i1, i2, i3;
		int counter = 0;
		i1 = rand() % s;
		do {
			i2 = rand() % s;
			i3 = rand() % s;
			counter++;
			if( counter > 100) {
				return NULL;
			}
		} while( i1 == i2 || i1 == i3 || i2 == i3);

		Point *p1 = NULL;
		Point *p2 = NULL;
		Point *p3 = NULL;
		int i = 0;
		int found = 0;
		list<Point>::const_iterator it;
		for( it = data->begin(); it != data->end() && found < 3; ++it) {
			if( i == i1) {
				p1 = new Point(*it);
				found++;
			} else if( i == i2) {
				p2 = new Point(*it);
				found++;
			} else if( i == i3) {
				p3 = new Point(*it);
				found++;
			}
			i++;
		}

		if( found < 3 || p1 == NULL || p2 == NULL || p3 == NULL) {
			if( p1 != NULL)
				delete p1;
			if( p2 != NULL)
				delete p2;
			if( p3 != NULL)
				delete p3;
			return NULL;
		}

		/*
		 *  Cramers Rule to solve the equations for the 3 Points
		 */

		int firstCol[3] = { 1, 1, 1 };
		int secondCol[3] = { -p1->getX(), -p2->getX(), -p3->getX() };
		int thirdCol[3] = { -p1->getY(), -p2->getY(), -p3->getY() };
		int resultCol[3] = { (int) -(pow(p1->getX(), 2.0)
				+ pow(p1->getY(), 2.0)), (int) -(pow(p2->getX(), 2.0)
				+ pow(p2->getY(), 2.0)), (int) -(pow(p3->getX(), 2.0)
				+ pow(p3->getY(), 2.0)) };

		Matrix3x3* matrix = new Matrix3x3(firstCol, secondCol, thirdCol);
		Matrix3x3* matrixA = new Matrix3x3(resultCol, secondCol, thirdCol);
		Matrix3x3* matrixB = new Matrix3x3(firstCol, resultCol, thirdCol);
		Matrix3x3* matrixC = new Matrix3x3(firstCol, secondCol, resultCol);

		int det = matrix->calculateDeterminante();
		int A, B, C;

		if (det != 0) {
			A = matrixA->calculateDeterminante() / det;
			B = matrixB->calculateDeterminante() / det;
			C = matrixC->calculateDeterminante() / det;
		} else {
			A = 0;
			B = 0;
			C = 0;
		}

		delete matrix;
		delete matrixA;
		delete matrixB;
		delete matrixC;

		delete p1;
		delete p2;
		delete p3;

		int x = B / 2;
		int y = C / 2;
		int radius = (int) sqrt( B*B / 4 + C*C / 4 - A);
		CircleModel* newModel = new CircleModel( Point(x, y), radius);

		//Debugger::DEBUG("CircleModel", "Model: (%i|%i) - %i", x, y, radius);

		return newModel;
	}
	return NULL;
}

/**
 *  Calculates the quadratic Error between the LineModel and the data tha should be fitted.
 *
 *
 */
double CircleModel::calculateError(const list<Point>* data) {
	double sumOfErrors = 0;
	list<Point>::const_iterator pointIterator;

	for (pointIterator = data->begin(); pointIterator != data->end();
			++pointIterator) {
		sumOfErrors += calculateError(&(*pointIterator));
	}
	return sumOfErrors;
}

double CircleModel::calculateError(const Point* data) {
	// Distance to the Center minus the radius is the distance to the Circle.
	if (data == NULL) {
		return 1e100;
	} else {
		return abs((mCenter.calculateDistanceTo(*data) - (double)mRadius));
	}
}

void CircleModel::eraseFittedPoints(list<Point>* data, double maximumError) {
	list<Point>::iterator it;
	for (it = data->begin(); it != data->end(); ) {
		if (calculateError(&(*it)) <= maximumError) {
			it = data->erase(it);
		} else {
			++it;
		}
	}
}

size_t CircleModel::getNumberOfFittedDataPoints(const list<Point>* data,
		double maximumError) {

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

list<Point>* CircleModel::getFittedPoints(const list<Point>* data,
		double maximumError) {
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

list<Point>* CircleModel::getNotFittedPoints(const list<Point>* data,
		double maximumError) {
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

const Point CircleModel::getCenter() const {
	return mCenter;
}

int CircleModel::getRadius() const {
	return mRadius;
}
