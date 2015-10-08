/*
 * ObjectList.h
 *
 *  Created on: Sep 1, 2012
 *      Author: jan
 */

#ifndef ObjectList_H_
#define ObjectList_H_

#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"
#include "../Messages/ObjectList.pb.h"
#include "Object.h"


/**
 * Representation of a list of objects (ball, goal, robot, ...)
 */
class ObjectList: public Printable {
public:

	/**
	 * Constructor
	 */
	ObjectList();

	/**
	 * Destructor
	 */
	virtual ~ObjectList();

	/**
	 * clean up the list
	 */
	void clearList();

	/**
	 * add an object to the list
	 * @param ob	object to be added
	 */
	void addObject(Object ob);

	/**
	 * add several objects to the list
	 * @param obs	objectList
	 */
	void addObjects(const ObjectList& obs);

	/**
	 * return all objects in a list
	 * @return vector of all objects
	 */
	const vector<Object>& getObjects() const;

	/**
	 * get the largest object of the list
	 * @return largest object
	 */
	const Object getLargestObject() const;

	/**
	 * get the nearest object of the list
	 * @return nearest object
	 */
	//const Object getNearestObject() const;

	/**
	 * get all objects of the list sorted by distance
	 * @return vector of objects sorted by distance
	 */
	//const vector<Object> getDistanceSortedObjects() const;

	/**
	 * print to an output stream
	 * @param stream 	output stream
	 */
	virtual void print(ostream& stream) const {

	}

private:
	vector<Object> mData;	//!< data

};

/**
 * spezialization of Serializer for ObjectList
 */
template<>
class Serializer<ObjectList> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const ObjectList& representation, ostream& stream);
	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(istream& stream, ObjectList& representation);
};

#endif /* ObjectList_H_ */
