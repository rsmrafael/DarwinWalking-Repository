/*
 * ConfigDescriptions.cpp
 *
 *  Created on: 01.04.2014
 *      Author: Stefan Krupop
 */

#include "ConfigDescriptions.h"
#include "../Messages/ConfigDescriptions.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"

using namespace std;

ConfigDescriptions::ConfigDescriptions() {
}

ConfigDescriptions::~ConfigDescriptions() {
	mDescriptions.clear();
}

void ConfigDescriptions::addDescription(string Section, string Setting, string Description) {
	string key = Section + "/" + Setting;
	mDescriptions[key] = Description;
}

void ConfigDescriptions::clear() {
	mDescriptions.clear();
}

const map<string, string>* ConfigDescriptions::getDescriptions() const {
	return &mDescriptions;
}

void Serializer<ConfigDescriptions>::serialize(const ConfigDescriptions& representation, ostream& stream) {
	protobuf::ConfigDescriptions ConfigDescriptionsMsg;

	const map<string, string>* lst = representation.getDescriptions();
	for(map<string, string>::const_iterator it = lst->begin(); it != lst->end(); ++it){
		protobuf::ConfigDescriptions_ConfigDescription* ob = ConfigDescriptionsMsg.add_description();
		ob->set_setting((*it).first);
		ob->set_description((*it).second);
	}

	OstreamOutputStream buf(&stream);
	ConfigDescriptionsMsg.SerializeToZeroCopyStream(&buf);
}

void Serializer<ConfigDescriptions>::deserialize(istream& stream, ConfigDescriptions& representation) {
	protobuf::ConfigDescriptions ConfigDescriptionsMsg;

	IstreamInputStream buf(&stream);
	ConfigDescriptionsMsg.ParseFromZeroCopyStream(&buf);

	representation.clear();
}
