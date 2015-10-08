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

/**
 * Representation for a 2D vector given by angle and length
 */
class Vector: public Printable {
public:

	/**
	 * Constructor
	 */
	Vector();

	/**
	 * Constructor
	 * Angle in radians, length in mm
	 */
	Vector(double angle, double length);

	/**
	 * Constructor
	 * @param vector	protobuf vector
	 */
	Vector(protobuf::Vector2D vector);

	/**
	 * Copy-Constructor
	 * @param vector
	 */
	Vector(const Vector& vector);

	/**
	 * Destructor
	 */
	virtual ~Vector();

	/**
	 * print to stream
	 * @param stream 	output stream
	 */
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
	 * copies vector to another vector reference
	 */
	void copyTo(Vector &vec) const;

	/**
	 * angle in radians, length in mm
	 */
	void updateVector(double angle, double length);

	/**
	 * returns distance to another vector in mm
	 */
	double calcluateDistanceTo(const Vector& vect) const;

	/**
	 * calculate the sum with another vector
	 * @param other		vector
	 * @return sum vector
	 */
	Vector sum(const Vector& other) const;

	/**
	 * calculate the subtraction of this vector and another
	 * @param other 	vector
	 * @return subtract vector
	 */
	Vector subtract(const Vector& other) const;

	/**
	 * scale this vector with a scalar
	 * @return scaled vector
	 */
	Vector scale(double scalar) const;

	/**
	 * get unit vector
	 * @return unit vector of this vector
	 */
	Vector unit() const;

	/**
	 * write to protobuf
	 * @param object	protobuf vector
	 */
	void writeProtoBuf(protobuf::Vector2D* object) const;

protected:
	double mAngle;		//!< angle in radians
	double mLength;		//!< length in mm

	void boundAngle();	//!< make sure the angle is between -pi and pi

};

/**
 * spezialization of Serializer for Vector
 */
template<>
class Serializer<Vector> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const Vector& representation, ostream& stream);
	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(istream& stream, Vector& representation);
};

#endif /* Vector_H_ */
