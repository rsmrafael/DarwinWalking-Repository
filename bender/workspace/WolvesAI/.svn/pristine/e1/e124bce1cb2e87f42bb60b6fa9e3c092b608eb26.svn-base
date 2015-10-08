/*
 * BallDetectionOpenCV.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef BallDetectionOpenCV_H_
#define BallDetectionOpenCV_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/OpenCVImage.h"
#include "../../Representations/Object.h"
#include "../../Representations/PointCloud.h"
#include "../../Utils/Clustering/Ransac/Models/CircleModel.h"
#include "../../Representations/ObjectList.h"


BEGIN_DECLARE_MODULE(BallDetectionOpenCV)
	REQUIRE(OpenCVImage,OpenCVImage)
	REQUIRE(PointCloud,BallPoints)
	PROVIDE(PointCloud,BallEdges)
	PROVIDE(Object,Ball)
	PROVIDE(ObjectList,Balls)
END_DECLARE_MODULE(BallDetectionOpenCV)

class BallDetectionOpenCV: public BallDetectionOpenCVBase {
public:
	BallDetectionOpenCV();
	virtual ~BallDetectionOpenCV();

	virtual bool execute();
};

#endif /* BallDetectionOpenCV_H_ */
