/*
 * VisionVisualisation.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef VisionVisualisation_H_
#define VisionVisualisation_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/FieldLines.h"
#include "../../Representations/Object.h"
#include "../../Representations/PointCloud.h"
#include "../../Representations/OpenCVImage.h"
#include "../../Representations/DebugDrawings.h"
#include "../../Representations/Point.h"
#include "../../Representations/ObjectList.h"


BEGIN_DECLARE_MODULE(VisionVisualisation)
	REQUIRE(OpenCVImage,OpenCVImage)
END_DECLARE_MODULE(VisionVisualisation)

/**
 * VisionVisualisation
 */
class VisionVisualisation: public VisionVisualisationBase {
public:
	VisionVisualisation();
	virtual ~VisionVisualisation();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

private:
	int mDebugDrawings;
};

#endif /* VisionVisualisation_H_ */
