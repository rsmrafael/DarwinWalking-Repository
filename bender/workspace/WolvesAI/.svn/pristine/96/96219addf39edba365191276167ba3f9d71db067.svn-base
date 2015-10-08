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

class Point: public Printable {
public:
	Point(int x, int y);
	Point();
	Point(const Point& point);
	virtual ~Point();

	Point& operator=(const Point& cSource);

	bool operator==(const Point &p) const;
	bool equals(Point p) const;

	virtual void print(std::ostream& stream) const {
		stream << "(" << mX << "|" << mY << ")";
	}

	int getX() const {return mX;}
	int getY() const {return mY;}

	void updatePoint(int x, int y);
	double calcluateDistanceTo(const Point &point) const;

private:
	int mX;
	int mY;
};

template<>
	class Serializer<Point> {
	public:
		static void serialize(const Point& representation, std::ostream& stream);
		static void deserialize(std::istream& stream, Point& representation);
	};

#endif /* POINT_H_ */
