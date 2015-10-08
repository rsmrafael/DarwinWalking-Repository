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

/**
 * AbstractModel is an interface for shape models like line or circle for use in RANSAC.
 */
class AbstractModel {
public:

	AbstractModel() {}

	virtual ~AbstractModel() {}

	/**
	 * create next model
	 * @param data		given point cloud
	 * @return the created model (you have to release it with delete!)
	 */
	virtual AbstractModel* createNextModel(const list<Point>* data) = 0;

	/**
	 * calculate the error for a point cloud
	 * @param data 		given point cloud
	 * @return error value
	 */
	virtual double calculateError(const list<Point>* data) = 0;

	/**
	 * calculate the error for one point
	 * @param point		given point
	 * @return error value
	 */
	virtual double calculateError(const Point* point) = 0;

	/**
	 * get all fitted points
	 * @param data			given point cloud
	 * @param maximumError	maximal error
	 * @return point cloud of fitted points
	 */
	virtual list<Point>* getFittedPoints(const list<Point>* data, double maximumError) = 0;

	/**
	 * get all not fitted points
	 * @param data			given point cloud
	 * @param maximumError	maximal error
	 * @return point cloud of not fitted points
	 */
	virtual list<Point>* getNotFittedPoints(const list<Point>* data, double maximumError) = 0;

	/**
	 * get number of fitted data points
	 * @param data			given point cloud
	 * @param maximumError	maximal error
	 * @return number of fitted points
	 */
	virtual size_t getNumberOfFittedDataPoints(const list<Point>* data, double maximumError) = 0;

};


#endif /* AbstractModel_H_ */
