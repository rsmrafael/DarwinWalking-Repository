/*
 * LeastSquareCircle.h
 *
 *  Created on: 03.02.2015
 *      Author: Oliver Krebs
 */

#ifndef LEASTSQUARECIRCLE_H_
#define LEASTSQUARECIRCLE_H_

#include <list>
#include "../../../Representations/Point.h"
#include "../Ransac/Models/CircleModel.h"

/**
 * LeastSuareCircle creates a CircleModel with LeastSquare-algorithm
 */
class LeastSquareCircle {
public:
	/**
	 * Constructor
	 * @param data		given points
	 */
	LeastSquareCircle(const std::list<Point>* data);

	/**
	 * run algorithm and create circle model
	 * @return calculated circle model
	 */
	CircleModel *run();

private:
	const std::list<Point>* mData;
};


#endif /* LEASTSQUARECIRCLE_H_ */
