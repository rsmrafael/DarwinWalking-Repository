/*
 * PointCloudList.h
 *
 *  Created on: 17.12.2013
 *      Author: Oliver
 */

#ifndef POINTCLOUDLIST_H_
#define POINTCLOUDLIST_H_

#include "../Blackboard/Printable.h"
#include "PointCloud.h"
#include <vector>

struct PointCloudData {
	int minX;
	int maxX;
	int minY;
	int maxY;
};

class PointCloudList : public Printable {
public:
	PointCloudList();
	virtual ~PointCloudList();

	void clearList();
	PointCloud *addPointCloud( PointCloud *cloud);
	const vector<PointCloud*>& getPointClouds() const;

	virtual void print(ostream& stream) const;

private:
	vector<PointCloud*> mClouds;
};

#endif /* POINTCLOUDLIST_H_ */
