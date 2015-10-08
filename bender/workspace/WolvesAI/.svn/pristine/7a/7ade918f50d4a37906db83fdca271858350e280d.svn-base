/*
 * Line.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef LINE_H_
#define LINE_H_

#include "Point.h"
#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"

class Line: public Printable {
public:
	Line();
	Line(const Point& startPoint, const Point& endPoint);
	Line(const Line& cSource);
	virtual ~Line();

	Line& operator=(const Line& cSource);

	void updateLine(const Point& one, const Point& two);

	const Point getStartPoint() const {
		return mStartPoint;
	}
	const Point getEndPoint() const {
		return mEndPoint;
	}

	int getX(int y) const;
	int getY(int x) const;

	bool isPointBelow(const Point* point) const;

	virtual void print(std::ostream& stream) const {
		mStartPoint.print(stream);
		stream << " -> ";
		mEndPoint.print(stream);
		stream << " ";
	}

private:
	void update();

	Point mStartPoint;
	Point mEndPoint;
	double mSlope;
	double mYIntercept;
};

template<>
class Serializer<Line> {
public:
	static void serialize(const Line& representation, std::ostream& stream);
	static void deserialize(std::istream& stream, Line& representation);
};

#endif /* LINE_H_ */
