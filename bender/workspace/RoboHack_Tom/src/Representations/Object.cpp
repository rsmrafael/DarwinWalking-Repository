/*
 * Object.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: jan
 */

#include "Object.h"
#include "../Messages/Representations.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"
#include "../SettingData.h"

Object::Object() {
	lastImageX = 0;
	lastImageY = 0;
	lastImageSeen = false;
	lastImageWidth = 0;
	lastImageHeight = 0;
	lastImageTopLeftX = 0;
	lastImageTopLeftY = 0;
	type = UNKNOWN;
	matched = false;
	lastVector = Vector(0.0, 0.0);
	lastSeenCounter = 0;
	originalHeight = 0;
	originalWidth = 0;
}

Object::Object(BoundingBox boundingBox, ObjectType typ) {
	lastImageTopLeftX = boundingBox.topLeft.getX();
	lastImageTopLeftY = boundingBox.topLeft.getY();
	lastImageX = lastImageTopLeftX + boundingBox.width / 2;
	lastImageY = lastImageTopLeftY + boundingBox.height / 2;
	lastImageHeight = boundingBox.height;
	lastImageWidth = boundingBox.width;
	lastImageSeen = true;
	type = typ;
	lastVector = Vector(0.0, 0.0);
	matched = false;
	lastSeenCounter = 0;
	originalHeight = 0;
	originalWidth = 0;
	updateType();
}

Object::Object(const Object* object) {
	lastImageX = object->lastImageX;
	lastImageY = object->lastImageY;
	lastImageSeen = object->lastImageSeen;
	lastImageWidth = object->lastImageWidth;
	lastImageHeight = object->lastImageHeight;
	lastImageTopLeftX = object->lastImageTopLeftX;
	lastImageTopLeftY = object->lastImageTopLeftY;
	type = object->type;
	lastVector = object->lastVector;
	matched = object->matched;
	lastSeenCounter = object->lastSeenCounter;
	originalHeight = object->originalHeight;
	originalWidth = object->originalWidth;
}

Object::Object(int x, int y, int width, int heigth) {
	lastImageTopLeftX = x;
	lastImageTopLeftY = y;
	lastImageX = lastImageTopLeftX + width / 2;
	lastImageY = lastImageTopLeftY + heigth / 2;
	lastImageHeight = heigth;
	lastImageWidth = width;
	lastImageSeen = true;
	type = UNKNOWN;
	lastVector = Vector(0.0, 0.0);
	matched = false;
	lastSeenCounter = 0;
	originalHeight = 0;
	originalWidth = 0;
}

Object::Object(int x, int y, int x1, int y1, ObjectType typ) {
	int width, heigth;

	if (x < x1) {
		lastImageTopLeftX = x;
		width = x1 - x;
	} else {
		lastImageTopLeftX = x1;
		width = x - x1;
	}

	if (y < y1) {
		lastImageTopLeftY = y;
		heigth = y1 - y;
	} else {
		lastImageTopLeftY = y1;
		heigth = y - y1;
	}

	lastImageX = lastImageTopLeftX + width / 2;
	lastImageY = lastImageTopLeftY + heigth / 2;
	lastImageHeight = heigth;
	lastImageWidth = width;
	lastImageSeen = true;
	type = typ;
	lastVector = Vector(0.0, 0.0);
	matched = false;
	lastSeenCounter = 0;
	originalHeight = 0;
	originalWidth = 0;
	updateType();
}

Object::~Object() {
}

void Object::updateType() {
	switch( type) {
	case GOAL_POLE_YELLOW:
		originalHeight = GOAL_CROSSBAR_HEIGHT;
		originalWidth = GOAL_POLE_DIAMETER;
		break;
	case BALL:
		originalHeight = BALL_DIAMETER;
		originalWidth = BALL_DIAMETER;
		break;
	case GOAL_YELLOW_CROSSBAR:
		originalHeight = GOAL_CROSSBAR_DIAMETER;
		originalWidth = GOAL_WIDTH;
		break;
	case ROBOT:
	case ROBOT_CYAN:
	case ROBOT_MAGENTA:
		originalHeight = ROBOT_HEIGHT_MEDIAN;
		originalWidth = ROBOT_WIDTH_MEDIAN;
		break;
	case OBSTACLE:
		originalHeight = OBSTACLE_HEIGHT_MEDIAN;
		originalWidth = OBSTACLE_WIDTH_MEDIAN;
		break;
	case FIELD_LINE:
	case LINE:
	case LINE_POINT:
		originalHeight = LINE_WIDTH;
		originalWidth = LINE_WIDTH;
		break;
	case UNKNOWN:
	default:
		originalHeight = 0;
		originalWidth = 0;
		break;
	}
}

void Object::setOriginalSize(int width, int height) {
	originalWidth = width;
	originalHeight = height;
}

void Object::updateObject(int x, int y, int width, int height) {
	lastImageTopLeftX = x;
	lastImageTopLeftY = y;
	lastImageX = lastImageTopLeftX + width / 2;
	lastImageY = lastImageTopLeftY + height / 2;
	lastImageHeight = height;
	lastImageWidth = width;
	lastImageSeen = true;
}

void Object::notSeen() {
	lastImageSeen = false;
}

bool Object::sortDistance(const Object& obj1, const Object& obj2)
{
  return obj1.lastVector.getLength() < obj2.lastVector.getLength();
}

void Object::mergeBoundingBox(const BoundingBox &box)
{
	int x1, y1, x2, y2;
	if( box.topLeft.getX() < lastImageTopLeftX) {
		x1 = box.topLeft.getX();
	} else {
		x1 = lastImageTopLeftX;
	}

	if( box.topLeft.getY() < lastImageTopLeftY) {
		y1 = box.topLeft.getY();
	} else {
		y1 = lastImageTopLeftY;
	}

	if( (box.topLeft.getX() + box.width) > (lastImageTopLeftX + lastImageWidth)) {
		x2 = (box.topLeft.getX() + box.width);
	} else {
		x2 = (lastImageTopLeftX + lastImageWidth);
	}

	if( (box.topLeft.getY() + box.height) > (lastImageTopLeftY + lastImageHeight)) {
		y2 = (box.topLeft.getY() + box.height);
	} else {
		y2 = (lastImageTopLeftY + lastImageHeight);
	}

	bool lastSeen = lastImageSeen;
	updateObject( x1, y1, (x2 - x1), (y2 - y1));
	lastImageSeen = lastSeen;
}

const BoundingBox Object::getBoundingBox() const {
	return BoundingBox( Point(lastImageTopLeftX,
			lastImageTopLeftY), lastImageWidth, lastImageHeight);
}

const Point Object::getBoundingBoxTopLeft() const {
	return Point(lastImageTopLeftX,
			lastImageTopLeftY);
}

const Point Object::getBoundingBoxBottomRigth() const {
	return Point(lastImageX + lastImageWidth / 2,
			lastImageY - lastImageHeight / 2);
}

bool Object::doBoundingBoxesOverlap(Object object) const {
	bool noOverlap = lastImageTopLeftX
			> object.lastImageTopLeftX + object.lastImageWidth
			|| object.lastImageTopLeftX > lastImageTopLeftX + lastImageWidth
			|| lastImageTopLeftY
					> object.lastImageTopLeftY + object.lastImageHeight
			|| object.lastImageTopLeftY > lastImageTopLeftY + lastImageHeight;

	return !noOverlap;
}

void Object::writeProtoBuf(protobuf::Object* object) const {
	object->set_x(lastImageX);
	object->set_y(lastImageY);

	object->set_topleftx(lastImageTopLeftX);
	object->set_toplefty(lastImageTopLeftY);
	object->set_heigth(lastImageHeight);
	object->set_width(lastImageWidth);

	object->set_seen(lastImageSeen);
	object->set_type((protobuf::ObjectType) type);

	protobuf::Vector2D* vec = object->mutable_vector();

	lastVector.writeProtoBuf(vec);
}

void Serializer<Object>::serialize(const Object& representation,
		ostream& stream) {
	protobuf::Object object;

	representation.writeProtoBuf(&object);

	OstreamOutputStream buf(&stream);
	object.SerializeToZeroCopyStream(&buf);
}

void Serializer<Object>::deserialize(istream& stream, Object& representation) {
	protobuf::Object object;

	IstreamInputStream buf(&stream);
	object.ParseFromZeroCopyStream(&buf);

	int x = object.x();
	int y = object.y();

	representation.updateObject(x, y, 1, 1);
}
