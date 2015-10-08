/*
 * ConvertOpenCV.h
 *
 *  Created on: Aug 28, 2012
 *      Author: jan
 */

#ifndef CONVERTOPENCV_H_
#define CONVERTOPENCV_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/CameraSettings.h"
#include "../../Representations/OpenCVImage.h"

BEGIN_DECLARE_MODULE(ConvertOpenCV)
	REQUIRE(YUVImage,Image)
	PROVIDE(OpenCVImage,OpenCVImage)
END_DECLARE_MODULE(ConvertOpenCV)

/**
 * Provides OpenCVImages created from YUVImage
 */
class ConvertOpenCV: public ConvertOpenCVBase {
public:

	/**
	 * Constructor
	 */
	ConvertOpenCV();

	/**
	 * Destructor
	 */
	virtual ~ConvertOpenCV();

	/**
	 * execute
	 */
	virtual bool execute();
};

#endif /* CONVERTOPENCV_H_ */
