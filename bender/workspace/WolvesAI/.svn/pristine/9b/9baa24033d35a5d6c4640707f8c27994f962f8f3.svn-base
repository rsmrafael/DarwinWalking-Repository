/*
 * Vector.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: jan
 */

#include "Vector.h"
#include "../Messages/Representations.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"
#include "../Messages/Vector2D.pb.h"
#include "../Utils/Constants.h"
#include <cmath>

Vector::Vector() {
	mAngle = 0.0;
	mLength = 0.0;
}

Vector::Vector(protobuf::Vector2D vec) {
	mAngle = vec.angle();
	mLength = vec.length();
}

Vector::Vector(double angle, double length) {
	mAngle = angle;
	mLength = length;
}

Vector::Vector(const Vector& vec) : Printable(vec) {
	mAngle = vec.mAngle;
	mLength = vec.mLength;
}

Vector::~Vector() {
}

void Vector::copyTo(Vector &vec) const {
	vec.mAngle = mAngle;
	vec.mLength = mLength;
}

void Vector::updateVector(double angle, double length) {
	mAngle = angle;
	if( length < 0.0) {
		mAngle += PI;
		mLength = -length;
	} else {
		mLength = length;
	}
}

double Vector::calcluateDistanceTo(const Vector& vect) const {
	Vector vec = subtract(vect);

	return vec.mLength;
}


Vector Vector::sum(const Vector& other) const {
  // Transform to Cartesian coordinates
  double x = getX() + other.getX();
  double y = getY() + other.getY();

  // Transform back to polar coordinates
  return Vector(atan2(x, y), sqrt(x * x + y * y));
 }

 /** Returns the difference of the two given vectors i.e. v1 - v2 */
Vector Vector::subtract(const Vector& other) const {
	// Transform to Cartesian coordinates
	double x = getX() - other.getX();
	double y = getY() - other.getY();

	// Transform back to polar coordinates
	return Vector(atan2(x, y), sqrt(x * x + y * y));
 }

 /** Scale the vector (multiply length times scalar) */
Vector Vector::scale(double scalar) const {
  return Vector(mAngle, mLength * scalar);
 }

 /** Adjust the length of the vector to 1 unit i.e. v / |v| */
Vector Vector::unit() const{
  double length = 1.0;;
  double angle = mAngle;
  // Adjust the length of a vector to be always positive by inverting the direction (angle plus 180 degrees if necessary
  if (mLength < 0) {
   length *= -1.0;
   angle += PI;
  }

  return Vector(angle, length);
 }

void Vector::boundAngle() {
  while (mAngle > PI) {
   mAngle -= TWICE_PI;
  }
  while (mAngle < -PI) {
   mAngle += TWICE_PI;
  }
 }

double Vector::getX() const {
	return mLength * cos(mAngle);
}

double Vector::getY() const {
	return mLength * sin(mAngle);
}

 void Vector::writeProtoBuf(protobuf::Vector2D* vector2D) const{
	vector2D->set_angle(mAngle);
	vector2D->set_length(mLength);
}

void Serializer<Vector>::serialize(const Vector& representation,
		ostream& stream) {
	protobuf::Vector2D vector2D;

	representation.writeProtoBuf(&vector2D);

	OstreamOutputStream buf(&stream);
	vector2D.SerializeToZeroCopyStream(&buf);
}

void Serializer<Vector>::deserialize(istream& stream, Vector& representation) {
	protobuf::Vector2D vector_;

	IstreamInputStream buf(&stream);
	vector_.ParseFromZeroCopyStream(&buf);

	representation.updateVector(vector_.angle(), vector_.length());

	//representation = Vector(vector_);
}
