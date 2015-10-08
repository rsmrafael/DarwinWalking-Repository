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

/**
 * A line in a 2D world (represented by two points)
 */
class Line: public Printable {
public:
	/**
	 * Constructor
	 */
	Line();

	/**
	 * Constructor
	 * @param startPoint	first point of line
	 * @param endPoint		second point of line
	 */
	Line(const Point& startPoint, const Point& endPoint);

	/**
	 * Constructor
	 * @param cSource		source line
	 */
	Line(const Line& cSource);

	/**
	 * Destructor
	 */
	virtual ~Line();

	/**
	 * assignment operator, copy data from another line
	 */
	Line& operator=(const Line& cSource);

	/**
	 * update the line
	 * @param one	first point
	 * @param two	second point
	 */
	void updateLine(const Point& one, const Point& two);

	/**
	 * return start point
	 */
	const Point getStartPoint() const {
		return mStartPoint;
	}

	/**
	 * return end point
	 */
	const Point getEndPoint() const {
		return mEndPoint;
	}

	/**
	 * get x coordinate to y
	 * @param y 	given y coordinate
	 * @return x coordinate to given y
	 */
	int getX(int y) const;

	/**
	 * get y coordinate to x
	 * @param x		given x coordinate
	 * @return y coordinate to given x
	 */
	int getY(int x) const;

	/**
	 * test if point is below this line
	 * @param point 	point to test
	 * @return true if given point is below the line
	 */
	bool isPointBelow(const Point* point) const;

	/**
	 * print to an output stream
	 * @param stream 	output stream
	 */
	virtual void print(std::ostream& stream) const {
		mStartPoint.print(stream);
		stream << " -> ";
		mEndPoint.print(stream);
		stream << " ";
	}

private:
	void update();		//!< update line

	Point mStartPoint;	//!< start point
	Point mEndPoint;	//!< end point
	double mSlope;		//!< slope of line
	double mYIntercept;	//!< y-intercept
};

/**
 * spezialization of Serializer for Line
 */
template<>
class Serializer<Line> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const Line& representation, std::ostream& stream);
	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(std::istream& stream, Line& representation);
};

#endif /* LINE_H_ */
