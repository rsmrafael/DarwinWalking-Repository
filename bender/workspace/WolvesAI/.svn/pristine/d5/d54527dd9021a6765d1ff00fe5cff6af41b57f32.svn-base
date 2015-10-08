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


class ServoStatus{
public:
	ServoStatus();
	ServoStatus(uint8_t id);
	ServoStatus(const protobuf::ServoStatus* status);
	virtual ~ServoStatus();

	void setStatus(double maxPositionError, double temperature, double voltage);
	void setID(int id);

	uint8_t getID() const {
		return mID;
	}
	double getMaxPositionError() const {
		return mMaxPositionError;
	}
	double getTemperature() const {
		return mTemperature;
	}
	double getVoltage() const {
		return mVoltage;
	}

	void writeProtoBuf(protobuf::ServoStatus* status) const;

private:
	uint8_t mID;
	double mMaxPositionError;
	double mTemperature;
	double mVoltage;

};

template<>
	class Serializer<ServoStatus> {
	public:
		static void serialize(const ServoStatus& representation, ostream& stream);
		static void deserialize(istream& stream, ServoStatus& representation);
	};

#endif /* SERVOSTATUS_H_ */
