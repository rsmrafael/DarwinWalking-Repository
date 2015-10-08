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

/**
 * A position is a vector with a given accuracy (used for localization).
 */
class Position : public Vector {
public:
	/**
	 * Constructor
	 */
	Position();

	/**
	 * Constructor
	 * @param vec	vector
	 */
	Position(const Vector &vec);

	/**
	 * Copy-Constructor
	 * @param pos	position
	 */
	Position(const Position &pos);

	/**
	 * Destructor
	 */
	virtual ~Position();

	/**
	 * set the accuracy of the position
	 * @param accuracy		accuracy between 0.0 (worst) and 1.0 (best)
	 */
	void setAccuracy(double accuracy);

	/**
	 * get the accuracy
	 * @return accuracy between 0.0 and 1.0
	 */
	double getAccuracy() const;

	/**
	 * write to protobuf
	 * @param object	protobuf representation
	 */
	void writeProtoBuf(protobuf::Position* object) const;

protected:
	double mAccuracy;		//!< accuracy of position in percent (0.0 to 1.0)
};

/**
 * spezialization of Serializer for Position
 */
template<>
class Serializer<Position> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const Position& representation, ostream& stream);
	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(istream& stream, Position& representation);
};

#endif /* POSITION_H_ */
