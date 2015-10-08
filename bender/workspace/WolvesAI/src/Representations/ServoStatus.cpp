/*
 * ServoStatus.cpp
 *
 *  Created on: 26.02.2013
 *      Author: Stefan Krupop
 */

#include "ServoStatus.h"
#include "../Messages/Representations.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"

ServoStatus::ServoStatus() {
	mMaxPositionError = 0.0;
	mTemperature = 0.0;
	mVoltage = 0.0;
	mID = 0;
}

ServoStatus::ServoStatus(uint8_t id) :
		mID(id) {
	mMaxPositionError = 0.0;
	mTemperature = 0.0;
	mVoltage = 0.0;
}

ServoStatus::ServoStatus(const protobuf::ServoStatus* status) {
	mID = (uint8_t) status->id();
	mMaxPositionError = status->positionerror();
	mTemperature = status->temperatur();
	mVoltage = status->voltage();
}

ServoStatus::~ServoStatus() {
}

void ServoStatus::setStatus(double maxPositionError, double temperature,
		double voltage) {
	mMaxPositionError = maxPositionError;
	mTemperature = temperature;
	mVoltage = voltage;
}

void ServoStatus::setID(int d) {
	mID = (uint8_t) d;
}

void ServoStatus::writeProtoBuf(protobuf::ServoStatus* status) const {
	status->set_id(mID);
	status->set_positionerror(mMaxPositionError);
	status->set_temperatur(mTemperature);
	status->set_voltage(mVoltage);
}

void Serializer<ServoStatus>::serialize(const ServoStatus& representation, ostream& stream) {
	protobuf::ServoStatus status;

	representation.writeProtoBuf(&status);

	OstreamOutputStream buf(&stream);
	status.SerializeToZeroCopyStream(&buf);
}

void Serializer<ServoStatus>::deserialize(istream& stream, ServoStatus& representation) {
	protobuf::ServoStatus status;

	IstreamInputStream buf(&stream);
	if (!status.ParseFromZeroCopyStream(&buf)) {
		Debugger::ERR("ServoStatus", "Failed to deserialize: %s", status.InitializationErrorString().c_str());
	}

	representation.setID(status.id());
	representation.setStatus(status.positionerror(), status.temperatur(), status.voltage());
}
