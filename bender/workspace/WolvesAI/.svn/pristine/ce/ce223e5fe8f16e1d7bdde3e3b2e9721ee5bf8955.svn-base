/*
 * HorizonDetection.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef HorizonDetection_H_
#define HorizonDetection_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/Line.h"
#include "../../Representations/BodyStatus.h"
#include "../../Representations/RobotConfiguration.h"
#include "../../Representations/YUVImage.h"
#include "../../ConfigChangeListener.h"


BEGIN_DECLARE_MODULE(HorizonDetection)
	REQUIRE(BodyStatus,BodyStatus)
	REQUIRE(RobotConfiguration, RobotConfiguration)
	REQUIRE(YUVImage,Image)
	PROVIDE(Line, Horizon)
END_DECLARE_MODULE(HorizonDetection)

class HorizonDetection: public HorizonDetectionBase, public ConfigChangeListener {
public:
	HorizonDetection();
	virtual ~HorizonDetection();

	virtual bool execute();
	virtual void configChanged();

private:

	int tiltCorrection;
};

#endif /* HorizonDetection_H_ */
