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

/**
 * Representation of an OpenCV image
 */
class OpenCVImage {
public:

	/**
	 * Constructor
	 */
	OpenCVImage();

	/**
	 * Destructor
	 */
	virtual ~OpenCVImage();

#ifdef USE_OPENCV

	/**
	 * update image
	 * @param imageData		BGR data of image in a matrix
	 * @param width			width of image in pixel
	 * @param height 		height of image in pixel
	 */
	void updateImage(const cv::Mat &imageData, int width, int height);

	/**
	 * update the yuv image
	 * @param imageData		pointer to image data
	 * @param width			width of image in pixel
	 * @param height 		height of image in pixel
	 */
	void updateYUVImage(const char* imageData, int width, int height);

	/**
	 * get the width of the image in pixel
	 */
	int getWidth() const {
		return mWidth;
	}

	/**
	 * get the height of the image in pixel
	 */
	int getHeight() const {
		return mHeight;
	}

	/**
	 * get the image data matrix
	 */
	const cv::Mat getImage() const {
		return mData;
	}

	/**
	 * get the YUV data matrix
	 */
	const cv::Mat getOpenCVYUV() const{
		return mYUVData;
	}

	/**
	 * get the Y channel matrix
	 */
	const cv::Mat getChannelY() const {
		return mYData;
	}

	/**
	 * get the Cb channel matrix
	 */
	const cv::Mat getChannelCB() const {
		return mCbData;
	}

	/**
	 * get the Cr channel matrix
	 */
	const cv::Mat getChannelCR() const {
		return mCrData;
	}

	/**
	 * create a yuv image from the data
	 * Important: You have to free the memory after calling this function to avoid memory leaks!
	 * @return pointer to yuv image data
	 */
	const char* createYUVImage() const;

private:
	int mWidth;			//!< width of image in pixel
	int mHeight;		//!< height of image in pixel

	cv::Mat mData;		//!< data of image
	cv::Mat mYData;		//!< Y data
	cv::Mat mCbData;	//!< Cb data
	cv::Mat mCrData;	//!< Cr data
	cv::Mat mYUVData;	//!< yuv data of image

#endif
};

#endif /* OpenCVImage_H_ */
