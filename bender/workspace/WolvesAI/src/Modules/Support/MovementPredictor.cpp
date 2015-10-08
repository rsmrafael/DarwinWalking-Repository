/*
 * MovementPredictor.cpp
 *
 *  Created on: May 22, 2013
 *      Author: CHO
 */


#include "MovementPredictor.h"
#include "../../Debugging/Debugger.h"

MovementPredictor::MovementPredictor()
:	mLastBall(),
 	mLastBallVector(),
	mCurrentPan(0),
	mCurrentTilt(0),
	mLastPan(0),
	mLastTilt(0)
{
}

MovementPredictor::~MovementPredictor() {

}

bool MovementPredictor::execute() {

	mCurrentPan = getBodyStatus().getPan();
	mCurrentTilt = getBodyStatus().getTilt();

	// prediction is problematic, so dont use
	//predictMovement(getBallVector(), &mLastBallVector, &getBallPredictVector());
	//predictMovement(getBall(), &mLastBall, &getBallPredict());
	getBall().copyTo(getBallPredict());
	getBallVector().copyTo(getBallPredictVector());

#ifdef _DEBUG
	if( getBallPredict().lastImageSeen) {
		DRAW_CIRCLE("Ball", getBallPredict().lastImageTopLeftX, getBallPredict().lastImageTopLeftY,
			getBallPredict().lastImageWidth,
					DebugDrawer::BLACK);
	}
#endif
	/*DRAW_CIRCLE("BallDetection", ballPredictX, ballPredictY,
				ballPredictSize, DebugDrawer::BLUE);

	Debugger::INFO("MovementPredictor", "BallPredict: %d, %d; %d",
			ballPredictX, ballPredictY, ballPredictSize);
*/
	mLastPan = mCurrentPan;
	mLastTilt = mCurrentTilt;
	return true;
}

void MovementPredictor::predictMovement(const Vector &current, Vector *last, Vector *predict) {
	// TODO use pan / tilt for prediction
	double angle = 2.0*current.getAngle() - last->getAngle();
	double length = 2.0*current.getLength() - last->getLength();

	predict->updateVector(angle, length);
	last->updateVector(current.getAngle(), current.getLength());
}

void MovementPredictor::predictMovement(const Object &currentObj, Object *lastObj, Object *predictObj) {
	if(currentObj.lastImageSeen) {
		int x = currentObj.lastImageTopLeftX;
		int y = currentObj.lastImageTopLeftY;
		int w = currentObj.lastImageWidth;
		int h = currentObj.lastImageHeight;
		if( lastObj->lastImageSeen) {
			int newX = 2*x - lastObj->lastImageTopLeftX;
			int newY = 2*y - lastObj->lastImageTopLeftY;
			int newW = 2*w - lastObj->lastImageWidth;
			int newH = 2*h - lastObj->lastImageHeight;
			predictObj->updateObject(newX, newY, newW, newH);
		} else {
			predictObj->updateObject(x, y, w, h);
			//predictObj->lastImageSeen = false;
		}
		lastObj->updateObject(x, y, w, h);
	} else {
		lastObj->notSeen();
		predictObj->notSeen();
	}
}

void MovementPredictor::configChanged() {

}
