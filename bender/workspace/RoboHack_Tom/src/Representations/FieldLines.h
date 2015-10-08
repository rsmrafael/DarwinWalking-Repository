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

class FieldLines: public Printable {
public:
	FieldLines();
	FieldLines(const FieldLines& cSource);
	virtual ~FieldLines();
	FieldLines& operator=(const FieldLines& cSource);


	void clearPoints();
	void addEdgePoint(Point point);
	const std::list<Point>* getEdgePoints() const { return mEdgePoints;}
	bool isPointInField(Point point, int error) const;
	int getY(const int x) const;

	virtual void print(std::ostream& stream) const {

	}

private:
	std::list<Point>* mEdgePoints;
};

template<>
class Serializer<FieldLines> {
public:
	static void serialize(const FieldLines& representation, std::ostream& stream);
	static void deserialize(std::istream& stream, const FieldLines& representation);
};

#endif /* FIELDLINES_H_ */
