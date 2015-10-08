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

/**
 * OpenCV capture device
 */
class CaptureOpenCV: public CaptureOpenCVBase {
public:

	/**
	 * Construcotr for OpenCV capturing
	 */
	CaptureOpenCV();

	/**
	 * Destructor
	 */
	virtual ~CaptureOpenCV();

	/**
	 * update image
	 */
	virtual bool execute();

private:
#ifdef USE_OPENCV
	void setCameraParameters();		//!< update camera parameters

	cv::VideoCapture mCapture;		//!< capture device
#endif

	char* mData;					//!< data
	int mWidth;						//!< width of image
	int mHeigth;					//!< height of image
	uint8_t mChangedState;			//!< changed state
};

#endif /* CAPTUREOPENCV_H_ */
