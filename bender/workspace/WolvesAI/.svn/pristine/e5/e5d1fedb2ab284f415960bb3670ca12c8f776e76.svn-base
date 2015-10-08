/*
 * BackgroundScan.cpp
 *
 *  Created on: 21.03.2014
 *      Author: Oliver Krebs
 */

#include "BackgroundScan.h"
#include "../../Config.h"
#include "../../Debugging/DebugDrawer.h"

BackgroundScan::BackgroundScan()
{
	mScanBackground = true;
	getBackgroundYUV().setType(0, 63, 1);
	Config::GetInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

BackgroundScan::~BackgroundScan()
{
	Config::GetInstance()->removeConfigChangeHandler(this);
}

bool BackgroundScan::execute()
{
	getBackgroundY().clearHist();
	getBackgroundU().clearHist();
	getBackgroundV().clearHist();
	getBackgroundYUV().clearHist();

	if( !mScanBackground || getGoalPoles().getObjects().empty()) {
		return true;
	}

	const YUVImage* yuvImage = &getImage();
	int width = yuvImage->getWidth();
	int dx = 4;
	int dy = 4;
	for (int x = 0; x < width; x += dx) {
		int maxY = getFieldLines().getY(x);
		int minY = maxY - (dy * 20);
		if( minY < 0) {
			minY = 0;
		}
		for (int y = minY; y < maxY; y += dy) {
			struct YUVImage::YUVData data = yuvImage->getValue(x, y);
			// test if pixel is not goal pixel
			if (data.U >= mMaxU_Goal
				|| (data.V + data.U) >= mMaxVaddU_Goal) {
					getBackgroundY().addData( data.Y);
					getBackgroundU().addData( data.U);
					getBackgroundV().addData( data.V);

					//Perform color quantization
					data.Y /= 64;
					data.U /= 64;
					data.V /= 64;
					int colorQuantized = data.Y * 16 + data.U * 4 + data.V;

					getBackgroundYUV().addData(colorQuantized);
			}
		}
	}
	getBackgroundYUV().normalizeToByte();
	return true;
}

void BackgroundScan::configChanged()
{
	//mMinY_Goal = Config::GetInt("Vision", "Goal_minY",  80);
	mMaxU_Goal = Config::GetInt("Vision", "Goal_maxU", 110);
	mMaxVaddU_Goal = Config::GetInt("Vision", "Goal_maxVaddU", 240);
	mScanBackground = Config::GetBoolean("Vision", "scanBackground", true);
}
