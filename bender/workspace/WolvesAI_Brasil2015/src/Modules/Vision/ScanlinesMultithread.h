/*
 * Scanlines.h
 *
 *  Created on: 29.08.2012
 *      Author: Stefan Krupop
 */

#ifndef SCANLINES_MULTITHREAD_H_
#define SCANLINES_MULTITHREAD_H_

#ifdef SCANLINES_MULTITHREAD
#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/PointCloud.h"
#include "../../Representations/BodyStatus.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(Scanlines)
	REQUIRE(YUVImage,Image)
	REQUIRE(BodyStatus, BodyStatus)
	PROVIDE(PointCloud,GreenPoints)
	PROVIDE(PointCloud,WhitePoints)
	PROVIDE(PointCloud,GoalPoints)
	PROVIDE(PointCloud,MagentaPoints)
	PROVIDE(PointCloud,CyanPoints)
	PROVIDE(PointCloud,ObstaclePoints)
	PROVIDE(PointCloud,BallPoints)
END_DECLARE_MODULE(Scanlines)

/**
 * Scanlines with use of multiple threads
 */
class Scanlines: public ScanlinesBase, public ConfigChangeListener {
public:
	Scanlines();
	virtual ~Scanlines();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

	/**
	 * config has changed
	 * @see ConfigChangeListener::configChanged
	 */
	virtual void configChanged();

private:
	class ScanlineThread : public Thread {
	public:
		ScanlineThread(Scanlines *parent, int startX, int endX);
		virtual ~ScanlineThread();

	//protected:
		virtual void execute(void* arg);

	//private:
		Scanlines *mParent;
		int mStartX;
		int mEndX;

		PointCloud *greenPoints;
		PointCloud *goalPoints;
		PointCloud *ballPoints;
		PointCloud *whitePoints;
		PointCloud *cyanPoints;
		PointCloud *magentaPoints;
		PointCloud *obstaclePoints;
	};

	void addBallPointToNearestCloud(Point point, int compV);

	void threadFinished();

	int mThreadCounter;
	pthread_mutex_t mThreadsMutex;

	int mImageWidth;
	int mImageHeight;
	int mStepX;
	int mStepY;

	/**
	 * Config Parameters
	 */

	int mNumberOfThreads;
	int mScanlineDistanceColumn;
	int mScanlineDistanceRow;
	int mMaxGreenNotSeen;
	int mMinCloudDistance;
	int mMinCloudDistanceRow;
	int mMinCloudDistanceColumn;
	int mFieldLineScanDivisor;
	int mFieldLineScanDivisorStep;
	size_t mNumGreenSearchPoints;
	int mMinGreenPoints;

	bool mIgnoreObjectsOutsideField;

	// Green (Field)
	int mMaxY_Green;
	int mMinU_Green;
	int mMaxU_Green;
	int mMaxV_Green;
	int mMaxSum_Green;

	int mMinY_Ball;
	int mMinV_Ball;
	int mMaxU_Ball;

	// Goal (yellow)
	int mMinY_Goal;
	int mMaxU_Goal;
	int mMinV_Goal;
	int mMaxV_Goal;
	int mMaxVaddU_Goal;

	// White (line)
	int mMinY_White;
	int mMinSum_White;

	int mMaxY_Black;

	// Gray
	int mMaxVariance_Gray;

	// Cyan
	int mMinU_Cyan;
	int mMaxV_Cyan;

	// Magenta
	int mMinU_Magenta;
	int mMinV_Magenta;

	int mDebugDrawings;
};
#endif //SCANLINES_MULTITHREAD

#endif /* SCANLINES_MULTITHREAD_H_ */
