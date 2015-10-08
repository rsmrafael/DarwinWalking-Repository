/*
 * CaptureOpenCV.h
 *
 *  Created on: Aug 28, 2012
 *      Author: jan
 */

#ifndef CAPTUREOPENCV_H_
#define CAPTUREOPENCV_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/OpenCVImage.h"
#include "../../Representations/CameraSettings.h"
#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#endif

BEGIN_DECLARE_MODULE(CaptureOpenCV)
	REQUIRE(CameraSettings,CameraSettings)
	PROVIDE(YUVImage,Image)
	PROVIDE(OpenCVImage,OpenCVImage)
END_DECLARE_MODULE(CaptureOpenCV)

class CaptureOpenCV: public CaptureOpenCVBase {
public:
	CaptureOpenCV();
	virtual ~CaptureOpenCV();

	virtual bool execute();

private:
#ifdef USE_OPENCV
	void setCameraParameters();

	cv::VideoCapture mCapture;
#endif

	char* mData;
	int mWidth;
	int mHeigth;
	uint8_t mChangedState;
};

#endif /* CAPTUREOPENCV_H_ */
