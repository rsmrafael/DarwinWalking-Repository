/*
 * DBSCAN.h
 *
 * DBSCAN: Density-Based Spatial Clustering of Applications with Noise
 * Data mining algorithm for cluster analysis
 *
 *  Created on: April 26, 2013
 *      Author: Jan Carstensen
 */

#ifndef DBSCAN_H_
#define DBSCAN_H_

#include <list>
#include <cstdlib>
#include "../../../Representations/Point.h"
#include "../../../Representations/PointCloud.h"

using namespace std;

class DBSCAN {
public:

	DBSCAN(size_t minPoints, int epsilonDistance);
	~DBSCAN();

	list<PointCloud> cluster(list<Point> *dataset);//const PointCloud &dataset

private:
	const size_t MAX_CLUSTER_POINTS;

	enum TYPE{
			NOTVISITED,
			VISITED,
			NOISE
		};

	struct mapComp{
			bool operator() (const Point& p1, const Point& p2) const{
				if(p1.getY() < p2.getY()){
					return true;
				} else if(p1.getY() == p2.getY()){
					if(p1.getX() < p2.getX()){
						return true;
					} else {
						return false;
					}
				} else {
					return false;
				}
			}
	};

	map<Point, enum TYPE, mapComp> mPoints;

	size_t mMinPoints;
	int mEpsilonDistance;
	list<PointCloud> mClusters;
	PointCloud mCluster;

	void getNeighbors(const Point &point, PointCloud *neighbours) const;
	void expandCluster(const Point &point, PointCloud *neighbours);
	bool isMemberOfCluster(Point point);

};

#endif /* DBSCAN_H_ */
