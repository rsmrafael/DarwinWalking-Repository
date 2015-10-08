/*
 * RobotDetection.cpp
 *
 *  Created on: April 3, 2013
 *      Author: Jan
 */

#include "RobotDetection.h"
#include "../../Debugging/Debugger.h"
#include "../../Debugging/DebugDrawer.h"
#include "../../Config.h"

RobotDetection::RobotDetection()
:	mDebugDrawings( 0),
 	mMinPointsAreaRatio( 0.3),
 	mMinColorBlackRatio( 0.2),
 	mMinCloudPoints( 12),
 	mMinColorCloudPoints( 6),
 	mMinCloudDistance( 15)
{
	this->configChanged();
	Config::getInstance()->registerConfigChangeHandler(this);
}

RobotDetection::~RobotDetection()
{
	Config::getInstance()->removeConfigChangeHandler(this);
}

bool RobotDetection::execute()
{
	getCyanRobots().clearList();
	getMagentaRobots().clearList();
	getRobots().clearList();
/*
	vector<PointCloudObject_t> cyanClouds;
	vector<PointCloudObject_t> magentaClouds;
	vector<PointCloudObject_t> obstacleClouds;    //useless?
	vector<PointCloudObject_t> greyClouds;

	// separate points to clouds
	list<Point>::const_iterator pointIt = getCyanPoints().getCloud()->begin();
	for( ; pointIt != getCyanPoints().getCloud()->end(); ++pointIt) {
		addPointToNearestCloud( (*pointIt), cyanClouds);
	}
	eraseSmallClouds(cyanClouds);

	pointIt = getMagentaPoints().getCloud()->begin();
	for( ; pointIt != getMagentaPoints().getCloud()->end(); ++pointIt) {
		addPointToNearestCloud( (*pointIt), magentaClouds);
	}
	eraseSmallClouds(magentaClouds);

	//TODO implement in Scanlines
//	pointIt = getGreyPoints().getCloud()->begin();
//	for( ; pointIt != getGreyPoints().getCloud()->end(); ++pointIt) {
//		addPointToNearestCloud( (*pointIt), greyClouds);
//	}
//	eraseSmallClouds(greyClouds);

	vector<PointCloudObject_t>::iterator cloudIt;

	int idx = 0;
	for( cloudIt = cyanClouds.begin(); cloudIt != cyanClouds.end(); ++cloudIt) {
		idx++;
		//TODO resolve inconsistency!
		PointCloudObject_t nearestCyanCloud = getNearestCloudToPoint(cloudIt->cloud->getCenterPoint(), cyanClouds);
		int xCenter = (nearestCyanCloud.maxX + nearestCyanCloud.minX) / 2;
		int yCenter = (nearestCyanCloud.maxY + nearestCyanCloud.minY) / 2;
		PointCloud *nearest = nearestCyanCloud.cloud;
		DRAW_TEXT("cyanCloudCenterText", 0, 7 * idx, DebugDrawer::LIGHT_GRAY,
				"NearestCloudCenter: %d/%d -> %d/%d",
				xCenter,
				yCenter,
				nearest->getCenterPoint().getX(),
				nearest->getCenterPoint().getY());
		DRAW_TEXT("cyanCloudCenterPoint", xCenter, yCenter,
				DebugDrawer::WHITE, ". %d/%d",
				xCenter, yCenter);*

		if (cyanClouds.size() < 2) {
			//TODO Stuff for only one cloud here!
		} else {
			//Berechnung für kürzesten Abstand zwischen 2 Boxen
			int c1x, c2x, c1y, c2y;
			if (cloudIt->maxX < nearestCyanCloud.minX) {
				c1x = cloudIt->maxX;
				c2x = nearestCyanCloud.minX;
			} else if (cloudIt->minX > nearestCyanCloud.maxX) {
				c1x = cloudIt->minX;
				c2x = nearestCyanCloud.maxX;
			} else {
				if (cloudIt->minX < nearestCyanCloud.minX) {
					c1x = nearestCyanCloud.minX;
					c2x = nearestCyanCloud.minX;
				} else {
					c1x = cloudIt->minX;
					c2x = cloudIt->minX;
				}
			}
			if (cloudIt->maxY < nearestCyanCloud.minY) {
				c1y = cloudIt->maxY;
				c2y = nearestCyanCloud.minY;
			} else if (cloudIt->minY > nearestCyanCloud.maxY) {
				c1y = cloudIt->minY;
				c2y = nearestCyanCloud.maxY;
			} else {
				if (cloudIt->minY < nearestCyanCloud.minY) {
					c1y = nearestCyanCloud.minY;
					c2y = nearestCyanCloud.minY;
				} else {
					c1y = cloudIt->minY;
					c2y = cloudIt->minY;
				}
			}

			Point p(c1x, c1y);
			Point pNear(c2x, c2y);
			Point middle(((c1x + c2x) / 2), ((c1y + c2y) / 2));

			double maxDistance = 15.0;
			double dist = p.calcluateDistanceTo(pNear);

			DRAW_TEXT("Point_Between_Clouds", middle.getX(), middle.getY(), DebugDrawer::WHITE,
					". Middle: %d/%d Dist: %f", middle.getX(), middle.getY(), dist);
			BoundingBox box1 = cloudIt->cloud->calculateBoundingBox();
			BoundingBox box2 = nearestCyanCloud.cloud->calculateBoundingBox();
			DRAW_BOX("Robot", box1.topLeft.getX(), box1.topLeft.getY(),
					box1.width, box1.height, DebugDrawer::CYAN);
			DRAW_BOX("Robot", box2.topLeft.getX()+1, box2.topLeft.getY()+1,
					box2.width-2, box2.height-2, DebugDrawer::RED);
			if (dist < maxDistance) {
				DRAW_TEXT("Middle", middle.getX(), middle.getY() + 7, DebugDrawer::YELLOW,
						"JOIN");
//				check for grey in middle if distance > maxDis?
			}
		}
		BoundingBox box = cloudIt->cloud->calculateBoundingBox();
		if (mDebugDrawings == 1) {
//			DRAW_BOX("Robot", box.topLeft.getX(), box.topLeft.getY(),
//					box.width, box.height, DebugDrawer::CYAN);
//			DRAW_TEXT("BoxCoords", box.topLeft.getX(), box.topLeft.getY(), DebugDrawer::BLACK,
//					"Center: %d/%d DisToNearest: %f",
//					cloudIt->cloud.getCenterPoint().getX(),
//					cloudIt->cloud.getCenterPoint().getY(),
//					cloudIt->cloud.getCenterPoint().calcluateDistanceTo(nearestCyanCloud.cloud.getCenterPoint()));
		}

		Object obj(box, Object::ROBOT_CYAN);
		getCyanRobots().addObject(obj);
	}

	// clean up
	for( cloudIt = cyanClouds.begin(); cloudIt != cyanClouds.end(); ++cloudIt) {
		delete cloudIt->cloud;
	}
	for( cloudIt = magentaClouds.begin(); cloudIt != magentaClouds.end(); ++cloudIt) {
		delete cloudIt->cloud;
	}
	*/

	return true;
}

void RobotDetection::addPointToNearestCloud( const Point &point, vector<PointCloudObject_t> &cloudList) const
{
	int x = point.getX();
	int y = point.getY();
	vector<PointCloudObject_t>::iterator it;
	for( it = cloudList.begin(); it != cloudList.end(); ++it) {
		if( x > (*it).minX - mMinCloudDistance
			&& x < (*it).maxX + mMinCloudDistance
			&& y > (*it).minY - mMinCloudDistance
			&& y < (*it).maxY + mMinCloudDistance) {
				(*it).cloud->addPoint( point);
				if( x < (*it).minX) {
					(*it).minX = x;
				} else if( x > (*it).maxX) {
					(*it).maxX = x;
				}
				if( y < (*it).minY) {
					(*it).minY = y;
				} else if( y > (*it).maxY) {
					(*it).maxY = y;
				}
				return;
		}
	}

	PointCloudObject_t cloudObj;
	cloudObj.cloud = new PointCloud();
	cloudObj.cloud->addPoint(point);
	cloudObj.minX = x;
	cloudObj.maxX = x;
	cloudObj.minY = y;
	cloudObj.maxY = y;
	cloudList.push_back(cloudObj);
}

void RobotDetection::eraseSmallClouds(vector<PointCloudObject_t> &cloudList) const
{
	vector<PointCloudObject_t>::iterator cloudIt;
	for( cloudIt = cloudList.begin(); cloudIt != cloudList.end();) {
		if( cloudIt->cloud->getSize() < mMinColorCloudPoints) {
			delete cloudIt->cloud;
			cloudIt = cloudList.erase(cloudIt);
		} else {
			++cloudIt;
		}
	}
}

PointCloudObject_t RobotDetection::getNearestCloudToPoint(const Point &point, vector<PointCloudObject_t> &cloudList) const
{
	double shortestDistance = 1e10;
	PointCloudObject_t nearestCloud = (*cloudList.begin());
	vector<PointCloudObject_t>::iterator cloudIt;
	for( cloudIt = cloudList.begin(); cloudIt != cloudList.end(); ++cloudIt) {
		Point center = cloudIt->cloud->getCenterPoint();
		double dist = point.calculateDistanceTo(center);
		if (dist > 0.0 && dist <= shortestDistance) {
			shortestDistance = dist;
			nearestCloud = (*cloudIt);
		}
	}
	return nearestCloud;
}

void RobotDetection::configChanged()
{
	Debugger::INFO("RobotDetection", "config changed");

	ConfigFile *config = Config::getInstance();
	mDebugDrawings = config->get<int>("Vision", "Robot_debugDrawings", 0);
	mMinPointsAreaRatio = config->get<double>("Vision", "Robot_minPointsAreaRatio", 0.02);
	mMinColorBlackRatio = config->get<double>("Vision", "Robot_minColorBlackRatio", 0.2);
	mMinCloudPoints 	 = config->get<size_t>("Vision", "Robot_minCloudPoints", (size_t)12);
	mMinColorCloudPoints = config->get<size_t>("Vision", "Robot_minColorCloudPoints", (size_t)6);
	mMinCloudDistance = config->get<int>("Vision", "minCloudDistance", 10);//15
}
