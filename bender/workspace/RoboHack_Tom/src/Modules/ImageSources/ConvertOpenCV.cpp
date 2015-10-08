/*
 * ConvertOpenCV.cpp
 *
 *  Created on: Aug 28, 2012
 *      Author: jan
 */
#include "ConvertOpenCV.h"
#include "../../Debugging/Debugger.h"

#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#endif


ConvertOpenCV::ConvertOpenCV() {
}

ConvertOpenCV::~ConvertOpenCV() {

}

bool ConvertOpenCV::execute() {
#ifdef USE_OPENCV
	const char* imageData = getImage().getImage();
	int width = getImage().getWidth();
	int height = getImage().getHeight();

	cv::Mat yuvFrame(height, width, CV_8UC3);

	int pixelCount = width * height / 2;
	for (int i = 0; i < pixelCount; ++i) {
		yuvFrame.data[6*i] = imageData[4*i];
		yuvFrame.data[6*i+1] = imageData[4*i+1];
		yuvFrame.data[6*i+2] = imageData[4*i+3];
		yuvFrame.data[6*i+3] = imageData[4*i+2];
		yuvFrame.data[6*i+4] = imageData[4*i+1];
		yuvFrame.data[6*i+5] = imageData[4*i+3];
	}

	cv::Mat bgrFrame;
	cv::cvtColor(yuvFrame, bgrFrame, CV_YUV2BGR);

	getOpenCVImage().updateImage(bgrFrame, width, height);
#endif

	return true;
}

