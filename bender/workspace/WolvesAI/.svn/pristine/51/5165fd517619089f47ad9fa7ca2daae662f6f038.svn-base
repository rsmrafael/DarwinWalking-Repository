/*
 * OpenCVImage.cpp
 *
 *  Created on: 1.11.2012
 *      Author: Jan Cartensen
 */

#include "OpenCVImage.h"
#include "../Debugging/Debugger.h"

#include <sstream>

#define SIZE_OF_YUV422_PIXEL	2

OpenCVImage::OpenCVImage() {
#ifdef USE_OPENCV

	// Default size
	mWidth = 320;
	mHeight = 288;

	cv::Mat dummy(288,320,CV_8U);

	mData = dummy;
	mYData = dummy;
	mCbData = dummy;
	mCrData = dummy;
	mDrawnData = dummy;

#endif

}

OpenCVImage::~OpenCVImage() {

}

#ifdef USE_OPENCV

void OpenCVImage::updateImage(cv::Mat imageData, int width, int height) {
	mData = imageData;
	mWidth = width;
	mHeight = height;

	std::vector<cv::Mat> channels;

	mYUVData.create(mHeight, mWidth, CV_8UC3);

	cv::cvtColor(mData, mYUVData, CV_BGR2YCrCb);

	cv::split(mYUVData, channels);

	mYData.create(mHeight, mWidth, CV_8U);
	mCbData.create(mHeight, mWidth, CV_8U);
	mCrData.create(mHeight, mWidth, CV_8U);
	mDrawnData.create(mHeight, mWidth, CV_8U);

	mYData = channels[0];
	mCbData = channels[2];
	mCrData = channels[1];
	mDrawnData = imageData;
}

void OpenCVImage::updateYUVImage(const char* imageData, int width, int heigth) {

	cv::Mat yuvFrame;

	yuvFrame.create(width, heigth, CV_8UC3);

	for (int i = 0; i < width * heigth / 2; ++i) {
		yuvFrame.data[6 * i] = imageData[4 * i];
		yuvFrame.data[6 * i + 1] = imageData[4 * i + 1];
		yuvFrame.data[6 * i + 2] = imageData[4 * i + 3];
		yuvFrame.data[6 * i + 3] = imageData[4 * i + 2];
		yuvFrame.data[6 * i + 4] = imageData[4 * i + 1];
		yuvFrame.data[6 * i + 5] = imageData[4 * i + 3];
	}

	updateImage(yuvFrame, width, heigth);
}

const char* OpenCVImage::getYUVImage() const {

	char* data = (char*) malloc(
			(size_t) (uint32_t) (mHeight * mWidth * SIZE_OF_YUV422_PIXEL));

	Debugger::DEBUG("OpenCVImage", "getYUVImage...");

	cv::Mat yuvFrame;
	yuvFrame.create(mWidth, mHeight, CV_8UC3);

	Debugger::DEBUG("OpenCVImage", "mData: %i , yuvFrame: %i", mData.dims,
			yuvFrame.dims);

	cv::cvtColor(mData, yuvFrame, CV_BGR2YCrCb);

	for (int i = 0; i < mWidth * mHeight / 2; ++i) {
		data[4 * i] = yuvFrame.data[6 * i];
		data[4 * i + 1] = yuvFrame.data[6 * i + 1];
		data[4 * i + 2] = yuvFrame.data[6 * i + 3];
		data[4 * i + 3] = yuvFrame.data[6 * i + 5];
	}

	Debugger::DEBUG("OpenCVImage", "return YUVImage...");

	return data;
}

#endif
