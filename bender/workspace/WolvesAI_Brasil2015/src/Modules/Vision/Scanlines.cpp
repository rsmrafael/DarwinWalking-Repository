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
#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#endif

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
	mDebugDrawings( 0),
	mMinU_Ball(100)

#ifdef USE_OPENCV
	,mCamWidth(800)
	,mCamHeight(800)
	,mContoursWhite(1)
	,mContoursGreen(1)
	,mKernelSizeContur(10)
	,mMinWhiteCloud(1500)
	,mWhiteCounter(0)
#endif
{
	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();

#ifdef USE_OPENCV
	mContoursWhite = cv::Mat(mCamHeight, mCamWidth, CV_8UC3);
	mContoursGreen = cv::Mat(mCamHeight, mCamWidth, CV_8UC3);
#endif
}

Scanlines::~Scanlines() {
	Config::getInstance()->removeConfigChangeHandler(this);
}

bool Scanlines::execute() {
	int whiteCounter = 0;
	const YUVImage* yuvImage = &getImage();
	int width = yuvImage->getWidth();
	int height = yuvImage->getHeight();

#ifdef USE_OPENCV
	mContoursWhite.zeros(mCamHeight, mCamWidth, CV_8UC3);
	mContoursGreen.zeros(mCamHeight, mCamWidth, CV_8UC3);
#endif

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

	cv::Vec3b greenColor = cv::Vec3b(255, 0, 0);
	cv::Vec3b whiteColor = cv::Vec3b(255, 255, 90);
	cv::Vec3b blackColor = cv::Vec3b(0, 127, 127);

	for (int x = 0; x < width; x += dx) {
		int greenNotSeenCounter = 0;
		int lastGreenRow = height;

		for( size_t i = 0; i < mNumGreenSearchPoints; ++i) {
			lastPixels[i] = Object::UNKNOWN;
		}

		for (int y = height - 1; y >= 0; y -= dy) { //
			struct YUVImage::YUVData currentColor = yuvImage->getValue(x, y);

			for( size_t i = 0; i < mNumGreenSearchPoints - 1; ++i) {
				lastPixels[i] = lastPixels[i+1];
			}
			lastPixels[mNumGreenSearchPoints-1] = Object::UNKNOWN;

			if (currentColor.U < mMaxU_Green && currentColor.V < mMaxV_Green && currentColor.Y < mMaxY_Green) {
				int counter = 0;
				lastPixels[mNumGreenSearchPoints-1] = Object::FIELD_LINE;
				for( size_t i = 0; i < mNumGreenSearchPoints; ++i) {
					if( lastPixels[i] == Object::FIELD_LINE) {
						counter++;
					}
				}
				if( counter >= mMinGreenPoints) {
					greenNotSeenCounter = 0;
					lastGreenRow = y;
					green = currentColor;
				}
#ifdef USE_OPENCV
				mContoursGreen.at<cv::Vec3b>(y, x) = greenColor;
#endif
			} else if (currentColor.Y >= mMinY_Ball && currentColor.U >= mMinU_Ball) {
#ifdef USE_OPENCV
				mContoursWhite.at<cv::Vec3b>(y, x) = whiteColor;
				whiteCounter++;
#else
				getBallPoints().addPoint(x, y);
#endif
			} else if(currentColor.U >= mMinU_Magenta && currentColor.V >= mMinV_Magenta) {
				getMagentaPoints().addPoint(x, y);
			} else if (currentColor.U >= mMinU_Cyan && currentColor.V < mMaxV_Cyan) {
				getCyanPoints().addPoint(x, y);
			} else {
#ifdef USE_OPENCV
//				mEasyConturs.at<cv::Vec3b>(y, x) = blackColor;
#endif
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
	}
#ifdef USE_OPENCV
	getContourGreen().setMat(mContoursGreen);
 	getContourWhite().setMat(mContoursWhite);
	//TODO switch
//	getWhiteCounter().
#endif
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
#ifdef USE_OPENCV
	mCamWidth 					= config->get<int>("Camera", "width", 800); //352
	mCamHeight 					= config->get<int>("Camera", "height", 600); //288
#endif

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
	mMaxY_Green = config->get<int>("Vision", "Field_maxY", 180);//180
	//mMinU_Green = config->get<int>("Vision", "Field_minU", 100);
	mMaxU_Green = config->get<int>("Vision", "Field_maxU", 180);//160
	mMaxV_Green = config->get<int>("Vision", "Field_maxV", 100);//100//90//88//120
	//mMaxSum_Green = config->get<int>("Vision", "Field_maxSum", 430);//410//400

	//Debugger::DEBUG("Scanlines", "Green: maxY: %d, maxU: %d, maxV: %d", maxY_Green, maxU_Green, maxV_Green);

	mMinY_Ball = config->get<int>("Vision", "Ball_minY",  50);//70
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

	mMinY_White  = config->get<int>("Vision", "White_minY", 210);//240//200
	mMinSum_White = config->get<int>("Vision", "Line_minSum", 350);

	//new Mats for Contours
	mMinY_Ball 			= config->get<int>("Vision", "Ball_minY", 150);
	mMinU_Ball 			= config->get<int>("Vision", "Ball_minU", 100);
#ifdef USE_OPENCV
	mKernelSizeContur	= config->get<int>("Vision", "Ball_kernelSizeContur", 5);
	mMinWhiteCloud 		= config->get<int>("Vision", "Ball_minWhiteScanPoints", 1500);
#endif
}

#endif // SCANLINES_MULTITHREAD
