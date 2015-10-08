/*
 * FieldLines.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef FIELDLINES_H_
#define FIELDLINES_H_

#include "Point.h"
#include "Line.h"
#include <list>
#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"

/**
 * Representation of field lines
 */
class FieldLines: public Printable {
public:

	/**
	 * Constructor
	 */
	FieldLines();

	/**
	 * Constructor
	 * @param cSource 	source for copy
	 */
	FieldLines(const FieldLines& cSource);

	/**
	 * Destructor
	 */
	virtual ~FieldLines();

	/**
	 * copy function
	 */
	FieldLines& operator=(const FieldLines& cSource);

	/**
	 * clear all points
	 */
	void clearPoints();

	/**
	 * add one edge point
	 * @param point		point to be add
	 */
	void addEdgePoint(Point point);

	/**
	 * return pointer to all edge points
	 */
	const std::list<Point>* getEdgePoints() const { return mEdgePoints;}

	/**
	 * test if point is in field (under field line)
	 * @param point		point to be checked
	 * @param error		maximal error in y
	 */
	bool isPointInField(const Point &point, int error) const;

	/**
	 * return y value for given x
	 * @param x			x value
	 */
	int getY(const int x) const;

	/**
	 * print to stream
	 * @param stream		stream for print
	 */
	virtual void print(std::ostream& stream) const {

	}

private:
	std::list<Point>* mEdgePoints;		//!< all edge points
};

/**
 * spezialization of Serializer for FieldLines
 */
template<>
class Serializer<FieldLines> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const FieldLines& representation, std::ostream& stream);

	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(std::istream& stream, const FieldLines& representation);
};

#endif /* FIELDLINES_H_ */
