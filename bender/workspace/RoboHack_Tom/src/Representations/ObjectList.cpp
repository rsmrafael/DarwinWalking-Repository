/*
 * ObjectList.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: jan
 */

#include "ObjectList.h"
#include "../Messages/Representations.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"
#include "../Messages/ObjectList.pb.h"
#include <algorithm>

ObjectList::ObjectList() {

}

ObjectList::~ObjectList() {
}

void ObjectList::clearList() {
	mData.clear();
}

void ObjectList::addObject(Object ob) {
	mData.push_back(ob);
}

const vector<Object>& ObjectList::getObjects() const {
	return mData;
}

void ObjectList::addObjects(const ObjectList& obs) {
	const vector<Object>& data = obs.getObjects();

	vector<Object>::const_iterator it;
	for (it = data.begin(); it != data.end(); ++it) {
		addObject(*it);
	}

}

const Object ObjectList::getLargestObject() const {
	if (!mData.empty()) {
		int maxArea = 0;
		Object obj;
		for (vector<Object>::const_iterator it = mData.begin();
				it != mData.end(); ++it) {
			if (it->lastImageSeen) {
				int area = it->lastImageWidth * it->lastImageHeight;
				if (area > maxArea) {
					maxArea = area;
					obj = Object(*it);
				}
			}
		}
		return obj;
	} else {
		Object ob = Object();
		ob.notSeen();
		return ob;
	}
}

const Object ObjectList::getNearestObject() const {
	if (!mData.empty()) {
		double minDist = 1e10;
		Object obj;
		for (vector<Object>::const_iterator it = mData.begin();
				it != mData.end(); ++it) {
			if (it->lastImageSeen) {
				double dist = it->lastVector.getLength();
				if (dist < minDist) {
					minDist = dist;
					obj = Object(*it);
				}
			}
		}
		return obj;
	} else {
		Object ob = Object();
		ob.notSeen();
		return ob;
	}
}

const vector<Object> ObjectList::getDistanceSortedObjects() const {
	vector<Object> objects = mData;
	sort(objects.begin(), objects.end(), Object::sortDistance);
	return objects;
}

void Serializer<ObjectList>::serialize(const ObjectList& representation,
		ostream& stream) {
	protobuf::ObjectList ObjectListMsg;

	const vector<Object>& lst = representation.getObjects();
	for (vector<Object>::const_iterator it = lst.begin(); it != lst.end(); ++it) {
		protobuf::Object* ob = ObjectListMsg.add_object();
		(*it).writeProtoBuf(ob);
	}

	OstreamOutputStream buf(&stream);
	ObjectListMsg.SerializeToZeroCopyStream(&buf);
}

void Serializer<ObjectList>::deserialize(istream& stream,
		ObjectList& representation) {
	protobuf::ObjectList ObjectListMsg;

	IstreamInputStream buf(&stream);
	ObjectListMsg.ParseFromZeroCopyStream(&buf);

	representation.clearList();
	for (int i = 0; i < ObjectListMsg.object_size(); ++i) {
		const protobuf::Object& obj = ObjectListMsg.object(i);
		Object ob;
		ob.lastImageX = obj.x();
		ob.lastImageY = obj.y();
		representation.addObject(ob);
	}
}
