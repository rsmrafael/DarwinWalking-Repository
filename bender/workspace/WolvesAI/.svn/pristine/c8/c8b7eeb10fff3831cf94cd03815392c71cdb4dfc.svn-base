/*
 * BehaviorStriker.h
 *
 *  Created on: 26.03.2014
 *      Author: Oliver Krebs
 */

#ifndef BEHAVIORSTRIKER_H_
#define BEHAVIORSTRIKER_H_

#include "AbstractBehavior.h"

class BehaviorStriker
:	public AbstractBehavior,
	public ConfigChangeListener
{
public:
	BehaviorStriker(Game *game, MitecomCommunicator *communicator);
	virtual ~BehaviorStriker();
	virtual string getStateName(int state);
	virtual void configChanged();

	void gameStateHasChanged(Game::GameState state);

protected:
	virtual void executeBehavior();
	virtual void updateState();

	void entrySearchBall();
	int processSearchBall();

	void entryMoveToBall();
	int processMoveToBall();

	void entryCircleToGoal();
	int processCircleToGoal();

	void entryPositionNearBall();
	int processPositionNearBall();

	void entrySearchNextPole();
	int processSearchNextPole();

	void entryPrepareKick();
	int processPrepareKick();

	void entryKickBall();
	int processKickBall();

	void entryCheckKick();
	int processCheckKick();

	void entryMoveHalf();
	int processMoveHalf();

	void entryScanField();
	int processScanField();

	void seekTarget();


	enum {
		SEARCH_BALL,
		MOVE_TO_BALL,
		CIRCLE_TO_GOAL,
		POSITION_NEAR_BALL,
		SEARCH_NEXT_POLE,
		PREPARE_KICK,
		KICK_BALL,
		CHECK_KICK,
		CIRCLE_HALF,
		SCAN_FIELD
	};

	int16_t mTurningXMovement;
	int mTowardsBallLeftTurnAngle;
	int mTowardsBallRightTurnAngle;
	int mNumberOfKicks;
	int mNumberOfKickTries;
	int mNumberOfKickTriesMax;
	int mKickBallMovedRangeMin;
	int mMoveToBallDistance;
	int mKickLeftTurnAngle;
	int mKickRightTurnAngle;

	vect3d_t mBall;
	vect3d_t mTarget;

	uint64_t mKickingTryTimeout;
	uint64_t mCircleToGoalEntryTime;
	uint64_t mCircleToGoalTimeout;
	uint64_t mMyGoalCircleDelay;
	uint64_t mPositionForKickTimeout;
	uint64_t mPositionForKickBegun;
	uint64_t mCircleHalfDuration;
	uint64_t mEntryScanFieldBegun;
	uint64_t mSearchNextPoleBegun;
	uint64_t mSearchNextPoleTimeout;

	int mPositionBallDistanceMin;
	int mPositionBallDistanceMax;
	int mKickBallDistance;
	int mKickTargetDistanceMax;

	bool mSearchGoal;
	bool mAllowKick;
	bool mCheckCorrectGoal;


};

#endif /* BEHAVIORSTRIKER_H_ */
