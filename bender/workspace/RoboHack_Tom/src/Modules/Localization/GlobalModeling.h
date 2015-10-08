/*
 * GlobalModeling.h
 *
 *  Created on: 26.06.2014
 *      Author: Oliver Krebs
 */

#ifndef GLOBALMODELING_H_
#define GLOBALMODELING_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/Object.h"
#include "../../Representations/Position.h"

BEGIN_DECLARE_MODULE(GlobalModeling)
	REQUIRE(Object,BallRelative)
	PROVIDE(Position,BallAbsolute)
END_DECLARE_MODULE(GlobalModeling)

class GlobalModeling
:	public GlobalModelingBase
{
public:
	GlobalModeling();
	virtual ~GlobalModeling();

	virtual bool execute();
};

#endif /* GLOBALMODELING_H_ */
