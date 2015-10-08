/*
 * BehaviorStriker.cpp
 *
 *  Created on: 26.03.2014
 *      Author: Oliver Krebs
 */

#include <cmath>
#include "BehaviorStriker.h"
#include "BehaviorConstants.h"
#include "../../Config.h"
#include "../../Utils/Constants.h"

//#define CHECK_KICKOFF_TIME // kick-off: stop robot moving into center circle
#define GOAL_TARGETING_AVAILABLE
//#define USE_CONST_POSITIONS // old version like in 2013

BehaviorStriker::BehaviorStriker(Game *game, MitecomCommunicator *communicator)
:	AbstractBehavior( game, communicator)
{
	mThisRole = ROLE_STRIKER;

	mBall.x = 100;
	mBall.y = 0;
	mBall.z = 40;
	mTarget.x = 1800;
	mTarget.y = 0;
	mTarget.z = 40;

	mSearchGoal = true;
	mAllowKick = true;
	mCheckCorrectGoal = true;

	mNumberOfKicks = 0;
	mNumberOfKickTries = 0;
	mNumberOfKickTriesMax = 10;

	mTowardsBallLeftTurnAngle = -20;
	mTowardsBallRightTurnAngle = 20;
	mKickLeftTurnAngle = -30;
	mKickRightTurnAngle = 30;
	mTurningXMovement = 0;
	mKickBallMovedRangeMin = 10;

	mKickingTryTimeout = 5000;
	mCircleToGoalEntryTime = 0;
	mCircleToGoalTimeout = 2000;
	mMyGoalCircleDelay = 2;
	mPositionForKickTimeout = 10000;
	mPositionForKickBegun = 0;
	mEntryScanFieldBegun = 0;
	mSearchNextPoleBegun = 0;
	mSearchNextPoleTimeout = 5000;

	mPositionBallDistanceMin = 100;
	mPositionBallDistanceMax = 200;
	mKickBallDistance = 100;
	mKickTargetDistanceMax = 1800;
	mRunToFront = 4000;

	mMoveToBallDistance = BALL_DISTANCE_STRIKER;
	mCurrentState = SEARCH_BALL;
	entrySearchBall();

	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

BehaviorStriker::~BehaviorStriker()
{
	Config::getInstance()->removeConfigChangeHandler(this);
}

void BehaviorStriker::configChanged()
{
	ConfigFile *config = Config::getInstance();
	mTarget.x 		 	= config->get<int16_t>("AI", "kickTargetX", 1800);
	mTarget.y 		 	= config->get<int16_t>("AI", "kickTargetY", 0);
	mTarget.z 		 	= config->get<int16_t>("AI", "kickTargetZ", 60);
	mBall.x 		 	= config->get<int16_t>("AI", "kickBallX", 120);
	mBall.y 		 	= config->get<int16_t>("AI", "kickBallY", 70);
	mBall.z 		 	= config->get<int16_t>("AI", "kickBallZ", 60);

	mSearchGoal 		= config->get<bool>("AI", "searchGoal", true);
	mAllowKick 			= config->get<bool>("AI", "allowKick", true);
	mCheckCorrectGoal   = config->get<bool>("AI", "checkCorrectGoal", true);
	mTurningXMovement 	= config->get<int16_t>("AI", "turningXMovement", 0);//Bot4: -40
	mTowardsBallLeftTurnAngle 	= config->get<int>("AI", "towardsBallLeftTurnAngle", -20);
	mTowardsBallRightTurnAngle 	= config->get<int>("AI", "towardsBallRightTurnAngle", 20);
	mKickLeftTurnAngle 			= config->get<int>("AI", "kickLeftTurnAngleb", -30);
	mKickRightTurnAngle 		= config->get<int>("AI", "kickRightTurnAngle", 30);
	mKickBallMovedRangeMin		= config->get<int>("AI", "ballMovedRangeMin", 100);//, "minimal range the ball has moved after kick");
	mNumberOfKickTriesMax	= config->get<int>("AI", "maxKickTries", 10);
	mMoveToBallDistance = config->get<int>("AI","moveToBallDistance",100);

	mCircleToGoalTimeout 	= config->get<uint64_t>("AI", "circleToGoalTimeout", 2000L);
	mMyGoalCircleDelay 		= config->get<uint64_t>("AI", "myGoalCircleDelay", 2000L);
	mPositionForKickTimeout = config->get<uint64_t>("AI", "positionForKickTimeout", 10000L);
	mKickingTryTimeout		= config->get<uint64_t>("AI", "kickingTryTimeout", 5000L);
	mSearchNextPoleTimeout	= config->get<uint64_t>("AI", "searchNextPoleTimeout", 5000L);
	mPositionBallDistanceMin = config->get<int>("AI", "position_ballDistanceMin", 100);
	mPositionBallDistanceMax = config->get<int>("AI", "position_ballDistanceMax", 200);
	mKickTargetDistanceMax = config->get<int>("AI", "kick_targetDistanceMax", 1800);//, "maximal possible distance of kick target");
	mCircleHalfDuration = config->get<uint64_t>("AI", "circleHalfDuration", 10000L);
	mRunToFront = config->get<int>("AI","runToFrontTime",4000);
	if (!mAllowKick) {
		// unreachable, so no kick is executed
		mKickBallDistance = -100;
	} else {
		mKickBallDistance = config->get<int>("AI", "kick_ballDistance", 200);
	}
}

void BehaviorStriker::gameStateHasChanged(Game::GameState state)
{
	if( state == IGame::READY) {
		Debugger::INFO("Striker", "GameState ready: Scan field");
		mCurrentState = SCAN_FIELD;
		updateState();
	} else if( state == IGame::SET
			|| state == IGame::PLAYING) {
		if( mCurrentState == SCAN_FIELD) {
			Debugger::INFO("Striker", "GameState set/play: Search ball");
			mCurrentState = SEARCH_BALL;
			updateState();
		}
	}
	AbstractBehavior::gameStateHasChanged( state);
}

void BehaviorStriker::executeBehavior()
{
	switch(mCurrentState) {
	case SEARCH_BALL:
		mCurrentState = processSearchBall();
		break;
	case MOVE_TO_BALL:
		mCurrentState = processMoveToBall();
		break;
	case POSITION_NEAR_BALL:
		mCurrentState = processPositionNearBall();
		break;
	case CIRCLE_TO_GOAL:
		mCurrentState = processCircleToGoal();
		break;
	case SEARCH_NEXT_POLE:
		mCurrentState = processSearchNextPole();
		break;
	case PREPARE_KICK:
		mCurrentState = processPrepareKick();
		break;
	case KICK_BALL:
		mCurrentState = processKickBall();
		break;
	case CHECK_KICK:
		mCurrentState = processCheckKick();
		break;
	case CIRCLE_HALF:
		mCurrentState = processMoveHalf();
		break;
	case SCAN_FIELD:
		mCurrentState = processScanField();
		break;
	case RUN_TO_FRONT:
		mCurrentState = processRunToFront();
		break;
	case DELAY_LOOP:
		mCurrentState = processDelayLoop();
		break;
	default:
		Debugger::INFO("Striker", "Unknown state, change to searchBall");
		mCurrentState = SEARCH_BALL;
		break;
	}
}

void BehaviorStriker::updateState()
{
	if (mCurrentState != mLastState) {
		switch( mCurrentState) {
		case SEARCH_BALL:
			entrySearchBall();
			break;
		case MOVE_TO_BALL:
			entryMoveToBall();
			break;
		case POSITION_NEAR_BALL:
			entryPositionNearBall();
			break;
		case CIRCLE_TO_GOAL:
			entryCircleToGoal();
			break;
		case SEARCH_NEXT_POLE:
			entrySearchNextPole();
			break;
		case PREPARE_KICK:
			entryPrepareKick();
			break;
		case KICK_BALL:
			entryKickBall();
			break;
		case CHECK_KICK:
			entryCheckKick();
			break;
		case CIRCLE_HALF:
			entryMoveHalf();
			break;
		case SCAN_FIELD:
			entryScanField();
			break;
		case RUN_TO_FRONT:
			entryRunToFront();
			break;
		default:
			mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_WAITING;
			//Debugger::announce("Waiting");
			break;
		}
		Debugger::INFO("Striker", "Changed state %d to %d", mLastState, mCurrentState);
		mLastState = mCurrentState;
	}
}

/**
 * SearchBall
 */
void BehaviorStriker::entrySearchBall()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_WAITING;
	getHeadAction().searchObject(Object::BALL, true);
	//Debugger::announce("Searching ball");
}

int BehaviorStriker::processSearchBall()
{
	Debugger::DEBUG("Striker", "State: SearchBall");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	Debugger::INFO("Striker", "BallStatus: %d", status);
	switch(status) {
	case VisionStatus::OBJECT_FOUND_STABLE:
		Debugger::INFO("Striker", "Ball found stable => move to ball");
		getBodyAction().stop();
		return MOVE_TO_BALL;
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

/**
 * MoveToBall
 */
void BehaviorStriker::entryMoveToBall()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_GOING_TO_BALL;
	getHeadAction().searchObject(Object::BALL, false);
	//Debugger::announce("Going for ball");
}

int BehaviorStriker::processMoveToBall()
{
	Debugger::DEBUG("Striker", "State: MoveToBall");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	Debugger::INFO("Striker", "BallStatus: %d", status);
	switch( status) {
	case VisionStatus::OBJECT_SEARCHING:
	case VisionStatus::OBJECT_NOT_FOUND:
	case VisionStatus::OBJECT_LOST:
		getBodyAction().stop();
		Debugger::INFO("Striker", "Move: Ball lost!");
		return SEARCH_BALL;
	default:
		break;
	}

	int pan = getBodyStatus().getPan();
	int tilt = getBodyStatus().getTilt();

#ifdef CHECK_KICKOFF_TIME
	timeval currentTime;
	gettimeofday(&currentTime, 0);
	if( currentTime.tv_sec - getGameStatus().kickOffTime.tv_sec < 0) {
		if( getBall().lastVector.getLength() < CIRCLE_DISTANCE_MIN) {
			getBodyAction().stop();
			Debugger::INFO("Striker", "Waiting kick-off, do not move nearer to ball");
			return MOVE_TO_BALL;
		}
	}
#endif

	if((int)getBallVector().getLength() < mMoveToBallDistance) {
		//getBodyAction().stop();
		if (!mSearchGoal) {
			Debugger::INFO("Striker", "Not looking for goal! %d",(int)getBallVector().getLength());
			return KICK_BALL;
			//return PREPARE_KICK;
		} else {
			mPositionForKickBegun = getCurrentTime();
			return POSITION_NEAR_BALL;
		}
	}
	if(pan > mTowardsBallLeftTurnAngle && pan < mTowardsBallRightTurnAngle) {
		int16_t fbStep, rlStep;
		calculateWalkSteps( pan, tilt, rlStep, fbStep);
		getBodyAction().setWalkerCommand(fbStep, 0, rlStep);
		Debugger::DEBUG("Striker", "forward %d, rotating %d", fbStep, rlStep);
	} else {
		int16_t rlStep;
		int16_t yStep;
		calculateTurnWalkSteps( pan, rlStep, yStep);
		getBodyAction().setWalkerCommand(mTurningXMovement, yStep, rlStep);
		Debugger::DEBUG("Striker", "rotating %d", rlStep);
	}
	return MOVE_TO_BALL;
}

/**
 * PositionNearBall
 */
void BehaviorStriker::entryPositionNearBall()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_POSITIONING;
	getHeadAction().searchObject(Object::BALL, false);
	//Debugger::announce("Position near ball");
}

int BehaviorStriker::processPositionNearBall()
{
	Debugger::DEBUG("Striker", "State: PositionNearBall");

	uint64_t currentTime = getCurrentTime();
	if( (currentTime - mPositionForKickBegun) > mPositionForKickTimeout) {
		Debugger::INFO("Striker", "P: PositionForKickTimeout reached => prepare kick");
		return PREPARE_KICK;
	}

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	Debugger::INFO("Striker", "BallStatus: %d", status);
	switch( status) {
	case VisionStatus::OBJECT_SEARCHING:
		getBodyAction().stop();
		return POSITION_NEAR_BALL;
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND:
		Debugger::INFO("Striker", "PositionNearBall: Ball lost!");
		getBodyAction().stop();
		return SEARCH_BALL;
	default:
		break;
	}

	int pan = getBodyStatus().getPan();
	int targetDistance = (mPositionBallDistanceMin + mPositionBallDistanceMax) / 2;
	int dist = (int)getBallVector().getLength();
	if( dist < mPositionBallDistanceMin) {
		dist -= targetDistance;
		double tilt = atan(
				(double) (dist) / getRobotConfiguration().cameraHeight)
				* RADIAN_TO_DEGREE;
		Debugger::DEBUG("Striker", "target dist: %d, tilt: %.1f", dist, tilt);
		int16_t fbStep, rlStep;
		calculateWalkSteps( pan, (int)tilt, rlStep, fbStep);
		fbStep *= 2;
		getBodyAction().setWalkerCommand(fbStep, 0, rlStep);
		Debugger::DEBUG("Striker", "backward: %d, rotating: %d", fbStep, rlStep);
	} else if( dist > mPositionBallDistanceMax) {
		if( dist > 2*mPositionBallDistanceMax) {
			return MOVE_TO_BALL;
		}
		dist -= targetDistance;
		double tilt = atan(
				(double) (dist) / getRobotConfiguration().cameraHeight)
				* RADIAN_TO_DEGREE;
		Debugger::DEBUG("Striker", "target dist: %d, tilt: %.1f", dist, tilt);
		int16_t fbStep, rlStep;
		calculateWalkSteps( pan, (int)tilt, rlStep, fbStep);
		getBodyAction().setWalkerCommand(fbStep, 0, rlStep);
		Debugger::DEBUG("Striker", "forward: %d, rotating: %d", fbStep, rlStep);
	} else {
		getBodyAction().stop();
		//return POSITION_NEAR_BALL;//for test
		return CIRCLE_TO_GOAL;
	}
	return POSITION_NEAR_BALL;
}

/**
 * CircleToGoal
 */
void BehaviorStriker::entryCircleToGoal()
{
	//getHeadAction().setPanTilt(mLastGoalPan, mLastGoalTilt);
	mCircleToGoalEntryTime = getCurrentTime();
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_POSITIONING;
	getHeadAction().searchObject(Object::GOAL_POLE_YELLOW, false);
	//Debugger::announce("Aiming towards goal");
}

int BehaviorStriker::processCircleToGoal()
{
	Debugger::DEBUG("Striker", "State: CircleToGoal");

	uint64_t currentTime = getCurrentTime();
	if( (currentTime - mPositionForKickBegun) > mPositionForKickTimeout) {
		Debugger::INFO("Striker", "C: PositionForKickTimeout reached => prepare kick");
		getBodyAction().stop();
		return PREPARE_KICK;
	} else if( (currentTime - mCircleToGoalEntryTime) > mCircleToGoalTimeout) {
		getBodyAction().stop();
		return POSITION_NEAR_BALL;
	}

	VisionStatus::ObjectStatus status = getGoalStatus().getObjectStatus();
	Debugger::INFO("Striker", "GoalStatus: %d", status);
	switch(status) {
	case VisionStatus::OBJECT_FOUND_STABLE:
		if( !mCheckCorrectGoal || getGoalSidePrediction().getGoalSide() == GoalSide::OP_GOAL) {
			int pan = getBodyStatus().getPan();
			if (pan > GOAL_RIGHT_ANGLE) {
				getBodyAction().circleRight();
				Debugger::INFO("Striker", "Circle right");
				return CIRCLE_TO_GOAL;
			} else if( pan < GOAL_LEFT_ANGLE) {
				getBodyAction().circleLeft();
				Debugger::INFO("Striker", "Circle left");
				return CIRCLE_TO_GOAL;
			} else {
				getBodyAction().stop();
				//return SEARCH_NEXT_POLE;
#ifndef USE_CONST_POSITIONS
				seekTarget();
#endif
				return PREPARE_KICK;
			}
		} else {
			Debugger::INFO("Striker", "It could be my goal, circle!");
			getBodyAction().circleLeft();
			return CIRCLE_TO_GOAL;
			// dont use delay, because its not save
			//initDelayLoop(mMyGoalCircleDelay);
			//return DELAY_LOOP;
		}
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND:
		getBodyAction().circleLeft();
		return CIRCLE_TO_GOAL;
		// dont use delay, because its not save
		//initDelayLoop(CIRCLE_DURATION);
		//return DELAY_LOOP;
	case VisionStatus::OBJECT_SEARCHING:
	case VisionStatus::OBJECT_SEEN:
	default:
		getBodyAction().stop();
		mCircleToGoalEntryTime = getCurrentTime(); // reset time
		return CIRCLE_TO_GOAL;
	}
}

void BehaviorStriker::entrySearchNextPole()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_POSITIONING;
	getHeadAction().searchObject(Object::UNKNOWN, true);
	mSearchNextPoleBegun = getCurrentTime();
}

int BehaviorStriker::processSearchNextPole()
{
	if( getGoalPolesVectors().get().size() >= 2
			|| (getCurrentTime() - mSearchNextPoleBegun) > mSearchNextPoleTimeout) {
#ifndef USE_CONST_POSITIONS
		seekTarget();
#endif
		return PREPARE_KICK;
	}
	return SEARCH_NEXT_POLE;
}

void BehaviorStriker::seekTarget()
{
	double angle = 0.0;
	double length = (double)mKickTargetDistanceMax;
	/*vector<Object> goalPoles = getGoalPoles().getObjects();
	vector<Object>::const_iterator it;
	for( it = goalPoles.begin(); it != goalPoles.end(); ++it) {
		angle += (*it).lastVector.getAngle();
	}
	angle = angle / (double)(goalPoles.size());
	*/

	const vector<Vector> &goalPoles = getGoalPolesVectors().get();
	if( goalPoles.size() >= 2) {
		vector<Vector>::const_iterator it = goalPoles.begin();
		Vector vec1 = (*it);
		++it;
		Vector vec2 = (*it);
		angle = (vec1.getAngle() + vec2.getAngle()) / 2.0;
	}
	Vector target(angle, length);
	mTarget.x = (int16_t)target.getX();
	mTarget.y = (int16_t)target.getY();
	Debugger::DEBUG("Striker", "Calculated target: %.4f, %.1f (%d; %d)", angle, length, mTarget.x, mTarget.y);
}

/**
 * PrepareKick
 */
void BehaviorStriker::entryPrepareKick()
{
	//if( mSearchGoal) {
		//getHeadAction().setPanTilt(mLastBallPan, mLastBallTilt);
	//}
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_TRYING_TO_SCORE;
	getHeadAction().searchObject(Object::BALL, false);
	Debugger::announce("Prepare Kick");
}

int BehaviorStriker::processPrepareKick()
{
	Debugger::DEBUG("Striker", "State: PrepareKick");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	Debugger::INFO("Striker", "BallStatus: %d", status);
	switch(status) {
	case VisionStatus::OBJECT_SEARCHING:
		Debugger::DEBUG("Striker", "PrepareKick: Ball searching");
		getBodyAction().stop();
		return PREPARE_KICK;
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND:
		Debugger::INFO("Striker", "PrepareKick: Ball lost!");
		getBodyAction().stop();
		return SEARCH_BALL;
	default:
		int pan = getBodyStatus().getPan();
		int tilt = getBodyStatus().getTilt();

		if(pan > mKickLeftTurnAngle && pan < mKickRightTurnAngle) {
			if( getBallVector().getX() < mKickBallDistance
						&& getBallVector().getY() > -60
						&& getBallVector().getY() < +60) {
				mNumberOfKickTries = 0;
				return KICK_BALL;
			} else {
				int16_t fbStep, rlStep;
				calculateWalkSteps( pan, tilt, rlStep, fbStep);
				getBodyAction().setWalkerCommand(fbStep, 0, rlStep);
				Debugger::DEBUG("Striker", "PrepareKick: forward %d, rotating %d", fbStep, rlStep);
				//Debugger::INFO("Striker", "FOLLOW2 P:%.2f T:%.2f", pan, tilt);
			}
		} else {
			int16_t rlStep;
			int16_t yStep;
			calculateTurnWalkSteps( pan, rlStep,yStep);
			getBodyAction().setWalkerCommand(mTurningXMovement, yStep, rlStep);
			Debugger::DEBUG("Striker", "PrepareKick: rotating %d", rlStep);
		}
		return PREPARE_KICK;
	}
}

/**
 * KickBall
 */
void BehaviorStriker::entryKickBall()
{
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_TRYING_TO_SCORE;
	getHeadAction().searchObject(Object::BALL, false);
	Debugger::announce("Kick");
}

int BehaviorStriker::processKickBall()
{
	Debugger::DEBUG("Striker", "State: Kick");

	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	Debugger::INFO("Striker", "BallStatus: %d", status);
	switch( status) {
	case VisionStatus::OBJECT_LOST:
	case VisionStatus::OBJECT_NOT_FOUND: //if( !getBall().lastImageSeen) {
		Debugger::INFO("Striker", "KickBall: Ball lost!");
		getBodyAction().stop();
		return SEARCH_BALL;
	case VisionStatus::OBJECT_SEARCHING:
		getBodyAction().stop();
		return KICK_BALL;
	default:
		break;
	}

	// TODO quick fix
	int pan = getBodyStatus().getPan();
	if(pan < 0) {
		Debugger::INFO("Striker", "Kick left!");
		getBodyAction().kickLeft();
	} else {
		Debugger::INFO("Striker", "Kick right!");
		getBodyAction().kickRight();
	}
	/*
#ifndef USE_CONST_POSITIONS
	mBall.x = (int16_t)getBallVector().getX();
	mBall.y = (int16_t)getBallVector().getY();
	if( mBall.x < 100) {
		mBall.x = 100;
	} else if( mBall.x > 150) {
		mBall.x = 150;
	}
	Debugger::INFO("Striker", "Kick to target !");
	Debugger::DEBUG("Striker", "Ball: %d,%d,%d; Target: %d,%d,%d",
			mBall.x, mBall.y, mBall.z, mTarget.x, mTarget.y, mTarget.z);
	getBodyAction().kickToTarget(mBall, mTarget);
#else
	int pan = getBodyStatus().getPan();
	if(pan < 0) {
		Debugger::INFO("Striker", "Kick left");
		mBall.y = -70;
		mTarget.y = -70;
		getBodyAction().kickToTarget(mBall, mTarget);
	} else {
		Debugger::INFO("Striker", "Kick right");
		mBall.y = 70;
		mTarget.y = 70;
		getBodyAction().kickToTarget(mBall, mTarget);
	}
#endif
*/
	mNumberOfKickTries++;
	return CHECK_KICK;
}

/**
 * CheckKick
 */
void BehaviorStriker::entryCheckKick()
{
	mNumberOfKickTries = 0;
	mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_TRYING_TO_SCORE;
	getHeadAction().searchObject(Object::BALL, false);
	//Debugger::announce("Check Kick");
}

int BehaviorStriker::processCheckKick()
{
	Debugger::DEBUG("Striker", "State: CheckKick");

	getBodyAction().stop();
	bool ballHasMoved = false;
	VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
	if( status == VisionStatus::OBJECT_SEEN
			|| status == VisionStatus::OBJECT_FOUND_STABLE) {
		int dx = (int)getBallVector().getX() - (int)mBall.x;
		int dy = (int)getBallVector().getY() - (int)mBall.y;
		ballHasMoved = ((dx*dx) + (dy*dy) > mKickBallMovedRangeMin);
	} else {
		ballHasMoved = true;
	}

	if( ballHasMoved) {
		// if ball is moved, cancel kick
		Debugger::DEBUG("Striker", "CheckKick: Ball has moved => Cancel kick");
		getBodyAction().cancelKick();
		mIsKickOff = false;
		mNumberOfKicks++;
		mNumberOfKickTries = 0; // reset
		return SEARCH_BALL;
	} else {
		uint64_t currentTime = getCurrentTime();
		if (currentTime > (getKickStatus().getKickingTime() + mKickingTryTimeout)) {
			if( mNumberOfKickTries > mNumberOfKickTriesMax) {
				mNumberOfKickTries = 0; // reset
				return SEARCH_BALL;
			} else { // try kick again
				mKickBallDistance -= 5;
				Debugger::WARN("Striker", "CheckKick: Ball not hit => reduced distance: %d", mKickBallDistance);
				return KICK_BALL;
			}
		}
	}
	return CHECK_KICK;
}

string BehaviorStriker::getStateName(int state){
	switch(state){
	case SEARCH_BALL: return "SEARCH_BALL";
	case DELAY_LOOP: return "DELAY_LOOP";
	case MOVE_TO_BALL: return "MOVE_TO_BALL";
	case CIRCLE_TO_GOAL: return "CIRCLE_TO_GOAL";
	case POSITION_NEAR_BALL: return "POSITION_NEAR_BALL";
	case SEARCH_NEXT_POLE: return "SEARCH_NEXT_POLE";
	case PREPARE_KICK: return "PREPARE_KICK";
	case KICK_BALL: return "KICK_BALL";
	case CHECK_KICK: return "CHECK_KICK";
	case CIRCLE_HALF: return "CIRCLE_HALF";
	case SCAN_FIELD: return "SCAN_FIELD";
	case RUN_TO_FRONT: return "RUN_TO_FRONT";
	default: return AbstractBehavior::getStateName(state);
	}
}

void BehaviorStriker::entryMoveHalf() {
	getHeadAction().searchObject(Object::UNKNOWN, true);
}

int BehaviorStriker::processMoveHalf() {
	getBodyAction().circleRight();
	initDelayLoop(mCircleHalfDuration,PREPARE_KICK);
	return DELAY_LOOP;
}


void BehaviorStriker::entryScanField() {
	mEntryScanFieldBegun = getCurrentTime();
	getHeadAction().searchObject(Object::UNKNOWN, true);
}

int BehaviorStriker::processScanField() {
	getBodyAction().stop();
	uint64_t currentTime = getCurrentTime();
	if( (currentTime - mEntryScanFieldBegun) > SCAN_FIELD_TIMEOUT) {
		return SEARCH_BALL;
	}
	return SCAN_FIELD;
}

void BehaviorStriker::entryRunToFront(){


}

int BehaviorStriker::processRunToFront() {
	getBodyAction().setWalkerCommand(100,0,0);
	initDelayLoop(mRunToFront,SEARCH_BALL);
	return DELAY_LOOP;
}

