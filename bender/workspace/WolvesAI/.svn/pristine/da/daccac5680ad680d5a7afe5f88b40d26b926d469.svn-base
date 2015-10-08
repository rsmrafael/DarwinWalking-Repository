/*
 * LeastSquareLine.h
 *
 *  Created on: 26.02.2015
 *      Author: Oliver Krebs
 */

#ifndef LEASTSQUARELINE_H_
#define LEASTSQUARELINE_H_

#include <list>
#include "../../../Representations/Point.h"
#include "../Ransac/Models/LineModel.h"

/**
 * LeastSuareLine creates a LineModel with LeastSquare-algorithm
 */
class LeastSquareLine {
public:
	/**
	 * Constructor
	 * @param data		given points
	 */
	LeastSquareLine(const std::list<Point>* data);

	/**
	 * run algorithm and create line model
	 * @return calculated line model
	 */
	LineModel *run();

private:
	const std::list<Point>* mData;
};

#endif /* LEASTSQUARELINE_H_ */
