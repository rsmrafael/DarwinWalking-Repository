/*
 * BehaviorSupporter.h
 *
 *  Created on: 27.03.2014
 *      Author: Oliver Krebs
 */

#ifndef BEHAVIORSUPPORTER_H_
#define BEHAVIORSUPPORTER_H_

#include "AbstractBehavior.h"

class BehaviorSupporter
:	public AbstractBehavior,
	public ConfigChangeListener
{
public:
	BehaviorSupporter(Game *game, MitecomCommunicator *communicator);
	virtual ~BehaviorSupporter();

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

#endif /* BEHAVIORSUPPORTER_H_ */
