/*
 * OpenCVImage.h
 *
 *  Created on: 1.11.2012
 *      Author: Jan Carstensen
 */

#ifndef OpenCVImage_H_
#define OpenCVImage_H_

#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#endif

using namespace std;

class OpenCVImage {
public:

	OpenCVImage();
	virtual ~OpenCVImage();

#ifdef USE_OPENCV

	void showImage();

	void updateImage(cv::Mat imageData, int width, int height);

	void updateYUVImage(const char* imageData, int width, int height);

	int getWidth() const {
		return mWidth;
	}
	int getHeight() const {
		return mHeight;
	}

	const cv::Mat getImage() const {
		return mData;
	}

	const cv::Mat getOpenCVYUV() const{
		return mYUVData;
	}

	const cv::Mat getChannelY() const {
		return mYData;
	}

	const cv::Mat getChannelCB() const {
		return mCbData;
	}

	const cv::Mat getChannelCR() const {
		return mCrData;
	}

	const char* getYUVImage() const;

private:
	int mWidth;
	int mHeight;

	cv::Mat mData;
	cv::Mat mYData;
	cv::Mat mCbData;
	cv::Mat mCrData;
	cv::Mat mDrawnData;
	cv::Mat mYUVData;

#endif
};

#endif /* OpenCVImage_H_ */
