/*
 * Position.cpp
 *
 *  Created on: 26.06.2014
 *      Author: Oliver Krebs
 */

#include "Position.h"
//#include "../Messages/Representations.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"

Position::Position()
:	Vector(),
 	mAccuracy( 0.5)
{

}

Position::Position(const Vector &vec)
:	Vector(vec),
 	mAccuracy( 0.5)
{

}

Position::Position(const Position &pos)
:	Vector(pos.mAngle, pos.mLength),
 	mAccuracy( pos.mAccuracy)
{

}

Position::~Position()
{

}

void Position::setAccuracy(double accuracy)
{
	if( accuracy > 1.0) {
		mAccuracy = 1.0;
	} else if( accuracy < 0.0) {
		mAccuracy = 0.0;
	} else {
		mAccuracy = accuracy;
	}
}

double Position::getAccuracy() const
{
	return mAccuracy;
}

void Position::writeProtoBuf(protobuf::Position* position) const
{
	position->set_angle(mAngle);
	position->set_length(mLength);
	position->set_accuracy(mAccuracy);
}

void Serializer<Position>::serialize(const Position& representation,
		ostream& stream) {
	protobuf::Position position;

	representation.writeProtoBuf(&position);

	OstreamOutputStream buf(&stream);
	position.SerializeToZeroCopyStream(&buf);
}

void Serializer<Position>::deserialize(istream& stream, Position& representation) {
	protobuf::Position position;

	IstreamInputStream buf(&stream);
	position.ParseFromZeroCopyStream(&buf);

	representation.updateVector(position.angle(), position.length());
	representation.setAccuracy(position.accuracy());
}
