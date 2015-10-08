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

/**
 * CircleModel is a model for circles
 */
class CircleModel: public AbstractModel {
public:

	/**
	 * Default-Constructor
	 */
	CircleModel();

	/**
	 * Constructor
	 * @param center		center point of circle
	 * @param radius		radius of circle
	 */
	CircleModel(const Point &center, int radius);

	/**
	 * Destructor
	 */
	~CircleModel();

	/**
	 * create next model
	 * @see AbstractModel::createNextModel
	 */
	CircleModel* createNextModel(const list<Point>* data) override;

	/**
	 * calculate error
	 * @see AbstractModel::calculateError(const list<Point>*)
	 */
	double calculateError(const list<Point>* data) override;

	/**
	 * calculate error
	 * @see AbstractModel::calculateError(const Point*)
	 */
	double calculateError(const Point* data) override;

	/**
	 * get number of fitted data points
	 * @see AbstractModel::getNumberOfFittedDataPoints
	 */
	size_t getNumberOfFittedDataPoints(const list<Point>* data, double maximumError) override;

	/**
	 * erase fitted points
	 * @param data				given point cloud
	 * @param maximumError		maximal error
	 */
	void eraseFittedPoints(list<Point>* data, double maximumError);

	/**
	 * get not fitted points
	 * @see AbstractModel::getNotFittedPoints
	 */
	list<Point>* getNotFittedPoints(const list<Point>* data, double maximumError) override;

	/**
	 * get fitted points
	 * @see AbstractModel::getFittedPoints
	 */
	list<Point>* getFittedPoints(const list<Point>* data, double maximumError) override;

	/**
	 * get center of circle
	 * @return center point
	 */
	const Point getCenter() const;

	/**
	 * get radius of circle
	 * @return radius
	 */
	int getRadius() const;

private:
	Point mCenter;
	int mRadius;

};

#endif /* CircleModel_H_ */
