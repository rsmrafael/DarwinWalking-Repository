/*
 * GlobalModeling.h
 *
 *  Created on: 26.06.2014
 *      Author: Oliver Krebs
 */

#ifndef GLOBALMODELING_H_
#define GLOBALMODELING_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/Odometry.h"
#include "../../Representations/Object.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/Position.h"
#include "../../Representations/BodyStatus.h"
#include "../../Representations/Vector.h"

using namespace std;

BEGIN_DECLARE_MODULE(GlobalModeling)
	REQUIRE(Odometry,Odometry)
	REQUIRE(BodyStatus,BodyStatus)
	REQUIRE(ObjectList, GoalPoles)
	REQUIRE(Object,BallRelative)
	PROVIDE(Position,BallAbsolute)
END_DECLARE_MODULE(GlobalModeling)

/**
 * GlobalModeling creates a global model for all objects (ball, goal poles, robots)
 * TODO implement ...
 */
class GlobalModeling
:	public GlobalModelingBase
{
public:
	GlobalModeling();
	virtual ~GlobalModeling();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

private:

};

#endif /* GLOBALMODELING_H_ */
