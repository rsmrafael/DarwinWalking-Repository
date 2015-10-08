/*
 * ServoStatus.h
 *
 *  Created on: 26.02.2013
 *      Author: Stefan Krupop
 */

#ifndef SERVOSTATUS_H_
#define SERVOSTATUS_H_

#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"
#include "../Messages/ServoStatus.pb.h"

/**
 * Representation of servo status
 */
class ServoStatus {
public:

	/**
	 * Constructor
	 */
	ServoStatus();

	/**
	 * Constructor
	 * @param id 	id of servo
	 */
	ServoStatus(uint8_t id);

	/**
	 * Constructor
	 * @param status	protobuf servoStatus
	 */
	ServoStatus(const protobuf::ServoStatus* status);

	/**
	 * Destructor
	 */
	virtual ~ServoStatus();

	/**
	 * set the status of the servo
	 * @param maxPositionError		max position error of servo
	 * @param temperature			temperature of servo
	 * @param voltage				voltage of servo
	 */
	void setStatus(double maxPositionError, double temperature, double voltage);

	/**
	 * set the id of the servo
	 * @param id		the new id of the servo
	 */
	void setID(int id);

	/**
	 * get the id of the servo
	 * @return id of the servo
	 */
	uint8_t getID() const {
		return mID;
	}

	/**
	 * get the max position error
	 * @return max position error
	 */
	double getMaxPositionError() const {
		return mMaxPositionError;
	}

	/**
	 * get the temperature of the servo
	 * @return temperature of the servo
	 */
	double getTemperature() const {
		return mTemperature;
	}

	/**
	 * get the voltage of the servo
	 * @return voltage of the servo
	 */
	double getVoltage() const {
		return mVoltage;
	}

	/**
	 * write to protobuf representation
	 * @param status	protobuf representation
	 */
	void writeProtoBuf(protobuf::ServoStatus* status) const;

private:
	uint8_t mID;				//!< ID of servo
	double mMaxPositionError;	//!< max position error
	double mTemperature;		//!< temperature
	double mVoltage;			//!< voltage

};

/**
 * spezialization of Serializer for ServoStatus
 */
template<>
class Serializer<ServoStatus> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const ServoStatus& representation, ostream& stream);
	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(istream& stream, ServoStatus& representation);
};

#endif /* SERVOSTATUS_H_ */
