/*
 * Scanlines.h
 *
 *  Created on: 29.08.2012
 *      Author: Stefan Krupop
 */

#ifndef SCANLINES_H_
#define SCANLINES_H_

#ifndef SCANLINES_MULTITHREAD
#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/PointCloud.h"
#include "../../Representations/BodyStatus.h"
#include "../../Representations/OpenCVMat.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(Scanlines)
	REQUIRE(YUVImage,Image)
	REQUIRE(BodyStatus, BodyStatus)
	//---------------------------------
	PROVIDE(PointCloud,GreenPoints)
	PROVIDE(PointCloud,WhitePoints)
	PROVIDE(PointCloud,GoalPoints)
	PROVIDE(PointCloud,MagentaPoints)
	PROVIDE(PointCloud,CyanPoints)
	PROVIDE(PointCloud,ObstaclePoints)
	PROVIDE(PointCloud,BallPoints)
#ifdef USE_OPENCV
	PROVIDE(OpenCVMat,ContourWhite)
	PROVIDE(OpenCVMat,ContourGreen)
	PROVIDE(OpenCVMat,EasyContour)
//	PROVIDE(Integer,WhiteCounter)
#endif
END_DECLARE_MODULE(Scanlines)

/**
 * Scanlines scans the image and separates it to several point clouds
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
	void addBallPointToNearestCloud(Point point, int compV);

	/**
	 * Config Parameters
	 */
#ifdef USE_OPENCV
	size_t mCamWidth;
	size_t mCamHeight;
	size_t mKernelSizeContur;
	size_t mMinWhiteCloud;
#endif
	int mScanlineDistanceColumn;
	int mScanlineDistanceRow;
	int mMaxGreenNotSeen;
	int mMinCloudDistance;
	int mMinCloudDistanceRow;
	int mMinCloudDistanceColumn;
	int mFieldLineScanDivisor;
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
	size_t mMinU_Ball;

#ifdef USE_OPENCV
	size_t mWhiteCounter;
	cv::Mat mContoursWhite;
	cv::Mat mContoursGreen;
#endif
};

#endif // SCANLINES_MULTITHREAD

#endif /* SCANLINES_H_ */
