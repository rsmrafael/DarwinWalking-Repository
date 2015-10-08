/*
 * MovementPredictor.h
 *
 *  Created on: May 22, 2013
 *      Author: CHO
 */

#ifndef MOVEMENTPREDICTOR_H_
#define MOVEMENTPREDICTOR_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/Object.h"
#include "../../Representations/BodyStatus.h"
#include "../../ConfigChangeListener.h"

using namespace std;

BEGIN_DECLARE_MODULE(MovementPredictor)
	//REQUIRE(RobotConfiguration, RobotConfiguration)
	REQUIRE(BodyStatus, BodyStatus)
	REQUIRE(Object, Ball)
	PROVIDE(Object, BallPredict)
END_DECLARE_MODULE(MovementPredictor)

class MovementPredictor : public MovementPredictorBase, public ConfigChangeListener {
public:
	MovementPredictor();
	virtual ~MovementPredictor();

	virtual bool execute();
	virtual void configChanged();

private:
	void predictMovement(const Object *currentObj, Object *lastObj, Object *predictObj);

	Object lastBall;
	int currentPan;
	int currentTilt;
	int lastPan;
	int lastTilt;

	int ballPredictX;
	int ballPredictY;
	int ballPredictSize;

};

#endif /* MOVEMENTPREDICTOR_H_ */
