/*
 * OpenCVMat.h
 *
 *  Created on: 01.07.2015
 *      Author: bombadil
 */

#ifndef OPENCVMAT_H_
#define OPENCVMAT_H_

#include <opencv2/opencv.hpp>

class OpenCVMat {
public:
	OpenCVMat();
	virtual ~OpenCVMat();
	const cv::Mat& getMat() const {
		return mat;
	}
	void setMat(cv::Mat& m){
		m.copyTo(mat);
	}
private:
	cv::Mat mat;
};

#endif /* OPENCVMAT_H_ */
