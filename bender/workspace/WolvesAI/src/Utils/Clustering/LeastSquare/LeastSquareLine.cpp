/*
 * LeastSquareLine.cpp
 *
 *  Created on: 26.02.2015
 *      Author: Oliver Krebs
 */

#include "LeastSquareLine.h"
#include <limits>

LeastSquareLine::LeastSquareLine(const std::list<Point>* data)
:	mData(data) {

}

// Source: http://hotmath.com/hotmath_help/topics/line-of-best-fit.html
LineModel *LeastSquareLine::run() {
	double size = (double)mData->size();
	double minX = std::numeric_limits<double>::max();
	double maxX = -std::numeric_limits<double>::max();
	double xMean = 0.0f;
	double yMean = 0.0f;

	std::list<Point>::const_iterator it;
	for( it = mData->begin(); it != mData->end(); ++it) {
		double x = (double)it->getX();
		double y = (double)it->getY();
		xMean += x;
		yMean += y;
		if( x < minX) {
			minX = x;
		} else if( x > maxX) {
			maxX = x;
		}
	}
	xMean /= size;
	yMean /= size;

	double dividend = 0.0f;
	double divisor = 0.0f;
	for( it = mData->begin(); it != mData->end(); ++it) {
		double dx = ((double)it->getX() - xMean);
		double dy = ((double)it->getY() - yMean);
		dividend += dx * dy;
		divisor += dx * dx;
	}
	double m = dividend / divisor;
	double b = yMean + (m * xMean);
	int y1 = (int)(minX * m + b);
	int y2 = (int)(maxX * m + b);
	return new LineModel(Point((int)minX, y1), Point((int)maxX, y2));
}
