/*
 * Vector.h
 *
 *  Created on: Sep 1, 2012
 *      Author: jan
 */

#ifndef Vector_H_
#define Vector_H_

#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"
#include "../Messages/Vector2D.pb.h"

class Vector: public Printable {
public:
	Vector();

	/**
	 * Angle in radians, length in mm
	 */
	Vector(double angle, double length);
	Vector(protobuf::Vector2D vector);
	Vector(const Vector& Vector);
	virtual ~Vector();

	virtual void print(ostream& stream) const {
		stream << "Angle: " << mAngle << ", Length: " << mLength << " ";
	}

	/**
	 * returns angle of vector in radians
	 */
	double getAngle() const {return mAngle;}

	/**
	 * returns length of vector in mm
	 */
	double getLength() const {return mLength;}

	/**
	 * returns x-coordinate in mm
	 */
	double getX() const;

	/**
	 * returns y-coordinate in mm
	 */
	double getY() const;

	/**
	 * angle in radians, length in mm
	 */
	void updateVector(double angle, double length);

	/**
	 * returns distance to another vector in mm
	 */
	double calcluateDistanceTo(const Vector& vect) const;

	Vector sum(const Vector& other) const;
	Vector subtract(const Vector& other) const;
	Vector scale(double scalar) const;
	Vector unit() const;

	void writeProtoBuf(protobuf::Vector2D* object) const;

protected:
	double mAngle;
	double mLength;

	void boundAngle();

};

template<>
	class Serializer<Vector> {
	public:
		static void serialize(const Vector& representation, ostream& stream);
		static void deserialize(istream& stream, Vector& representation);
	};

#endif /* Vector_H_ */
