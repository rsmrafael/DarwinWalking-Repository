/*
 * Point.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: jan
 */

#include "Point.h"
#include "../Messages/Point.pb.h"
#include "../Messages/Representations.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"
#include <math.h>
#include <cmath>

Point::Point(){
	mX = 0;
	mY = 0;
}

Point::Point(int x, int y) {
	mX = x;
	mY = y;
}

Point::Point(const Point& point)
:	Printable(point)
{
	mX = point.mX;
	mY = point.mY;
}

Point::~Point() {
}

Point& Point::operator=(const Point& cSource) {
	if (this != &cSource) {
		mX = cSource.mX;
		mY = cSource.mY;
	}
	return *this;
}

void Point::updatePoint(int x, int y) {
	mX = x;
	mY = y;
}

void Serializer<Point>::serialize(const Point& representation,
		ostream& stream) {
	protobuf::Point point;

	point.set_x(representation.getX());
	point.set_y(representation.getY());

	OstreamOutputStream buf(&stream);
	point.SerializeToZeroCopyStream(&buf);
}

void Serializer<Point>::deserialize(istream& stream, Point& representation) {
	protobuf::Point point;

	IstreamInputStream buf(&stream);
	point.ParseFromZeroCopyStream(&buf);

	int x = point.x();
	int y = point.y();

	representation.updatePoint(x, y);
}

bool Point::operator==(const Point &p) const{

	if (p.getX() == getX() && p.getY() == getY()){
		//Debugger::DEBUG("Point", "p1 %i/%i , p2: %i/%i - true", mX, mY, p.getX(), p.getY());

		return true;
	} else {

	//	Debugger::DEBUG("Point", "p1 %i/%i , p2: %i/%i - false", mX, mY, p.getX(), p.getY());
		return false;
	}
}

bool Point::equals(Point p) const{
	if (p.getX() == getX() && p.getY() == getY()){
			Debugger::DEBUG("Point", "p1 %i/%i , p2: %i/%i - true", mX, mY, p.getX(), p.getY());

			return true;
		} else {

		//	Debugger::DEBUG("Point", "p1 %i/%i , p2: %i/%i - false", mX, mY, p.getX(), p.getY());
			return false;
		}
}
