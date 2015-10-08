/*
 * BehaviorDefender.h
 *
 *  Created on: 27.03.2014
 *      Author: Oliver Krebs
 */

#ifndef BEHAVIORDEFENDER_H_
#define BEHAVIORDEFENDER_H_

#include "AbstractBehavior.h"

class BehaviorDefender
:	public AbstractBehavior,
	public ConfigChangeListener
{
public:
	BehaviorDefender(Game *game, MitecomCommunicator *communicator);
	virtual ~BehaviorDefender();

	virtual void configChanged();
	virtual string getStateName(int state);

protected:
	virtual void executeBehavior();
	virtual void updateState();

	enum {
		SEARCH_BALL,
		MOVE_NEAR_BALL,
		PREPARE_KICK,
		KICK_BALL
	};

	void entrySearchBall();
	int processSearchBall();

	void entryMoveNearBall();
	int processMoveNearBall();

	void entryPrepareKick();
	int processPrepareKick();

	void entryKickBall();
	int processKickBall();

	int16_t mTurningXMovement;
	int mTowardsBallLeftTurnAngle;
	int mTowardsBallRightTurnAngle;
	int mTargetBallDistance;
	int mPrepareKickBallDistance;
	int mKickBallDistance;
	int mKickLeftTurnAngle;
	int mKickRightTurnAngle;
	bool mAllowKick;
};

#endif /* BEHAVIORDEFENDER_H_ */
