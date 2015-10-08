/*
 * BehaviorSupporter.cpp
 *
 *  Created on: 27.03.2014
 *      Author: Oliver Krebs
 */

#include "BehaviorSupporter.h"
#include "BehaviorConstants.h"
#include "../../Config.h"

BehaviorSupporter::BehaviorSupporter(Game *game, MitecomCommunicator *communicator)
:	AbstractBehavior( game, communicator)
{
	mThisRole = ROLE_SUPPORTER;
	mTurningXMovement = 0;
	mTowardsBallLeftTurnAngle = 0;
	mTowardsBallRightTurnAngle = 0;
	mTargetBallDistance = BALL_DISTANCE_SUPPORTER;
	mCurrentState = SEARCH_BALL;
	entrySearchBall();

	Config::GetInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

BehaviorSupporter::~BehaviorSupporter()
{
	Config::GetInstance()->removeConfigChangeHandler(this);
}

void BehaviorSupporter::configChanged()
{
	mAllowKick 				= Config::GetBoolean("AI", "allowKick", true);
	mKickLeftTurnAngle 		= Config::GetInt("AI", "kickLeftTurnAngle", -30);
	mKickRightTurnAngle 	= Config::GetInt("AI", "kickRightTurnAngle", 30);
	mTurningXMovement 		= Config::GetValue<int16_t>("AI", "turningXMovement", 0);//Bot4: -40
	mTowardsBallLeftTurnAngle 	= Config::GetInt("AI", "towardsBallLeftTurnAngle", -20);
	mTowardsBallRightTurnAngle 	= Config::GetInt("AI", "towardsBallRightTurnAngle", 20);
	mPrepareKickBallDistance    = Config::GetInt("AI", "prepareKickBallDistance", 250);
	if (!mAllowKick) {
		// unreachable, so no kick is executed
		mKickBallDistance = -100;
	} else {
		mKickBallDistance = Config::GetInt("AI", "kick_ballDistance", 200);
	}
}

void BehaviorSupporter::executeBehavior()
{
	switch( mCurrentState) {
	case SEARCH_BALL:
		mCurrentState = processSearchBall();
		break;
	case MOVE_NEAR_BALL:
		mCurrentState = processMoveNearBall();
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
		Debugger::INFO("Supporter", "Unknown state, change to searchBall");
		mCurrentState = SEARCH_BALL;
		break;
	}
}

void BehaviorSupporter::updateState()
{
	if (mCurrentState != mLastState) {
		switch( mCurrentState) {
		case SEARCH_BALL:
			entrySearchBall();
			break;
		case MOVE_NEAR_BALL:
			entryMoveNearBall();
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
		Debugger::INFO("Supporter", "Changed state %d to %d", mLastState, mCurrentState);
		mLastState = mCurrentState;
	}
}

void BehaviorSupporter::entrySearchBall()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_UNDEFINED;
	getHeadAction().searchObject(Object::BALL, true);
	Debugger::announce("Searching ball");
}

int BehaviorSupporter::processSearchBall()
{
	Debugger::DEBUG("Supporter", "State: SearchBall");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	switch(status) {
	case VisionStatus::OBJECT_FOUND_STABLE:
		Debugger::INFO("Striker", "Ball found stable => move near ball");
		getBodyAction().stop();
		return MOVE_NEAR_BALL;
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND:
		Debugger::INFO("Striker", "Ball not found => turn");
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

void BehaviorSupporter::entryMoveNearBall()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_GOING_TO_BALL;
	getHeadAction().searchObject(Object::BALL, false);
	Debugger::announce("Going for ball");
}

int BehaviorSupporter::processMoveNearBall()
{
	Debugger::DEBUG("Supporter", "State: MoveNearBall");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	switch(status) {
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND:
		Debugger::INFO("Striker", "Ball lost => search");
		getBodyAction().stop();
		return SEARCH_BALL;
	case VisionStatus::OBJECT_SEARCHING:
	case VisionStatus::OBJECT_SEEN:
		getBodyAction().stop();
		return MOVE_NEAR_BALL;
	default:
		break;
	}

	int pan = getBodyStatus().getPan();
	int tilt = getBodyStatus().getTilt();

	timeval currentTime;
	gettimeofday(&currentTime, 0);
	if( currentTime.tv_sec - getGameStatus().kickOffTime.tv_sec < 0) {
		if( getBall().lastVector.getLength() < CIRCLE_DISTANCE_MIN) {
			getBodyAction().stop();
			Debugger::INFO("Supporter", "Waiting kickOff, not moving nearer to ball");
			return MOVE_NEAR_BALL;
		}
	}

	if(pan > mTowardsBallLeftTurnAngle && pan < mTowardsBallRightTurnAngle) {
		int distance = (int)getBall().lastVector.getLength();
		if( distance < mTargetBallDistance) {
			if( distance < mPrepareKickBallDistance) {
				getBodyAction().stop();
				return PREPARE_KICK;
			} else {
				getBodyAction().stop();
				return MOVE_NEAR_BALL;
			}
		} else {
			int16_t fbStep, rlStep;
			calculateWalkSteps( pan, tilt, rlStep, fbStep);
			getBodyAction().setWalkerCommand(fbStep, 0, rlStep);
			Debugger::DEBUG("Supporter", "MoveNearBall: forward %d, rotating %d", fbStep, rlStep);
		}
	} else {
		int16_t rlStep;
		int16_t yStep;
		calculateTurnWalkSteps( pan, rlStep, yStep);
		getBodyAction().setWalkerCommand(mTurningXMovement, yStep, rlStep);
		Debugger::DEBUG("Supporter", "MoveNearBall: rotating %d", rlStep);
	}
	return MOVE_NEAR_BALL;
}

/**
 * PrepareKick
 */
void BehaviorSupporter::entryPrepareKick()
{
	//if( mSearchGoal) {
		//getHeadAction().setPanTilt(mLastBallPan, mLastBallTilt);
	//}
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_TRYING_TO_SCORE;
	getHeadAction().searchObject(Object::BALL, false);
	Debugger::announce("Prepare Kick");
}

int BehaviorSupporter::processPrepareKick()
{
	Debugger::DEBUG("Supporter", "State: PrepareKick");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	Debugger::INFO("Supporter", "BallStatus: %d", status);
	switch(status) {
	case VisionStatus::OBJECT_SEARCHING:
		Debugger::DEBUG("Supporter", "PrepareKick: Ball searching");
		getBodyAction().stop();
		return PREPARE_KICK;
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND:
		Debugger::INFO("Supporter", "PrepareKick: Ball lost!");
		getBodyAction().stop();
		return SEARCH_BALL;
	default:
		int pan = getBodyStatus().getPan();
		int tilt = getBodyStatus().getTilt();

		if(pan > mKickLeftTurnAngle && pan < mKickRightTurnAngle) {
			if( getBall().lastVector.getX() < mKickBallDistance
						&& getBall().lastVector.getY() > -60
						&& getBall().lastVector.getY() < +60) {
				return KICK_BALL;
			} else {
				int16_t fbStep, rlStep;
				calculateWalkSteps( pan, tilt, rlStep, fbStep);
				getBodyAction().setWalkerCommand(fbStep, 0, rlStep);
				Debugger::DEBUG("Supporter", "PrepareKick: forward %d, rotating %d", fbStep, rlStep);
				//Debugger::INFO("Striker", "FOLLOW2 P:%.2f T:%.2f", pan, tilt);
			}
		} else {
			int16_t rlStep;
			int16_t yStep;
			calculateTurnWalkSteps( pan, rlStep,yStep);
			getBodyAction().setWalkerCommand(mTurningXMovement, yStep, rlStep);
			Debugger::DEBUG("Supporter", "PrepareKick: rotating %d", rlStep);
		}
		return PREPARE_KICK;
	}
}

/**
 * KickBall
 */
void BehaviorSupporter::entryKickBall()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_TRYING_TO_SCORE;
	getHeadAction().searchObject(Object::BALL, false);
	Debugger::announce("Kick");
}

int BehaviorSupporter::processKickBall()
{
	Debugger::DEBUG("Supporter", "State: Kick");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	Debugger::INFO("Supporter", "BallStatus: %d", status);
	switch( status) {
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND: //if( !getBall().lastImageSeen) {
		Debugger::INFO("Supporter", "KickBall: Ball lost!");
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
		Debugger::INFO("Supporter", "Kick left");
		getBodyAction().kickLeft();
	} else {
		Debugger::INFO("Supporter", "Kick right");
		getBodyAction().kickRight();
	}
	return SEARCH_BALL;
}

string BehaviorSupporter::getStateName(int state) {
	switch(state){
	case SEARCH_BALL:
		return "SEARCH_BALL";
	case MOVE_NEAR_BALL:
		return "MOVE_NEAR_BALL";
	case PREPARE_KICK:
		return "PREPARE_KICK";
	case KICK_BALL:
		return "KICK_BALL";
	default:
		return AbstractBehavior::getStateName(state);
	}
}
