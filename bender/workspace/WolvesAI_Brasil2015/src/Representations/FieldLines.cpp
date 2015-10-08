/*
 * FieldLines.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#include "FieldLines.h"
#include "../Messages/Line.pb.h"
#include "../Messages/FieldLines.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"

using namespace std;

FieldLines::FieldLines() {
	mEdgePoints = new list<Point>();
}

FieldLines::FieldLines(const FieldLines& cSource) :
		Printable(cSource) {
	mEdgePoints = new list<Point>();
	for (list<Point>::const_iterator pointIterator =
			cSource.getEdgePoints()->begin();
			pointIterator != cSource.getEdgePoints()->end(); ++pointIterator) {
		mEdgePoints->push_back((*pointIterator));
	}
}

FieldLines::~FieldLines() {
	mEdgePoints->clear();
	delete mEdgePoints;
}

FieldLines& FieldLines::operator=(const FieldLines& cSource) {
	if (this != &cSource) // protect against invalid self-assignment
			{
		// 1: allocate new memory and copy the elements
		list<Point>* new_data = new list<Point>();
		for (list<Point>::iterator pointIterator = mEdgePoints->begin();
				pointIterator != mEdgePoints->end(); ++pointIterator) {
			new_data->push_back((*pointIterator));
		}

		// 2: deallocate old memory
		mEdgePoints->clear();
		delete mEdgePoints;

		// 3: assign the new memory to the object
		mEdgePoints = new_data;
	}
	// by convention, always return *this
	return *this;
}

void FieldLines::clearPoints() {
	mEdgePoints->clear();
}

void FieldLines::addEdgePoint(Point point) {
	mEdgePoints->push_back(point);
}

bool FieldLines::isPointInField(const Point &point, int error) const {
	return (point.getY() > (getY(point.getX()) - error));
}

int FieldLines::getY(const int x) const {
	if( mEdgePoints->size() < 2) {
		return 0;
	}

	Point p1 = (*mEdgePoints->begin());
	Point p2;
	for (list<Point>::const_iterator it = mEdgePoints->begin();
			it != mEdgePoints->end(); ++it) {

		p2 = (*it);
		if (p2.getX() >= x) {
			break;
		}
		//	Debugger::DEBUG("FieldLines", "p1 %i" , p1.getX());
		p1 = p2;
	}

	if (x == p2.getX()) {
		return p2.getY();
	}

	//	Debugger::DEBUG("FieldLines", "p1x: %i p2x: %i", p1.getX(), p2.getX());

	double deltaY = p2.getY() - p1.getY();
	double deltaX = p2.getX() - p1.getX();

	//	Debugger::DEBUG("FieldLines", "dx: %f dy: %f", deltaX, deltaY);

	double m = deltaY / deltaX;
	int b = p1.getY() - (int) (m * p1.getX());

	//	Debugger::DEBUG("FieldLines", "m: %f b: %i", m, b);

	double yValueFunc = m * x + b;

	//	Debugger::DEBUG("FieldLines", "yValue: %f pointValue: %i", yValueFunc, point.getY());

	return (int)yValueFunc;
}

void Serializer<FieldLines>::serialize(const FieldLines& representation,
		ostream& stream) {

	protobuf::FieldLines fieldLines;

	const Point* fPoint = NULL;

	for (list<Point>::const_iterator pointIterator =
			representation.getEdgePoints()->begin();
			pointIterator != representation.getEdgePoints()->end();
			++pointIterator) {

		if (fPoint == NULL) {
			fPoint = &(*pointIterator);
		} else {

			protobuf::Line* line = fieldLines.add_line();

			protobuf::Point* startPoint = line->mutable_startpoint();
			protobuf::Point* endPoint = line->mutable_endpoint();

			startPoint->set_x(fPoint->getX());
			startPoint->set_y(fPoint->getY());

			endPoint->set_x((*pointIterator).getX());
			endPoint->set_y((*pointIterator).getY());

			fPoint = &(*pointIterator);
		}

	}

	OstreamOutputStream buf(&stream);
	fieldLines.SerializeToZeroCopyStream(&buf);
}

void Serializer<FieldLines>::deserialize(istream& stream,
		const FieldLines& representation) {
	protobuf::FieldLines fieldLines;

	IstreamInputStream buf(&stream);
	fieldLines.ParseFromZeroCopyStream(&buf);
	/**
	 for (int i = 0; i < fieldLines.line_size(); ++i) {
	 Point startPoint = new Point(fieldLines.line(i).startpoint().x(),
	 fieldLines.line(i).startpoint().y());
	 Point endPoint = new Point(fieldLines.line(i).endpoint().x(),
	 fieldLines.line(i).endpoint().y());
	 representation.addLine(new Line(startPoint, endPoint));
	 }
	 */
}
