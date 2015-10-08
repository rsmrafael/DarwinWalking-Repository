/*
 * Scanlines.cpp
 *
 *  Created on: 29.08.2012
 *      Author: Stefan Krupop
 *      Edited: Oliver Krebs
 */

#ifndef SCANLINES_MULTITHREAD
#include "Scanlines.h"
#include "../../Config.h"
#include "../../Representations/Object.h"

Scanlines::Scanlines()
:	mScanlineDistanceColumn( 2),
	mScanlineDistanceRow( 2),
	mMaxGreenNotSeen( 20),
	mMinCloudDistance( 30),
	mMinCloudDistanceRow( 15),
	mMinCloudDistanceColumn( 15),
	mFieldLineScanDivisor( 4),
	mNumGreenSearchPoints( 8),
	mMinGreenPoints( 5),
	mIgnoreObjectsOutsideField( true),
	mMaxY_Green( 180),
	mMinU_Green( 100),
	mMaxU_Green( 180),
	mMaxV_Green( 85),
	mMaxSum_Green( 400),
	mMinY_Ball( 50),
	mMinV_Ball( 140),
	mMaxU_Ball( 140),
	mMinY_Goal( 80),
	mMaxU_Goal( 120),
	mMinV_Goal( 110),
	mMaxV_Goal( 160),
	mMaxVaddU_Goal( 240),
	mMinY_White( 150),
	mMinSum_White( 350),
	mMaxY_Black( 80),
	mMaxVariance_Gray( 60),
	mMinU_Cyan( 160),
	mMaxV_Cyan( 80),
	mMinU_Magenta( 140),
	mMinV_Magenta( 140),
	mDebugDrawings( 0)
{
	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

Scanlines::~Scanlines() {
	Config::getInstance()->removeConfigChangeHandler(this);
}

bool Scanlines::execute() {

	const YUVImage* yuvImage = &getImage();
	int width = yuvImage->getWidth();
	int height = yuvImage->getHeight();

	getBallPoints().clearCloud();
	getGoalPoints().clearCloud();
	getObstaclePoints().clearCloud();
	getCyanPoints().clearCloud();
	getMagentaPoints().clearCloud();
	getWhitePoints().clearCloud();
	getGreenPoints().clearCloud();

	/**
	 * Create Scan Lines from the Bottom and Rise them. Mark the last spot where a green Pixel is found.
	 */

	int dx = mScanlineDistanceColumn;
	int dy = mScanlineDistanceRow;

#ifndef JENKINS
	int tilt = getBodyStatus().getTilt();
	if( tilt < 50) {
		dx *= 4;//8
		dy *= 4;//8
	} else if( tilt < 80) {
		dx *= 2;//4
		dy *= 2;//4
	}
#endif

	int fieldLineScanDivisor = mFieldLineScanDivisor * dx;

	vector<enum Object::ObjectType> lastPixels;
	for( size_t i = 0; i < mNumGreenSearchPoints; ++i) {
		lastPixels.push_back( Object::UNKNOWN);
	}
	struct YUVImage::YUVData green;

	for (int x = 0; x < width; x += dx) {
		int greenNotSeenCounter = 0;
		int lastGreenRow = height;
		//firstGreenFound = false;

		for( size_t i = 0; i < mNumGreenSearchPoints; ++i) {
			lastPixels[i] = Object::UNKNOWN;
		}

		for (int y = height - 1; y >= 0; y -= dy) { //
			struct YUVImage::YUVData currentColor = yuvImage->getValue(x, y);

//			DRAW_POINT("Scanlines", column, row, DebugDrawer::BLACK);

			for( size_t i = 0; i < mNumGreenSearchPoints - 1; ++i) {
				//lint --e{864}
				lastPixels[i] = lastPixels[i+1];
			}
			lastPixels[mNumGreenSearchPoints-1] = Object::UNKNOWN;

			if (currentColor.U < mMaxU_Green && currentColor.V < mMaxV_Green /*&& currentColor.Y < mMaxY_Green*/
					/*&& (currentColor.U + currentColor.V + currentColor.Y) < mMaxSum_Green*/) {
				int counter = 0;
				lastPixels[mNumGreenSearchPoints-1] = Object::FIELD_LINE;
				for( size_t i = 0; i < mNumGreenSearchPoints; ++i) {
					if( lastPixels[i] == Object::FIELD_LINE) {
						counter++;
					}
				}
				//if( y > (lastGreenRow - maxGreenNotSeen)) {
				if( counter >= mMinGreenPoints) {
					greenNotSeenCounter = 0;
					lastGreenRow = y;
					//firstGreenFound = true;
					green = currentColor;
				}
//				if( mDebugDrawings == 8) {
//					DRAW_POINT("Field", x, y, DebugDrawer::GREEN);
//				}
			} /*NOT USED ANYMORE ...
			else if (currentColor.U < mMaxU_Goal
					&& currentColor.V >= mMinV_Goal && currentColor.V < mMaxV_Goal
					&& currentColor.Y >= mMinY_Goal
					&& (currentColor.V + currentColor.U) < mMaxVaddU_Goal) {

				if( mDebugDrawings == 4) {
					DRAW_POINT("Goal", x, y, DebugDrawer::YELLOW);
				}
				getGoalPoints().addPoint(x, y);
				greenNotSeenCounter = 0;
			} else if(currentColor.U < mMaxU_Ball
					&& currentColor.V >= mMinV_Ball) {
				//Debugger::INFO("Scan", "Ball pixel: (%d, %d) : %d, %d, %d", column, row, currentColor.Y, currentColor.U, currentColor.V);
				//DRAW_POINT("BallDetection", column, row, DebugDrawer::RED);
				if( mDebugDrawings == 3) {
					DRAW_POINT("Ball", x, y, DebugDrawer::RED);
				}
				getBallPoints().addPoint(x, y);
//			}*/ else if (currentColor.Y >= mMinY_White) {
//			//} else if ((currentColor.U + currentColor.V + currentColor.Y) >= mMinSum_White) {
//				//lastGreenRow = y;
//				greenNotSeenCounter = 0;
//				/*linePixelCounter += dy;
//				if( mIgnoreObjectsOutsideField && linePixelCounter > maxLinePixel) {
//					break;
//				}*/
//				if( mDebugDrawings == 2) {
//					DRAW_POINT("Line", x, y, DebugDrawer::WHITE);
//				}
//				getWhitePoints().addPoint(x, y);
			} else if(currentColor.U >= mMinU_Magenta && currentColor.V >= mMinV_Magenta) {
//				if( mDebugDrawings == 5) {
//					DRAW_POINT("Robot", x, y, DebugDrawer::MAGENTA);
//				}
				getMagentaPoints().addPoint(x, y);
			} else if (currentColor.U >= mMinU_Cyan && currentColor.V < mMaxV_Cyan) {
//				if( mDebugDrawings == 6) {
//					DRAW_POINT("Robot", x, y, DebugDrawer::CYAN);
//				}
				getCyanPoints().addPoint(x, y);
			} else {
//				if( mDebugDrawings == 7) {
//					DRAW_POINT("Obstacle", x, y, DebugDrawer::BLACK);
//				}
//				getObstaclePoints().addPoint(x, y);
//				++greenNotSeenCounter;
//			}
			}

			if (mIgnoreObjectsOutsideField && greenNotSeenCounter > mMaxGreenNotSeen) {
				break;
			}
		}

		if( !(x % fieldLineScanDivisor)) {
			getGreenPoints().addPoint(x, lastGreenRow);
#ifdef _DEBUG
			if( mDebugDrawings == 1) {
				DRAW_POINT("FieldLine", x, lastGreenRow, DebugDrawer::GREEN);
			}
#endif
		}
//		if( !(column % 4))
//			Debugger::INFO("Scan", "Green: (%d, %d) : %d, %d, %d", column, lastGreenRow, green.Y, green.U, green.V);
	}

	return true;
}

void Scanlines::configChanged() {
	Debugger::INFO("Scanlines", "config changed");

	ConfigFile *config = Config::getInstance();
	mIgnoreObjectsOutsideField 	= config->get<bool>("Vision", "ignoreObjectsOutsideField", false);//true
	mScanlineDistanceColumn		= config->get<int>("Vision", "scanlineDistanceColumn", 3);
	mScanlineDistanceRow		= config->get<int>("Vision", "scanlineDistanceRow", 3);
	mMinCloudDistance 			= config->get<int>("Vision", "minCloudDistance", 10);
	mMinCloudDistanceRow 		= mMinCloudDistance / mScanlineDistanceRow;
	mMinCloudDistanceColumn 	= mMinCloudDistance / mScanlineDistanceColumn;
	mFieldLineScanDivisor		= config->get<int>("Vision", "fieldLineScanDivisor", 4);
	mMaxGreenNotSeen 			= config->get<int>("Vision", "maxGreenNotSeen", 50);
	mMaxGreenNotSeen /= mScanlineDistanceRow; // maxGreenNotSeen independent of scanlineDistanceRow
	mMinGreenPoints = config->get<int>("Vision", "minGreenPoints", 8);
	mNumGreenSearchPoints = config->get<size_t>("Vision", "numGreenSearchPoints", (size_t)10);

	if( mScanlineDistanceColumn < 1) {
		Debugger::WARN("Scanlines", "ScanlineDistanceColumn was too low (%d), set to 1!", mScanlineDistanceColumn);
		mScanlineDistanceColumn = 1;
	}
	if( mScanlineDistanceRow < 1) {
		Debugger::WARN("Scanlines", "ScanlineDistanceRow was too low (%d), set to 1!", mScanlineDistanceRow);
		mScanlineDistanceRow = 1;
	}

	mDebugDrawings = config->get<int>("Vision", "Scan_debugDrawings", 0);

	//Green
	//mMaxY_Green = config->get<int>("Vision", "Field_maxY", 220);//190
	//mMinU_Green = config->get<int>("Vision", "Field_minU", 100);
	mMaxU_Green = config->get<int>("Vision", "Field_maxU", 180);//160
	mMaxV_Green = config->get<int>("Vision", "Field_maxV", 100);//100//90//88//120
	//mMaxSum_Green = config->get<int>("Vision", "Field_maxSum", 430);//410//400

	//Debugger::DEBUG("Scanlines", "Green: maxY: %d, maxU: %d, maxV: %d", maxY_Green, maxU_Green, maxV_Green);

	mMinY_Ball = config->get<int>("Vision", "Ball_minY",  50);//70
	mMaxU_Ball = config->get<int>("Vision", "Ball_maxU", 130);//140
	mMinV_Ball = config->get<int>("Vision", "Ball_minV", 160);//140

	//Debugger::DEBUG("Scanlines", "Ball: minY: %d, maxU: %d, minV: %d", minY_Ball, maxU_Ball, minV_Ball);

	mMinY_Goal = config->get<int>("Vision", "Goal_minY",  80);//140
	mMaxU_Goal = config->get<int>("Vision", "Goal_maxU", 105);//110//120//100
	mMinV_Goal = config->get<int>("Vision", "Goal_minV", 100);//100//90
	mMaxV_Goal = config->get<int>("Vision", "Goal_maxV", 180);//160//140
	mMaxVaddU_Goal = config->get<int>("Vision", "Goal_maxVaddU", 240);

	//Debugger::DEBUG("Scanlines", "Goal: minY: %d, maxU: %d, maxV: %d", minY_Goal, maxU_Goal, maxV_Goal);

	mMaxY_Black 	= config->get<int>("Vision", "Obstacle_maxY", 80);//60
	mMinU_Magenta	= config->get<int>("Vision", "Magenta_minU", 140);//160
	mMinV_Magenta	= config->get<int>("Vision", "Magenta_minV", 140);//160
	mMinU_Cyan		= config->get<int>("Vision", "Cyan_minU", 160);//160
	mMaxV_Cyan		= config->get<int>("Vision", "Cyan_maxV", 110);//80

	//mMaxVariance_Gray = config->get<int>("Vision", "Gray_maxVariance", 30);
	//mMinSum_Gray = config->get<int>("Vision", "Gray_minSum", 350);

	mMinY_White  = config->get<int>("Vision", "Line_minY", 210);//240//200
	mMinSum_White = config->get<int>("Vision", "Line_minSum", 350);
}

#endif // SCANLINES_MULTITHREAD
