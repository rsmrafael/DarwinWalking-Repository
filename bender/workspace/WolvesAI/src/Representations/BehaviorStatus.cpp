/*
 * BehaviorStatus.cpp
 *
 *  Created on: 28.04.2013
 *      Author: 7
 */

#include "BehaviorStatus.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"
#include "../Messages/BehaviorStatus.pb.h"
#include "../Communication/MitecomData.h"


BehaviorStatus::BehaviorStatus()
:	actualRole(ROLE_IDLING),
	actualState("")
{
}

BehaviorStatus::~BehaviorStatus()
{

}

void BehaviorStatus::setActualRole(int32_t role)
{
	actualRole = role;
}

void BehaviorStatus::setActualStatus(string state)
{
	actualState = state;
}

int32_t BehaviorStatus::getActualRole() const
{
	return actualRole;
}

string BehaviorStatus::getActualStatus() const
{
	return actualState;
}

void Serializer<BehaviorStatus>::serialize(const BehaviorStatus& representation, ostream& stream) {
	protobuf::BehaviorStatus status;
	status.set_status(representation.getActualStatus());
	OstreamOutputStream buf(&stream);
	status.SerializeToZeroCopyStream(&buf);
}

void Serializer<BehaviorStatus>::deserialize(istream& stream, BehaviorStatus& representation) {
	protobuf::BehaviorStatus status;

	IstreamInputStream buf(&stream);
	status.ParseFromZeroCopyStream(&buf);

	representation.setActualStatus(status.status());
}
