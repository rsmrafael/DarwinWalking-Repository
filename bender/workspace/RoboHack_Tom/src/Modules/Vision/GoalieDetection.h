/*
 * GoalieDetection.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef GoalieDetection_H_
#define GoalieDetection_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/PointCloud.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/FieldLines.h"
#include "../../ConfigChangeListener.h"
#include "../../Game.h"

BEGIN_DECLARE_MODULE(GoalieDetection)
REQUIRE(ObjectList, GoalPoles)
//REQUIRE(PointCloud, MagentaPoints)
//REQUIRE(PointCloud, CyanPoints)
PROVIDE(Object, GoalKeeper)
END_DECLARE_MODULE(GoalieDetection)

class GoalieDetection: public GoalieDetectionBase,
public ConfigChangeListener {
public:
	GoalieDetection();
	virtual ~GoalieDetection();

	virtual bool execute();
	virtual void configChanged();

private:
};

#endif /* GoalieDetection_H_ */
