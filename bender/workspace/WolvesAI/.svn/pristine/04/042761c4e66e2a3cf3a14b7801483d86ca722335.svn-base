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

/**
 * DBSCAN
 */
class DBSCAN {
public:

	/**
	 * Constructor
	 * @param minPoints			minimal points
	 * @param epsilonDistance	epsilon distance
	 */
	DBSCAN(size_t minPoints, int epsilonDistance);
	~DBSCAN();

	/**
	 * cluster a dataset
	 * @param dataset			given dataset
	 * @return clustered point clouds
	 */
	list<PointCloud> cluster(list<Point> *dataset);//const PointCloud &dataset

private:
	void getNeighbors(const Point &point, PointCloud *neighbours) const;
	void expandCluster(const Point &point, PointCloud *neighbours);
	bool isMemberOfCluster(Point point);

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

	const size_t MAX_CLUSTER_POINTS;

	list<PointCloud> mClusters;
	PointCloud mCluster;
	map<Point, enum TYPE, mapComp> mPoints;
	size_t mMinPoints;
	int mEpsilonDistance;

};

#endif /* DBSCAN_H_ */
