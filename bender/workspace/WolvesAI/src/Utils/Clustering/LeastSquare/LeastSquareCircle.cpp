/*
 * LeastSquareCircle.c
 *
 *  Created on: 03.02.2015
 *      Author: Oliver Krebs
 */

#include "LeastSquareCircle.h"
#include <cmath>

LeastSquareCircle::LeastSquareCircle(const std::list<Point>* data)
:	mData( data)
{

}

/*
 * Source: http://www.dtcenter.org/met/users/docs/write_ups/circle_fit.pdf
 */
CircleModel *LeastSquareCircle::run()
{
	double size = (double)mData->size();
	double xMean = 0.0f;
	double yMean = 0.0f;

	std::list<Point>::const_iterator it;
	for( it = mData->begin(); it != mData->end(); ++it) {
		xMean += (double)it->getX();
		yMean += (double)it->getY();
	}
	xMean /= size;
	yMean /= size;

	double Suu  = 0.0f;
	double Suv  = 0.0f;
	double Svv  = 0.0f;
	double Suuu = 0.0f;
	double Svvv = 0.0f;
	double Suvv = 0.0f;
	double Svuu = 0.0f;
	for( it = mData->begin(); it != mData->end(); ++it) {
		double ui = (double)it->getX() - xMean;
		double vi = (double)it->getY() - yMean;
		Suu += ui * ui;
		Suv += ui * vi;
		Svv += vi * vi;
		Suuu += ui * ui * ui;
		Svvv += vi * vi * vi;
		Suvv += ui * vi * vi;
		Svuu += vi * ui * ui;
	}

	double divisor = (2.0f * Suv * Suv) - (2.0f * Suu * Svv);
	double uc = -(Suv * (-Svvv-Svuu) + (Suvv * Svv) + (Suuu * Svv));
	double vc = Suu * (-Svvv-Svuu) + (Suvv * Suv) + (Suuu * Suv);
	uc /= divisor;
	vc /= divisor;

	double xc = uc + xMean;
	double yc = vc + yMean;
	double radius = sqrt((uc * uc) + (vc * vc) + ((Suu+Svv) / size));
	return new CircleModel(Point((int)xc, (int)yc), (int)radius);
}
