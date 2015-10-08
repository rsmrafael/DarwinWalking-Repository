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
#include "../../Representations/Vector.h"
#include "../../Representations/BodyStatus.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(MovementPredictor)
	//REQUIRE(RobotConfiguration, RobotConfiguration)
	REQUIRE(BodyStatus, BodyStatus)
	REQUIRE(Object, Ball)
	REQUIRE(Vector, BallVector)
	//-----------------------------------------------
	PROVIDE(Object, BallPredict)
	PROVIDE(Vector, BallPredictVector)
END_DECLARE_MODULE(MovementPredictor)

/**
 * MovementPredictor predicts movements for objects like the ball
 */
class MovementPredictor : public MovementPredictorBase, public ConfigChangeListener {
public:
	MovementPredictor();
	virtual ~MovementPredictor();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

	/**
	 * config has changed
	 * @see ConfigChangeListener::configChanged
	 */
	virtual void configChanged();

private:
	void predictMovement(const Vector &current, Vector *last, Vector *predict);
	void predictMovement(const Object &currentObj, Object *lastObj, Object *predictObj);

	Object mLastBall;
	Vector mLastBallVector;
	int mCurrentPan;
	int mCurrentTilt;
	int mLastPan;
	int mLastTilt;
};

#endif /* MOVEMENTPREDICTOR_H_ */
