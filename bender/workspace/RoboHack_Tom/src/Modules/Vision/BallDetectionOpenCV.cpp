/*
 * BallDetectionOpenCV.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#include "BallDetectionOpenCV.h"
#include "../../Debugging/Debugger.h"

#ifdef USE_OPENCV

#endif

BallDetectionOpenCV::BallDetectionOpenCV() {
}

BallDetectionOpenCV::~BallDetectionOpenCV() {

}

bool BallDetectionOpenCV::execute() {
#ifdef USE_OPENCV

	//get cr channel

	const cv::Mat& mat = getOpenCVImage().getChannelCR();

	cv::Point minLoc;
	cv::Point maxLoc;
	double minVal;
	double maxVal;

	cv::Mat blurred;

	cv::GaussianBlur(mat, blurred, cv::Size(21,21), 0, 0, cv::BORDER_DEFAULT);

	cv::minMaxLoc(blurred, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());


//	Debugger::DEBUG("BallDetectionOpenCV", "maxVal: %f", maxVal);
//
//	Debugger::DEBUG("BallDetectionOpenCV", "minLoc :%d | %d ", minLoc.x,
//			minLoc.y);

//	vector<cv::Vec3f> circles;
//
//	/// Apply the Hough Transform to find the circles
//	cv::HoughCircles(mat, circles, CV_HOUGH_GRADIENT, 1, mat.rows / 8, 50, 100,
//			0, 0);
//
//	for (size_t i = 0; i < circles.size(); ++i) {
//		Object ball;
//
//		ball.lastImageX = circles[i].val[0];
//		ball.lastImageY = circles[i].val[1];
//		ball.lastImageWidth = circles[i].val[2];
//		ball.lastImageHeigth = circles[i].val[2];
//
//		Debugger::DEBUG("BallDetectionOpenCV", "Circle Found");
//
//		getBalls().addObject(ball);
//	}
//
//
//


//	cv::Mat thresholded;
//
//	cv::threshold(mat, thresholded, maxVal-50, 255, cv::THRESH_BINARY);
//
//	cv::imshow("test", thresholded);
//
//	vector<cv::Mat> contours;
//
//	cv::findContours(thresholded, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));





	if (maxVal > 200) {
		getBall().updateObject(maxLoc.x, maxLoc.y, maxLoc.x, maxLoc.y);

	} else {
		getBall().notSeen();
	}

#endif
	return true;
}
