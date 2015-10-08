/*
 * LineModel.h
 *
 *  Created on: 07.01.2014
 *      Author: Oliver Krebs
 */

#ifndef LINEMODEL_H_
#define LINEMODEL_H_

#include "AbstractModel.h"
#include <cstdlib>
#include <list>
#include "../../../../Representations/Point.h"

using namespace std;

/**
 * LineModel is a model for a line
 */
class LineModel: public AbstractModel {
public:
	/**
	 * Default-Constructor
	 */
	LineModel();

	/**
	 * Constructor
	 * @param startPoint	start point
	 * @param endPoint		end point
	 */
	LineModel(const Point &startPoint, const Point &endPoint);

	/**
	 * Destructor
	 */
	~LineModel();

	/**
	 * create next model
	 * @see AbstractModel::createNextModel
	 */
	LineModel* createNextModel(const list<Point>* data) override;

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
	 * calculate left point
	 * @param data			given point cloud
	 * @param maximumError	maximal error
	 * @return left point
	 */
	Point calcLeftPoint(const list<Point>* data, double maximumError);

	/**
	 * calculate right point
	 * @param data			given point cloud
	 * @param maximumError	maximal error
	 * @return right point
	 */
	Point calcRightPoint(const list<Point>* data, double maximumError);

	/**
	 * get start point
	 * @return start point
	 */
	const Point getStartPoint() const;

	/**
	 * get end point
	 * @return end point
	 */
	const Point getEndPoint() const;

	/**
	 * get x to given y
	 * @param y		given y
	 * @return x to given y
	 */
	int getX(int y) const;

	/**
	 * get y to given x
	 * @param x		given x
	 * @return y to given x
	 */
	int getY(int x) const;

private:
	void update();

	Point mStartPoint;
	Point mEndPoint;
	double mSlope;
	double mYIntercept;

};

#endif /* LINEMODEL_H_ */
