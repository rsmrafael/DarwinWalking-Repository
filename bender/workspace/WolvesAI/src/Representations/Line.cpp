/*
 * Line.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#include "Line.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"
#include "../Messages/Line.pb.h"

using namespace std;

Line::Line()
:	mStartPoint(0,0),
 	mEndPoint(0,0),
 	mSlope(0.0),
 	mYIntercept(0.0)
{
	update();
}

Line::Line(const Point& startPoint, const Point& endPoint)
:	mStartPoint(startPoint),
 	mEndPoint(endPoint),
 	mSlope(0.0),
 	mYIntercept(0.0)
{
	update();
}

Line::Line(const Line& cSource)
:	Printable(cSource),
	mStartPoint(cSource.mStartPoint),
 	mEndPoint(cSource.mEndPoint),
 	mSlope(cSource.mSlope),
 	mYIntercept(cSource.mYIntercept)
{

}

Line::~Line()
{
}

Line& Line::operator=(const Line& cSource) {
	if (this != &cSource) // protect against invalid self-assignment
	{
		mStartPoint = cSource.mStartPoint;
		mEndPoint = cSource.mEndPoint;
		mSlope = cSource.mSlope;
		mYIntercept = cSource.mYIntercept;
	}
	// by convention, always return *this
	return *this;
}

void Line::updateLine(const Point& one, const Point& two) {
	if (one.getX() < two.getX()) {
		mStartPoint = Point(one);
		mEndPoint = Point(two);
	} else {
		mStartPoint = Point(two);
		mEndPoint = Point(one);
	}
	update();
}

bool Line::isPointBelow(const Point* point) const {
	if( point->getY() > getY(point->getX())) {
		return true;
	}
	return false;
}

void Line::update() {
	double dx = (double)(mEndPoint.getX() - mStartPoint.getX());
	double dy = (double)(mEndPoint.getY() - mStartPoint.getY());
	mSlope = dy / dx;
	mYIntercept = mStartPoint.getY() - mSlope * (mStartPoint.getX());
}

int Line::getX(int y) const {
	return (int)(((double)y - mYIntercept) / mSlope);
}

int Line::getY(int x) const {
	return (int)(mSlope*(double)x + mYIntercept);
}

void Serializer<Line>::serialize(const Line& representation, ostream& stream) {
	protobuf::Line line;
	protobuf::Point* startPoint;
	protobuf::Point* endPoint;

	startPoint = line.mutable_startpoint();
	startPoint->set_x(representation.getStartPoint().getX());
	startPoint->set_y(representation.getStartPoint().getY());
	endPoint = line.mutable_endpoint();
	endPoint->set_x(representation.getEndPoint().getX());
	endPoint->set_y(representation.getEndPoint().getY());

	OstreamOutputStream buf(&stream);
	line.SerializeToZeroCopyStream(&buf);
}

void Serializer<Line>::deserialize(istream& stream, Line& representation) {
	protobuf::Line line;

	IstreamInputStream buf(&stream);
	line.ParseFromZeroCopyStream(&buf);

	int sx = line.startpoint().x();
	int sy = line.startpoint().y();
	int ex = line.endpoint().x();
	int ey = line.endpoint().y();

	representation.updateLine( Point(sx, sy), Point(ex, ey));

}
