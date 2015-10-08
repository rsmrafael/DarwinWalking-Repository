/*
 * Position.h
 *
 *  Created on: 26.06.2014
 *      Author: Oliver Krebs
 */

#ifndef POSITION_H_
#define POSITION_H_

#include "Vector.h"
#include "../Messages/Position.pb.h"

class Position : public Vector {
public:
	Position();
	Position(const Vector &vec);
	Position(const Position &pos);
	virtual ~Position();

	void setAccuracy(double accuracy);
	double getAccuracy() const;

	void writeProtoBuf(protobuf::Position* object) const;

protected:
	double mAccuracy;
};

template<>
	class Serializer<Position> {
	public:
		static void serialize(const Position& representation, ostream& stream);
		static void deserialize(istream& stream, Position& representation);
	};

#endif /* POSITION_H_ */
