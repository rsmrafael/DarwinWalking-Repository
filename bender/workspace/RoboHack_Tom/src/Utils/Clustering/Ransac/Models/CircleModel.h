/*
 * CircleModel.h
 *
 *  Created on: Feb 22, 2012
 *      Author: Jan Carstensen
 */

#ifndef CIRCLEMODEL_H_
#define CIRCLEMODEL_H_

#include "AbstractModel.h"
#include <cstdlib>
#include <list>
#include "../../../../Representations/Point.h"

using namespace std;

class CircleModel: public AbstractModel {
public:

	CircleModel();
	CircleModel( const Point &center, int radius);
	~CircleModel();

	CircleModel* createNextModel(const list<Point>* data);
	double calculateError(const list<Point>* data);
	double calculateError(const Point* data);
	size_t getNumberOfFittedDataPoints(const list<Point>* data, double maximumError);

	list<Point>* getNotFittedPoints(const list<Point>* data, double maximumError);
	list<Point>* getFittedPoints(const list<Point>* data, double maximumError);

	const Point getCenter() const;
	int getRadius() const;

private:
	Point mCenter;
	int mRadius;

};

#endif /* CircleModel_H_ */
