/*
 * BodyController.cpp
 *
 *  Created on: 27.11.2012
 *      Author: Stefan Krupop
 */

#include "BodyController.h"

BodyController::BodyController(Body* body) {
	mBody = body;
	mPanTiltLastProcessedID = 0;
	mMovementLastProcessedID = 0;
}

BodyController::~BodyController() {
	mBody = NULL;
}

bool BodyController::execute() {
	const HeadAction& ha = getHeadAction();

	if (ha.getPanTiltChangeID() != mPanTiltLastProcessedID) {
		if (ha.getPanTiltSetDirect()) {
			mBody->PanTilt()->SetPosition((double)ha.getPan(), (double)ha.getTilt());
		} else {
			if (ha.getPanTiltWithError()) {
				mBody->PanTilt()->MoveTracking(ha.getPanTiltErrorX(), ha.getPanTiltErrorY(), ha.getDisablePanError());
			} else {
				mBody->PanTilt()->MoveTracking(ha.getDisablePanError());
			}
		}
		mPanTiltLastProcessedID = ha.getPanTiltChangeID();
	}

	const BodyAction& ba = getBodyAction();

	if (ba.getMovementChangeID() != mMovementLastProcessedID) {
		if (ba.getIsWalkerCommand()) {
			mBody->setWalkerCommand(ba.getWalkerX(), ba.getWalkerY(), ba.getWalkerR(), ba.getWalkerTurningAim());
		} else {
			switch (ba.getMotion()) {
				case ICommunicator::Stop:
					mBody->Stop();
					break;
				case ICommunicator::KickLeft:
					mBody->KickLeft();
					break;
				case ICommunicator::KickRight:
					mBody->KickRight();
					break;
				case ICommunicator::CancelKick:
					mBody->CancelKick();
					break;
				case ICommunicator::KickBall:
					mBody->KickBall( ba.getPositionBall());
					break;
				case ICommunicator::KickToTarget:
					mBody->KickToTarget( ba.getPositionBall(), ba.getPositionTarget());
					break;
				case ICommunicator::KickAdvanced:
					mBody->KickAdvanced( ba.getPositionBall(), ba.getPositionTarget(), ba.getKicktype());
					break;
				case ICommunicator::GoaliMoveLeft:
					mBody->GoaliMoveLeft();
					break;
				case ICommunicator::GoaliMoveRight:
					mBody->GoaliMoveRight();
					break;
				case ICommunicator::MoveHi:
					mBody->MoveHi();
					break;
				case ICommunicator::YeahGoal:
					mBody->YeahGoal();
					break;
				case ICommunicator::RaiseRightHand:
				case ICommunicator::RaiseLeftHand:
				case ICommunicator::RaiseBothHands:
				default:
					mBody->PlayMotion(ba.getMotion());
					break;
			}
		}
		mMovementLastProcessedID = ba.getMovementChangeID();
	}

	return true;
}
