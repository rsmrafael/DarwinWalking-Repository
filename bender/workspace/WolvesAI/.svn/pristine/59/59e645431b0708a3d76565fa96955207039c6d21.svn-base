/*
 * FieldDetection.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef FIELDDETECTION_H_
#define FIELDDETECTION_H_


#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/PointCloud.h"
#include "../../Representations/FieldLines.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(FieldDetection)
	REQUIRE(PointCloud,GreenPoints)
	REQUIRE(YUVImage,Image)
	PROVIDE(FieldLines, FieldLines)
END_DECLARE_MODULE(FieldDetection)

class FieldDetection
: public FieldDetectionBase,
  public ConfigChangeListener {
public:
	FieldDetection();
	virtual ~FieldDetection();

	virtual bool execute();
	virtual void configChanged();

private:
	size_t mNumIterations;
	size_t mMinPoints;
	double mMaxError;
	bool mShowDebugDrawings;
};

#endif /* FIELDDETECTION_H_ */
