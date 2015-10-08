/*
 * Point.h
 *
 *  Created on: Sep 1, 2012
 *      Author: jan
 */

#ifndef POINT_H_
#define POINT_H_

#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"
#include <cmath>

/**
 *	A Point in a 2D world (represented by x and y coordinates)
 */
class Point: public Printable {
public:

	/**
	 * Constructor
	 */
	Point();

	/**
	 * Constructor
	 * @param x		x coordinate
	 * @param y		y coordinate
	 */
	Point(int x, int y);

	/**
	 * Copy-Constructor
	 * @param point		point
	 */
	Point(const Point& point);

	/**
	 * Destructor
	 */
	virtual ~Point();

	/**
	 * assignment operator, copy data from another point
	 */
	Point& operator=(const Point& cSource);

	/**
	 * compare operator
	 */
	bool operator==(const Point &p) const;

	/**
	 * compare to another point
	 * @param p		point to compare with
	 * @return true if point has the same coordinates
	 */
	bool equals(Point p) const;

	/**
	 * print point to stream
	 * @param stream 	output stream
	 */
	virtual void print(std::ostream& stream) const {
		stream << "(" << mX << "|" << mY << ")";
	}

	/**
	 * get x coordinate of point
	 * @return x coordinate
	 */
	int getX() const {return mX;}

	/**
	 * get y coordinate of point
	 * @return y coordinate
	 */
	int getY() const {return mY;}

	/**
	 * update the point coordinates
	 * @param x 	x coordinate
	 * @param y 	y coordinate
	 */
	void updatePoint(int x, int y);

	/**
	 * calculate distance to another point
	 * @param point		point
	 * @return distance to the other point
	 */
	inline double calculateDistanceTo(const Point &point) const {
		return sqrt((mX-point.mX)*(mX-point.mX) + (mY-point.mY)*(mY-point.mY));
	}

private:
	int mX;	//!< x coordinate
	int mY; //!< y coordinate
};

/**
 * spezialization of Serializer for Point
 */
template<>
class Serializer<Point> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const Point& representation, std::ostream& stream);
	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(std::istream& stream, Point& representation);
};

#endif /* POINT_H_ */
