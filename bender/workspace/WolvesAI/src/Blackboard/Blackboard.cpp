/*
 * Blackboard.cpp
 *
 *  Created on: 07.08.2012
 *      Author: Stefan Krupop
 */

#include "Blackboard.h"
#include "../Messages/Blackboard.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include <sstream>
#include "../Debugging/Debugger.h"

using namespace std;

Blackboard::~Blackboard() {
	// delete the registry
	for (Registry::iterator iter = mRegistry.begin(); iter != mRegistry.end(); ++iter) {
		delete iter->second;
	}
}

void Blackboard::shutdown() {
	// delete the registry
	for (Registry::iterator iter = mRegistry.begin(); iter != mRegistry.end(); ++iter) {
		delete iter->second;
	}
	mRegistry.clear();
}

void Blackboard::serialize(ostream& stream, bool TOConly) {
	protobuf::Blackboard msg;

	for (Registry::iterator iter = mRegistry.begin(); iter != mRegistry.end(); ++iter) {
		//Debugger::DEBUG("Blackboard", "Serializing %s, type %s", iter->second->getName().c_str(), iter->second->getTypeName().c_str());
		protobuf::Blackboard_Representation* rep = msg.add_representations();
		rep->set_name(iter->second->getName());
		rep->set_type(iter->second->getTypeName());
		if (!TOConly) {
			ostringstream data;
			iter->second->getRepresentation().serialize(data);
			rep->set_data(data.str());
		}
	}

	OstreamOutputStream buf(&stream);
	msg.SerializeToZeroCopyStream(&buf);
}

void Blackboard::serialize(ostream& stream, bool TOConly, vector<string>& items) {
	protobuf::Blackboard msg;

	for (vector<string>::iterator iter = items.begin(); iter != items.end(); ++iter) {
		Registry::iterator it2 = mRegistry.find(*iter);
		if (it2 != mRegistry.end()) {
			//Debugger::DEBUG("Blackboard", "Serializing %s, type %s", it2->second->getName().c_str(), it2->second->getTypeName().c_str());
			protobuf::Blackboard_Representation* rep = msg.add_representations();
			rep->set_name(it2->second->getName());
			rep->set_type(it2->second->getTypeName());
			if (!TOConly) {
				ostringstream data;
				it2->second->getRepresentation().serialize(data);
				rep->set_data(data.str());
			}
		}
	}

	OstreamOutputStream buf(&stream);
	msg.SerializeToZeroCopyStream(&buf);
}

void Blackboard::deserialize(string name, istream& stream) {
	for (Registry::iterator iter = mRegistry.begin(); iter != mRegistry.end(); ++iter) {
		if (iter->second->getName() == name) {
			iter->second->getRepresentation().deserialize(stream);
			break;
		}
	}
}
