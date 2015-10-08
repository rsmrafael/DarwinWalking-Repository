/*
 * PointCloud.h
 *
 *  Created on: 29.08.2012
 *      Author: Stefan Krupop
 */

#ifndef POINTCLOUD_H_
#define POINTCLOUD_H_

#include "../Blackboard/Printable.h"
#include "Point.h"
#include <list>
#include <cstdlib>

using namespace std;

/**
 * Bounding box
 */
struct BoundingBox {
	Point topLeft;			//!< top left corner
	int width;				//!< width
	int height;				//!< height

	/**
	 * @param _topLeft		top left corner
	 * @param _width		width of box
	 * @param _height		height of box
	 */
	BoundingBox(Point _topLeft, int _width, int _height) {
		this->topLeft = _topLeft;
		this->width = _width;
		this->height = _height;
	}
};

/**
 * Point cloud data
 */
struct PointCloudData {
	int minX;		//!< min x
	int maxX;		//!< max x
	int minY;		//!< min y
	int maxY;		//!< max y
};

/**
 * Representation of a cloud of points
 */
class PointCloud: public Printable {
public:
	/**
	 * Constructor
	 */
	PointCloud();

	/**
	 * Copy-Constructor
	 * @param pC	original pointCloud to copy from
	 */
	PointCloud(const PointCloud& pC);

	/**
	 * Destructor
	 */
	virtual ~PointCloud();

	/**
	 * assignment operator, copy pointCloud
	 */
	PointCloud& operator=(const PointCloud& cSource);

	/**
	 * get the cloud
	 * @return list of all points
	 */
	const list<Point>* getCloud() const {
		return mData;
	}

	/**
	 * get the number of points in the cloud
	 * @return size of cloud
	 */
	size_t getSize() const{ return mData->size();}

	/**
	 * clear the cloud
	 */
	void clearCloud();

	/**
	 * add a point to the cloud
	 * @param point 	the point to be added
	 */
	void addPoint(Point point);

	/**
	 * add a point to the cloud
	 * @param x		x coordinate
	 * @param y		y coordinate
	 */
	void addPoint(int x, int y);

	/**
	 * remove the first point
	 */
	void removeFirst();

	/**
	 * remove the last point
	 */
	void removeLast();

	/**
	 * calculate the bounding box of the cloud
	 * @return BoundingBox including all points
	 */
	BoundingBox calculateBoundingBox() const;

	/**
	 * check if boundingBoxes overlap
	 * @param box	the other box to check
	 * @return true if boundingBoxes overlap
	 */
	bool doBoundingBoxesOverlap( const BoundingBox &box) const;

	/**
	 * get the center point of the cloud
	 * @return Point in the center of the cloud
	 */
	Point getCenterPoint() const;

	/**
	 * get the maximum deviation
	 * @return maximum deviation
	 */
	int getMaximumDeviation() const;

	/**
	 * join another cloud to this cloud with checking if points are in
	 * @param pc	pointCloud to join with
	 */
	void join(const PointCloud *pc);

	/**
	 * join another cloud to this cloud without checking if points are already in
	 * @param pc	pointCloud to join with
	 */
	void joinWithoutCheck(const PointCloud *pc);

	/**
	 * check if cloud contains a point
	 * @param p		point to check
	 * @return true if point is part of the cloud
	 */
	bool contains(Point p) const;

	/**
	 * print to a stream
	 * @param stream	output stream
	 */
	virtual void print(ostream& stream) const {
		stream << "Point Cloud: ";
		for (list<Point>::const_iterator pointIterator = mData->begin();
				pointIterator != mData->end(); ++pointIterator) {
			(pointIterator)->print(stream);
			stream << ", ";
		}
	}
private:
	list<Point>* mData;	//!< points of cloud
};

/**
 * PointCloud data object
 */
typedef struct {
	PointCloud *cloud;	//!< the cloud
	int minX;			//!< minimum x
	int maxX;			//!< maximum x
	int minY;			//!< minimum y
	int maxY;			//!< maximum y
} PointCloudObject_t;

/**
 * spezialization of Serializer for PointCloud
 */
template<>
class Serializer<PointCloud> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const PointCloud& representation, ostream& stream);
	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(istream& stream, PointCloud& representation);
};

#endif /* POINTCLOUD_H_ */
