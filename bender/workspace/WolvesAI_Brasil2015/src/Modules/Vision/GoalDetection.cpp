/*
 * GoalDetection.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: Jan Carstensen
 *      Edited: Oliver Krebs
 */

#include "GoalDetection.h"
#include "../../Debugging/Debugger.h"
#include "../../Debugging/DebugDrawer.h"
#include "../../Config.h"
#include "../../Utils/Constants.h"
#include <cmath>
#include <algorithm>

bool poleCompare(const pair<int, Object>& firstElem, const std::pair<int, Object>& secondElem) {
	return firstElem.first < secondElem.first;
}

GoalDetection::GoalDetection()
:	mMinPointsAreaRatio(0.02),
	mMinHeightRatio(0.2),
	mMaxHeightRatio(3.0),
	mMinCloudPoints(10),
	mMinCloudDistance(30),
	mMaxErrorY(5),
	mMinPoleWidth(5),
	mMinPoleHeight(10),
	mIsFilterFieldLine(true),
	mDebugDrawings(0),
	mMinY(80),
	mMaxU(100),
	mMinV(100),
	mMaxV(140)
{
	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

GoalDetection::~GoalDetection() {
	Config::getInstance()->removeConfigChangeHandler(this);
}

bool GoalDetection::execute() {

	getGoalPoles().clearList();

	vector<PointCloud*> cloudList;
	PointCloud *cloud = new PointCloud();

	int goalLastX = 0;
	int goalCloudMinY = 0;
	int goalCloudMaxY = 0;

	// go through all points and separate them to clouds (possible poles)
	list<Point>::const_iterator it = getGoalPoints().getCloud()->begin();
	for( ; it != getGoalPoints().getCloud()->end(); ++it) {
		int x = (*it).getX();
		int y = (*it).getY();
		if( x != goalLastX) {
			if( x > (goalLastX + 2*mMinCloudDistance)) {
				if( cloud->getSize() > 0) {
					cloudList.push_back(cloud);
					//lint -e{423}
					cloud = new PointCloud();
				}
				goalCloudMinY = y;
				goalCloudMaxY = y;
				//Debugger::DEBUG("Goal", "Last: %d, actual: %d (%d)", goalLastX, x, y);
			} else {
				int divisor = goalCloudMaxY - goalCloudMinY;
				if( divisor != 0) {
					double ratio = (double)(y - goalCloudMinY) / (double)divisor;
					//Debugger::DEBUG("Goal", "Ratio: %f (%d|%d) [%d]", ratio, x, y, (goalCloudMaxY - goalCloudMinY));
					if( ratio < mMinHeightRatio || ratio >= mMaxHeightRatio) {
						if( cloud->getSize() > 0) {
							cloudList.push_back(cloud);
							//lint -e{423}
							cloud = new PointCloud();
						}
						goalCloudMinY = y;
						goalCloudMaxY = y;
						//Debugger::DEBUG("Goal", "MinMax: %d", y);
					}
				}
			}
			goalLastX = x;
		}
		if( y > goalCloudMaxY) {
			goalCloudMaxY = y;
			//Debugger::DEBUG("Goal", "Max: %d", y);
		} else if( y < goalCloudMinY) {
			goalCloudMinY = y;
			//Debugger::DEBUG("Goal", "Min: %d", y);
		}

		cloud->addPoint( (*it));
	}

	if( cloud->getSize() > 0) {
		cloudList.push_back(cloud);
	} else {
		delete cloud;
		cloud = NULL;
	}

	double panAngle = DEGREE_TO_RADIAN * getBodyStatus().getPan();
	double tiltAngle = DEGREE_TO_RADIAN * getBodyStatus().getTilt();

	// go through all clouds and filter out wrong poles
	vector< pair<int, Object> > objectMap;
	vector<PointCloud*>::iterator cloudIt;
	for( cloudIt = cloudList.begin(); cloudIt != cloudList.end(); ) {
		//lint -e{423}
		cloud = (*cloudIt);
		//Debugger::INFO("GoalDetection", "Goal point cloud: %d", (*it)->getSize());
		if (cloud->getSize() >= mMinCloudPoints) {
			BoundingBox box = (*cloudIt)->calculateBoundingBox();

			//Debugger::INFO("GoalDetection", "Box: %d, %d (%d x %d)", box.topLeft.getX(), box.topLeft.getY(), box.width, box.heigth);

			if (box.width >= mMinPoleWidth && box.height >= mMinPoleHeight && filterFieldLines(box)) {
				int area = (box.width * box.height);
				if( area != 0) {
					double ratio = (double)cloud->getSize() / (double)area;
					if( ratio >= mMinPointsAreaRatio) {
						Object obj(box.topLeft.getX(), box.topLeft.getY(),
								box.width, box.height);
						obj.type = Object::GOAL_POLE_YELLOW;

						objectMap.push_back(make_pair(obj.lastImageTopLeftX, obj));
#ifdef _DEBUG
						if( mDebugDrawings >= 2) {
							DRAW_BOX("Goal", box.topLeft.getX(), box.topLeft.getY(),
									box.width, box.height, DebugDrawer::GREEN);
						}
#endif
					} else {
#ifdef _DEBUG
						if( mDebugDrawings >= 3) {
							DRAW_BOX("Goal", box.topLeft.getX(), box.topLeft.getY(),
									box.width, box.height, DebugDrawer::TEAL);
						}
#endif
					}
				}
#ifdef _DEBUG
				else {
					Debugger::WARN("GoalDetection", "Area is 0, this should be impossible!");
				}
#endif
			}
#ifdef _DEBUG
			else {
				if( mDebugDrawings >= 4) {
					DRAW_BOX("Goal", box.topLeft.getX(), box.topLeft.getY(),
						box.width, box.height, DebugDrawer::LIGHT_GRAY);
				}
			}
#endif

		}
#ifdef _DEBUG
		else if( cloud->getSize() > 0) {
			if( mDebugDrawings >= 5) {
				BoundingBox box = (*cloudIt)->calculateBoundingBox();
				DRAW_BOX("Goal", box.topLeft.getX(), box.topLeft.getY(),
					box.width, box.height, DebugDrawer::RED);
			}
		}
#endif
		delete cloud;
		cloud = NULL;
		cloudIt = cloudList.erase(cloudIt);
	}

	if( cloud != NULL) {
		delete cloud;
	}

	// sort objects downward
	//lint --e{864}
	sort( objectMap.begin(), objectMap.end(), poleCompare);

	// use only the outer objects (left and right)
	vector< pair<int,Object> >::iterator objIt = objectMap.begin();
	if( objIt != objectMap.end()) {
		Object obj = objIt->second;
		getGoalPoles().addObject(obj);

		objIt = objectMap.end();
		if( --objIt != objectMap.begin()) {
			obj = objIt->second;
			getGoalPoles().addObject(obj);
		}
	}

	return true;
}

bool GoalDetection::findCrossbar( int startX, int startY, int dirX) const {
	int minFound = 5;
	int notFound = 0;
	int found = 0;
	int endX = startX + 20 * dirX;
	for( int x = startX; x != endX; x += dirX) {
		YUVImage::YUVData data = getImage().getValue(x, startY);
		if( data.Y >= mMinY && data.U < mMaxU
				&& data.V >= mMinV && data.V < mMaxV) {
			found++;
			if( found > minFound) {
				return true;
			}
		} else {
			notFound++;
			if( notFound > minFound) {
				return false;
			}
		}
	}
	return false;
}

bool GoalDetection::filterFieldLines(const BoundingBox &box) const {
	if( mIsFilterFieldLine) {
		// TopLeft is not in field or is at top border of image
		bool topValid = !getFieldLines().isPointInField(box.topLeft, 0) || (box.topLeft.getY() < mMaxErrorY);
		if( !topValid) {
			return false;
		}

		Point bottomLeft, bottomRight;
		bottomLeft.updatePoint( box.topLeft.getX(), box.topLeft.getY() + box.height);
		bottomRight.updatePoint( box.topLeft.getX() + box.width, box.topLeft.getY() + box.height);

		// BottomLeft or BottomRight is in field or at bottom border of image
		bool bottomValid = (getFieldLines().isPointInField(bottomLeft, mMaxErrorY)
						|| getFieldLines().isPointInField(bottomRight, mMaxErrorY)
						|| bottomLeft.getY() > (getImage().getHeight() - mMaxErrorY));
		if( !bottomValid) {
			return false;
		}
	}
	return true;
}

void GoalDetection::configChanged() {
	Debugger::INFO("GoalDetection", "config changed");

	ConfigFile *config = Config::getInstance();
	mMinPointsAreaRatio 	= config->get<double>("Vision", "Goal_minPointsAreaRatio", 0.02);
	mMinHeightRatio 		= config->get<double>("Vision", "Goal_minHeightRatio", 0.2);//0.2
	mMaxHeightRatio 		= config->get<double>("Vision", "Goal_maxHeightRatio", 1.2);//3.0
	mMaxErrorY 				= config->get<int>("Vision", "Goal_maxErrorY", 12);//6
	mMinPoleWidth			= config->get<int>("Vision", "Goal_minPoleWidth", 5);
	mMinPoleHeight			= config->get<int>("Vision", "Goal_minPoleHeight", 10);
	mMinCloudPoints 		= config->get<size_t>("Vision", "Goal_minCloudPoints", (size_t)20);
	mDebugDrawings 			= config->get<int>("Vision", "Goal_debugDrawings", 0);
	mIsFilterFieldLine 		= config->get<bool>("Vision", "Goal_filterFieldLine", true);

	// not used
	//mMinY = config->get<int>("Vision", "Goal_minY",  80);//140
	//mMaxU = config->get<int>("Vision", "Goal_maxU", 120);//100
	//mMinV = config->get<int>("Vision", "Goal_minV", 110);//100//90
	//mMaxV = config->get<int>("Vision", "Goal_maxV", 160);//140

	mMinCloudDistance = config->get<int>("Vision", "minCloudDistance", 15);
}
