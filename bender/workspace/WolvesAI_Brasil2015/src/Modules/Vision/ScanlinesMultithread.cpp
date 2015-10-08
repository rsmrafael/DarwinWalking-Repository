/*
 * Scanlines.cpp
 *
 *  Created on: 29.08.2012
 *      Author: Stefan Krupop
 *      Edited: Oliver Krebs
 */
#ifdef SCANLINES_MULTITHREAD
#include "ScanlinesMultithread.h"
#include "../../Config.h"
#include "../../Representations/Object.h"

Scanlines::Scanlines()
:	mThreadCounter(0),
 	mNumberOfThreads( 4),
 	mScanlineDistanceColumn( 2),
	mScanlineDistanceRow( 2),
	mMaxGreenNotSeen( 20),
	mMinCloudDistance( 30),
	mMinCloudDistanceRow( 15),
	mMinCloudDistanceColumn( 15),
	mFieldLineScanDivisor( 4),
	mFieldLineScanDivisorStep( 4),
	mNumGreenSearchPoints( 8),
	mMinGreenPoints( 5),
	mIgnoreObjectsOutsideField( true),
	mImageWidth( 0),
	mImageHeight( 0),
	mStepX( 1),
	mStepY( 1),
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
	pthread_mutex_init(&mThreadsMutex, NULL);
	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

Scanlines::~Scanlines() {
	Config::getInstance()->removeConfigChangeHandler(this);
}

bool Scanlines::execute() {
	const YUVImage* yuvImage = &getImage();
	mImageWidth = yuvImage->getWidth();
	mImageHeight = yuvImage->getHeight();

	getBallPoints().clearCloud();
	getGoalPoints().clearCloud();
	getObstaclePoints().clearCloud();
	getCyanPoints().clearCloud();
	getMagentaPoints().clearCloud();
	getWhitePoints().clearCloud();
	getGreenPoints().clearCloud();

	mStepX = mScanlineDistanceColumn;
	mStepY = mScanlineDistanceRow;

#ifndef JENKINS
	int tilt = getBodyStatus().getTilt();
	if (tilt < 50) {
		mStepX *= 4;//8
		mStepY *= 4;//8
	} else if (tilt < 80) {
		mStepX *= 2;//4
		mStepY *= 2;//4
	}
#endif

	mFieldLineScanDivisorStep = mFieldLineScanDivisor * mStepX;

	int threadStepX = mImageWidth / mNumberOfThreads;
	if (threadStepX < 1) {
		threadStepX = 1;
	}
	pthread_mutex_lock(&mThreadsMutex);
	mThreadCounter = 0;
	pthread_mutex_unlock(&mThreadsMutex);

	// start threads
	std::vector<ScanlineThread*> threads;
	for (int x = 0; x < mImageWidth; x+=threadStepX) {
		int endX = x + threadStepX;
		if (endX > mImageWidth) {
			endX = mImageWidth;
		}
		ScanlineThread *thread = new ScanlineThread(this, x, endX);
		threads.push_back(thread);
		pthread_mutex_lock(&mThreadsMutex);
		mThreadCounter++;
		//Debugger::DEBUG("Scanlines", "New scanline thread: %d", mThreadCounter);
		pthread_mutex_unlock(&mThreadsMutex);
	}

	//Debugger::DEBUG("Scanlines", "Wait until finished");

	bool finished = false;
	while (!finished) {
		pthread_mutex_lock(&mThreadsMutex);
		finished = (mThreadCounter == 0);
		pthread_mutex_unlock(&mThreadsMutex);
	}

	//Debugger::DEBUG("Scanlines", "Threads have finished");

	// clean up
	std::vector<ScanlineThread*>::iterator it;
	for (it = threads.begin(); it != threads.end(); ++it) {
		//Debugger::DEBUG("Scanlines", "Join cloud");
		getBallPoints().joinWithoutCheck( (*it)->ballPoints);
		getGoalPoints().joinWithoutCheck( (*it)->goalPoints);
		getObstaclePoints().joinWithoutCheck( (*it)->obstaclePoints);
		getCyanPoints().joinWithoutCheck( (*it)->cyanPoints);
		getMagentaPoints().joinWithoutCheck( (*it)->magentaPoints);
		getGreenPoints().joinWithoutCheck( (*it)->greenPoints);
		delete (*it);
		//Debugger::DEBUG("Scanlines", "Cloud joined");
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

	if (mScanlineDistanceColumn < 1) {
		Debugger::WARN("Scanlines", "ScanlineDistanceColumn was too low (%d), set to 1!", mScanlineDistanceColumn);
		mScanlineDistanceColumn = 1;
	}
	if (mScanlineDistanceRow < 1) {
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

	mNumberOfThreads = config->get<int>("Vision", "Scanlines_threads", 8);
	if (mNumberOfThreads < 1) {
		config->set<int>("Vision", "Scanlines_threads", 1);
		mNumberOfThreads = 1;
	}
}

void Scanlines::threadFinished() {
	pthread_mutex_lock(&mThreadsMutex);
	mThreadCounter--;
	pthread_mutex_unlock(&mThreadsMutex);
	//Debugger::DEBUG("Scanlines", "Thread finished: %d", mThreadCounter);
}

//****************************************************************************

Scanlines::ScanlineThread::ScanlineThread(Scanlines *parent, int startX, int endX)
:	mParent(parent), mStartX(startX), mEndX(endX) {
	greenPoints 	= new PointCloud();
	goalPoints 		= new PointCloud();
	ballPoints 		= new PointCloud();
	whitePoints 	= new PointCloud();
	cyanPoints 		= new PointCloud();
	magentaPoints 	= new PointCloud();
	obstaclePoints 	= new PointCloud();
	start(this);
}

Scanlines::ScanlineThread::~ScanlineThread() {
	delete greenPoints;
	delete goalPoints;
	delete ballPoints;
	delete whitePoints;
	delete cyanPoints;
	delete magentaPoints;
	delete obstaclePoints;
}

void Scanlines::ScanlineThread::execute(void *args) {

	const YUVImage* yuvImage = &mParent->getImage();

	std::vector<enum Object::ObjectType> lastPixels;
	for (size_t i = 0; i < mParent->mNumGreenSearchPoints; ++i) {
		lastPixels.push_back( Object::UNKNOWN);
	}
	struct YUVImage::YUVData green;

	greenPoints->clearCloud();
	goalPoints->clearCloud();
	ballPoints->clearCloud();
	whitePoints->clearCloud();
	cyanPoints->clearCloud();
	magentaPoints->clearCloud();
	obstaclePoints->clearCloud();

	// Create Scan Lines from the bottom and rise them. Mark the last spot where a green Pixel is found.

	for (int x = mStartX; x < mEndX; x += mParent->mStepX) {
		int greenNotSeenCounter = 0;
		int lastGreenRow = mParent->mImageHeight;
		//firstGreenFound = false;

		for( size_t i = 0; i < mParent->mNumGreenSearchPoints; ++i) {
			lastPixels[i] = Object::UNKNOWN;
		}

		for (int y = mParent->mImageHeight - 1; y >= 0; y -= mParent->mStepY) { //
			struct YUVImage::YUVData currentColor = yuvImage->getValue(x, y);

	//			DRAW_POINT("Scanlines", column, row, DebugDrawer::BLACK);

			for (size_t i = 0; i < mParent->mNumGreenSearchPoints - 1; ++i) {
				//lint --e{864}
				lastPixels[i] = lastPixels[i+1];
			}
			lastPixels[mParent->mNumGreenSearchPoints-1] = Object::UNKNOWN;

			if (currentColor.U < mParent->mMaxU_Green
					&& currentColor.V < mParent->mMaxV_Green /*&& currentColor.Y < mMaxY_Green*/
					/*&& (currentColor.U + currentColor.V + currentColor.Y) < mMaxSum_Green*/) {
				int counter = 0;
				lastPixels[mParent->mNumGreenSearchPoints-1] = Object::FIELD_LINE;
				for (size_t i = 0; i < mParent->mNumGreenSearchPoints; ++i) {
					if( lastPixels[i] == Object::FIELD_LINE) {
						counter++;
					}
				}
				//if( y > (lastGreenRow - maxGreenNotSeen)) {
				if (counter >= mParent->mMinGreenPoints) {
					greenNotSeenCounter = 0;
					lastGreenRow = y;
					//firstGreenFound = true;
					green = currentColor;
				}
			} else if (currentColor.U < mParent->mMaxU_Goal
					&& currentColor.V >= mParent->mMinV_Goal && currentColor.V < mParent->mMaxV_Goal
					&& currentColor.Y >= mParent->mMinY_Goal
					&& (currentColor.V + currentColor.U) < mParent->mMaxVaddU_Goal) {
				goalPoints->addPoint(x, y);
				greenNotSeenCounter = 0;
			} else if (currentColor.U < mParent->mMaxU_Ball
					/*&& currentColor.Y >= mParent->mMinY_Ball*/
					&& currentColor.V >= mParent->mMinV_Ball) {
				//Debugger::INFO("Scan", "Ball pixel: (%d, %d) : %d, %d, %d", column, row, currentColor.Y, currentColor.U, currentColor.V);
				//DRAW_POINT("BallDetection", column, row, DebugDrawer::RED);
				ballPoints->addPoint(x, y);
			} else if (currentColor.Y >= mParent->mMinY_White) {
			//} else if ((currentColor.U + currentColor.V + currentColor.Y) >= mMinSum_White) {
				//lastGreenRow = y;
				greenNotSeenCounter = 0;
				/*linePixelCounter += dy;
				if( sIgnoreObjectsOutsideField && linePixelCounter > maxLinePixel) {
					break;
				}*/
				whitePoints->addPoint(x, y);
			} else if (currentColor.U >= mParent->mMinU_Magenta
					&& currentColor.V >= mParent->mMinV_Magenta) {
				magentaPoints->addPoint(x, y);
			} else if (currentColor.U >= mParent->mMinU_Cyan
					&& currentColor.V < mParent->mMaxV_Cyan) {
				cyanPoints->addPoint(x, y);
			} else {
				obstaclePoints->addPoint(x, y);
				++greenNotSeenCounter;
			}

			if (mParent->mIgnoreObjectsOutsideField
					&& greenNotSeenCounter > mParent->mMaxGreenNotSeen) {
				break;
			}
		}

		if (!(x % mParent->mFieldLineScanDivisorStep)) {
			greenPoints->addPoint(x, lastGreenRow);
		}
	}
	stop();
	mParent->threadFinished();
}
#endif //SCANLINES_MULTITHREAD
