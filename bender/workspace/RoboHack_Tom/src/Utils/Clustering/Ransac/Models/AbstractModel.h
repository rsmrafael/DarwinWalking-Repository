/*
 * AbstractModel.h
 *
 *  Created on: Jan 16, 2012
 *      Author: Jan Carstensen
 */

#ifndef AbstractModel_H_
#define AbstractModel_H_

#include <list>
#include "../../../../Representations/Point.h"


class AbstractModel {
public:

	AbstractModel(){
	}

	virtual ~AbstractModel(){
	};

	virtual AbstractModel* createNextModel(const list<Point>* data) = 0;
	virtual double calculateError(const list<Point>* data) = 0;
	virtual double calculateError(const Point* point) = 0;

	virtual list<Point>* getFittedPoints(const list<Point>* data, double maximumError) = 0;
	virtual list<Point>* getNotFittedPoints(const list<Point>* data, double maximumError) = 0;

	virtual size_t getNumberOfFittedDataPoints(const list<Point>* data, double maximumError) = 0;

protected:

};


#endif /* AbstractModel_H_ */
