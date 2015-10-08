/*
 * BehaviorGoalie.cpp
 *
 *  Created on: 26.03.2014
 *      Author: Oliver Krebs
 */

#include "BehaviorGoalie.h"
#include "BehaviorConstants.h"
#include "../../Config.h"
#include "../../Utils/Constants.h"
#include <cmath>

BehaviorGoalie::BehaviorGoalie(Game *game, MitecomCommunicator *communicator)
:	AbstractBehavior( game, communicator)
{
	mThisRole = ROLE_GOALIE;
	mReactTilt = 30;
	mReactDistance = 200.0;
	mCurrentState = SEARCH_BALL;
	entrySearchBall();

	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

BehaviorGoalie::~BehaviorGoalie()
{
	Config::getInstance()->removeConfigChangeHandler(this);
}

void BehaviorGoalie::configChanged()
{
	ConfigFile *config = Config::getInstance();
	mAllowKick 			= config->get<bool>("AI", "allowKick", true);
	mKickLeftTurnAngle 	= config->get<int>("AI", "kickLeftTurnAngle", -30);
	mKickRightTurnAngle = config->get<int>("AI", "kickRightTurnAngle", 30);
	mTurningXMovement   = config->get<int16_t>("AI", "turningXMovement", 0);//Bot4: -40
	mReactTilt 			= config->get<int>("Goalie", "reactTilt", 30);
	mReactDistance 		= config->get<double>("Goalie", "reactDistance", 200.0);
	mPrepareKickBallDistance    = config->get<int>("AI", "prepareKickBallDistance", 250);
	mReactRole 			= config->getString("Goalie", "reactAI", "goalie");
	if (!mAllowKick) {
		// unreachable, so no kick is executed
		mKickBallDistance = -100;
	} else {
		mKickBallDistance = config->get<int>("AI", "kick_ballDistance", 200);
	}
}

void BehaviorGoalie::executeBehavior()
{
	switch( mCurrentState) {
	case SEARCH_BALL:
		mCurrentState = processSearchBall();
		break;
	case TRACK_BALL:
		mCurrentState = processTrackBall();
		break;
	case SAVE_BALL:
		mCurrentState = processSaveBall();
		break;
	case LOCALIZE_AND_POSITION:
		mCurrentState = processLocalizeAndPosition();
		break;
	case DELAY_LOOP:
		mCurrentState = processDelayLoop();
		break;
	case PREPARE_KICK:
		mCurrentState = processPrepareKick();
		break;
	case KICK_BALL:
		mCurrentState = processKickBall();
		break;
	default:
		Debugger::INFO("Goalie", "Unknown state, change to searchBall");
		mCurrentState = SEARCH_BALL;
		break;
	}
}

void BehaviorGoalie::updateState()
{
	if (mCurrentState != mLastState) {
		switch( mCurrentState) {
		case SEARCH_BALL:
			entrySearchBall();
			break;
		case TRACK_BALL:
			entryTrackBall();
			break;
		case SAVE_BALL:
			entrySaveBall();
			break;
		case LOCALIZE_AND_POSITION:
			entryLocalizeAndPosition();
			break;
		case PREPARE_KICK:
			entryPrepareKick();
			break;
		case KICK_BALL:
			entryKickBall();
			break;
		default:
			mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_WAITING;
			//Debugger::announce("Waiting");
			break;
		}
		Debugger::INFO("Goalie", "Changed state %d to %d", mLastState, mCurrentState);
		mLastState = mCurrentState;
	}
}

/**
 * SearchBall
 */
void BehaviorGoalie::entrySearchBall()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_UNDEFINED;
	getHeadAction().searchObject(Object::BALL, true);
	Debugger::announce("Searching ball");
}

int BehaviorGoalie::processSearchBall()
{
	Debugger::DEBUG("Goalie", "State: SearchBall");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	switch(status) {
	case VisionStatus::OBJECT_FOUND_STABLE:
		Debugger::INFO("Goalie", "Ball found stable => track ball");
		getBodyAction().stop();
		return TRACK_BALL;
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND:
		Debugger::INFO("Goalie", "Ball not found => turn");
		getBodyAction().setWalkerCommand(mTurningXMovement, 0, 100);
		initDelayLoop(TURN_DURATION);
		return DELAY_LOOP;
	case VisionStatus::OBJECT_SEARCHING:
	case VisionStatus::OBJECT_SEEN:
	default:
		getBodyAction().stop();
		return SEARCH_BALL;
	}
}

/**
 * TrackBall
 */
void BehaviorGoalie::entryTrackBall()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_WAITING;
	getHeadAction().searchObject(Object::BALL, false);
	Debugger::announce("Track Ball");
}

int BehaviorGoalie::processTrackBall()
{
	Debugger::DEBUG("Goalie", "State: TrackBall");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	switch(status) {
	case VisionStatus::OBJECT_FOUND_STABLE:
		Debugger::INFO("Goalie", "Ball found stable");
		getBodyAction().stop();
		// old code
		/*if (panTiltCorrection) {
			int correction = abs(getBodyStatus().getPan());

			if (correction > maxPanAngle) {
				correction = maxPanAngle;
			}
			correction = (int) (correction / panTiltScaling);

			if (getBodyStatus().getTilt() + correction < reactTilt) {
				mCurrentState = reactToSaveBall;
				return true;
			}

		} else {
			if (getBodyStatus().getTilt() < reactTilt) {
				mCurrentState = reactToSaveBall;
				return true;
			}
		}*/

		// predicted ball is near enough for reactDistance -> react!
		if( getBallPredictVector().getLength() < mReactDistance) {
			if( mReactRole == "goalie") {
				return SAVE_BALL;
			} else {
				changeRole(mReactRole);
			}
		} else if( getBallPredictVector().getLength() < mPrepareKickBallDistance) {
			return PREPARE_KICK;
		}
		return TRACK_BALL;
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND:
		Debugger::INFO("Goalie", "Ball not found => search");
		getBodyAction().stop();
		return SEARCH_BALL;
	case VisionStatus::OBJECT_SEARCHING:
	case VisionStatus::OBJECT_SEEN:
	default:
		getBodyAction().stop();
		return TRACK_BALL;
	}
}

/**
 * SaveBall
 */
void BehaviorGoalie::entrySaveBall()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_UNDEFINED;
	getHeadAction().searchObject(Object::UNKNOWN, false);
	Debugger::announce("Save ball");
}

int BehaviorGoalie::processSaveBall()
{
	/*if (getBodyStatus().getPan() < 0) {
		getBodyAction().GoaliMoveLeft();
	} else {
		getBodyAction().GoaliMoveRight();
	}
	mIsBallSeenStable = false;*/
	changeRole(ROLE_STRIKER);
	return SEARCH_BALL;
}

/**
 * LocalizeAndPosition
 */
void BehaviorGoalie::entryLocalizeAndPosition()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_POSITIONING;
	getHeadAction().searchObject(Object::UNKNOWN, false);
}

int BehaviorGoalie::processLocalizeAndPosition()
{
	Debugger::DEBUG("Goalie", "State: LocalizeAndPosition");
	GoalSide::eGoalSide goalSide = getGoalSidePrediction().getGoalSide();
	if( goalSide == GoalSide::MY_GOAL) {
		vector<Vector> poles = getGoalPolesVectors().get();//.getDistanceSortedObjects();
		vector<Vector>::const_iterator it;
		for( it = poles.begin(); it != poles.end(); ++it) {
			double angle = it->getAngle();
			if( angle > -PI_2 && angle < PI_2) {
				// my pole
			}
		}
	} else {
		Debugger::DEBUG("Goalie", "Search my side, turn!");
		getBodyAction().circleLeft();
	}
	return LOCALIZE_AND_POSITION;
}

/**
 * PrepareKick
 */
void BehaviorGoalie::entryPrepareKick()
{
	//if( mSearchGoal) {
		//getHeadAction().setPanTilt(mLastBallPan, mLastBallTilt);
	//}
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_TRYING_TO_SCORE;
	getHeadAction().searchObject(Object::BALL, false);
	Debugger::announce("Prepare Kick");
}

int BehaviorGoalie::processPrepareKick()
{
	Debugger::DEBUG("Goalie", "State: PrepareKick");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	Debugger::INFO("Goalie", "BallStatus: %d", status);
	switch(status) {
	case VisionStatus::OBJECT_SEARCHING:
		Debugger::DEBUG("Goalie", "PrepareKick: Ball searching");
		getBodyAction().stop();
		return PREPARE_KICK;
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND:
		Debugger::INFO("Goalie", "PrepareKick: Ball lost!");
		getBodyAction().stop();
		return SEARCH_BALL;
	default:
		int pan = getBodyStatus().getPan();
		int tilt = getBodyStatus().getTilt();

		if(pan > mKickLeftTurnAngle && pan < mKickRightTurnAngle) {
			if( getBallVector().getX() < mKickBallDistance
						&& getBallVector().getY() > -60
						&& getBallVector().getY() < +60) {
				return KICK_BALL;
			} else {
				int16_t fbStep, rlStep;
				calculateWalkSteps( pan, tilt, rlStep, fbStep);
				getBodyAction().setWalkerCommand(fbStep, 0, rlStep);
				Debugger::DEBUG("Goalie", "PrepareKick: forward %d, rotating %d", fbStep, rlStep);
				//Debugger::INFO("Goalie", "FOLLOW2 P:%.2f T:%.2f", pan, tilt);
			}
		} else {
			int16_t rlStep;
			int16_t yStep;
			calculateTurnWalkSteps( pan, rlStep,yStep);
			getBodyAction().setWalkerCommand(mTurningXMovement, yStep, rlStep);
			Debugger::DEBUG("Goalie", "PrepareKick: rotating %d", rlStep);
		}
		return PREPARE_KICK;
	}
}

/**
 * KickBall
 */
void BehaviorGoalie::entryKickBall()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_TRYING_TO_SCORE;
	getHeadAction().searchObject(Object::BALL, false);
	Debugger::announce("Kick");
}

int BehaviorGoalie::processKickBall()
{
	Debugger::DEBUG("Goalie", "State: Kick");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	Debugger::INFO("Goalie", "BallStatus: %d", status);
	switch( status) {
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND: //if( !getBall().lastImageSeen) {
		Debugger::INFO("Goalie", "KickBall: Ball lost!");
		getBodyAction().stop();
		return SEARCH_BALL;
	case VisionStatus::OBJECT_SEARCHING:
		getBodyAction().stop();
		return KICK_BALL;
	default:
		break;
	}

	int pan = getBodyStatus().getPan();
	if(pan < 0) {
		Debugger::INFO("Goalie", "Kick left");
		getBodyAction().kickLeft();
	} else {
		Debugger::INFO("Goalie", "Kick right");
		getBodyAction().kickRight();
	}
	return SEARCH_BALL;
}

string BehaviorGoalie::getStateName(int state) {
	switch(state){
	case SEARCH_BALL:
		return "SEARCH_BALL";
	case TRACK_BALL:
		return "TRACK_BALL";
	case SAVE_BALL:
		return "SAVE_BALL";
	case LOCALIZE_AND_POSITION:
		return "LOCALIZE_AND_POSITION";
	case PREPARE_KICK:
		return "PREPARE_KICK";
	case KICK_BALL:
		return "KICK_BALL";
	default:
		return AbstractBehavior::getStateName(state);
	}
}
