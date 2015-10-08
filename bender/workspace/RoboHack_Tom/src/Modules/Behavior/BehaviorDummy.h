/*
 * BehaviorDummy.h
 *
 *  Created on: 20.10.2014
 *      Author: bombadil
 */

#ifndef BEHAVIORDUMMY_H_
#define BEHAVIORDUMMY_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/BodyStatus.h"
//aufgabe 3
#include "../../Representations/BodyAction.h"
//aufgabe 4
#include "../../Representations/VisionStatus.h"
#include "../../Representations/HeadAction.h"
//aufgabe 5
#include "../../Representations/Object.h"

BEGIN_DECLARE_MODULE(BehaviorDummy)
REQUIRE(BodyStatus,BodyStatus)
//aufgabe 3
PROVIDE(BodyAction, BodyAction)
//aufgabe 4
REQUIRE(VisionStatus, BallStatus)
PROVIDE(HeadAction, HeadAction)
//aufgabe 5
REQUIRE(Object, Ball)
END_DECLARE_MODULE(BehaviorDummy)

class BehaviorDummy: public BehaviorDummyBase {
public:
	virtual bool execute();
	BehaviorDummy();
	virtual ~BehaviorDummy();

private:
int cycle;
int state;

void incrementCycle();
void aufgabe1();
void aufgabe2();
void aufgabe3();
void aufgabe4();
void aufgabe5();
};



#endif /* BEHAVIORDUMMY_H_ */
