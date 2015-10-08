/*
 * Object.h
 *
 *  Created on: Sep 1, 2012
 *      Author: jan
 */

#ifndef Object_H_
#define Object_H_

#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"
#include "../Messages/Object.pb.h"
#include "PointCloud.h"
#include "Vector.h"
#include "Point.h"

/**
 * A representation for all real objects in the given environment
 * e.g. ball, goal pole, robot, ...
 */
class Object: public Printable {
public:

	/**
	 * Object type
	 */
	enum ObjectType {
		UNKNOWN = 0,
		GOAL_POLE_YELLOW = 1,
		BALL = 2,
		GOAL_YELLOW_CROSSBAR = 3,
		ROBOT = 4,
		ROBOT_CYAN = 5,
		ROBOT_MAGENTA = 6,
		OBSTACLE = 7,
		FIELD_LINE = 8,
		LINE = 9,
		LINE_POINT = 10//,
		//GOAL = 11
	};

	int lastImageTopLeftX;	//!< top left x coordinate
	int lastImageTopLeftY;	//!< top left y coordinate
	int lastImageX;			//!< center x coordinate
	int lastImageY;			//!< center y coordinate
	int lastImageWidth;		//!< width in pixel
	int lastImageHeight;	//!< height in pixel
	bool lastImageSeen;		//!< object was seen last frame
	bool matched;			//!< matched
	ObjectType type;		//!< object type
	int lastSeenCounter;	//!< counter
	int originalHeight;		//!< original height in mm
	int originalWidth;		//!< original width in mm
	//Vector lastVector;		//!< vector to object

	/**
	 * Default Constructor for Object
	 */
	Object();

	/**
	 * Constructor for Object
	 * @param x 		left border of object
	 * @param y			top border of object
	 * @param width		width of object
	 * @param height	height of object
	 */
	Object(int x, int y, int width, int height);

	/**
	 * Constructor for Object
	 * @param x 		left border of object
	 * @param y			top border of object
	 * @param x2		right border of object
	 * @param y2 		bottom border of object
	 * @param objType	type of object
	 */
	Object(int x, int y, int x2, int y2, ObjectType objType);

	/**
	 * Constructor for Object
	 * @param boundingBox	the bounding box for the object
	 * @param objType		the object type
	 */
	Object(BoundingBox boundingBox, ObjectType objType);

	/**
	 * Constructor, creates a copy of an existing object
	 * @param object 	the original object
	 */
	Object(const Object* object);

	/**
	 * Destructor
	 */
	virtual ~Object();

	virtual void print(ostream& stream) const {

	}

	/**
	 * get string
	 * @return string
	 */
	std::string toString() const;

	/**
	 * copies object to another object reference
	 * @param obj		the object reference for the destination
	 */
	void copyTo(Object &obj) const;

	/**
	 * update the position and size (object was seen)
	 * @param x			left border of new position
	 * @param y			top border of new position
	 * @param width		new width of object
	 * @param height	new height of object
	 */
	void updateObject(int x, int y, int width, int height);

	/**
	 * object was not seen
	 */
	void notSeen();

	/**
	 * update the type of the object
	 */
	void updateType();

	/**
	 * set the original object size
	 * @param width		width of original object in mm
	 * @param height	height of original object in mm
	 */
	void setOriginalSize(int width, int height);

	/**
	 * merge a bounding box to this object, update position and size respectively
	 * @param box		BoundingBox to be merged
	 */
	void mergeBoundingBox(const BoundingBox &box);

	/**
	 * get the bounding box of this object
	 * @return bounding box of this object
	 */
	const BoundingBox getBoundingBox() const;

	/**
	 * check if bounding boxes of objects overlap
	 * @param object	object to check overlap with
	 * @return true, if bounding boxes overlap
	 */
	bool doBoundingBoxesOverlap(Object object) const;

	/**
	 * write data to protobuf representation
	 * @param object	protobuf representation of object
	 */
	void writeProtoBuf(protobuf::Object* object) const;

	/**
	 * sort objects depending on its distance
	 * @param obj1		first object
	 * @param obj2		second object
	 * @return	true, if first object is nearer than second object
	 */
	//static bool sortDistance(const Object& obj1, const Object& obj2);
};

/**
 * spezialization of Serializer for Object
 */
template<>
class Serializer<Object> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const Object& representation, ostream& stream);
	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(istream& stream, Object& representation);
};

#endif /* Object_H_ */
