/*
 * BehaviorDefender.h
 *
 *  Created on: 27.03.2014
 *      Author: Oliver Krebs
 */

#ifndef BEHAVIORDEFENDER_H_
#define BEHAVIORDEFENDER_H_

#include "AbstractBehavior.h"

/**
 * Defender is the role for a defending robot near the goal
 */
class BehaviorDefender
:	public AbstractBehavior,
	public ConfigChangeListener
{
public:

	/**
	 * Constructor
	 * @param game			information of the game
	 * @param communicator	mitecom communicator
	 */
	BehaviorDefender(Game *game, MitecomCommunicator *communicator);

	/**
	 * Destructor
	 */
	virtual ~BehaviorDefender();

	/**
	 * config has changed
	 * @see ConfigChangeListener::configChanged
	 */
	virtual void configChanged();

	/**
	 * get state name
	 * @param state		index of state
	 * @return name of state
	 */
	virtual string getStateName(int state);

protected:
	virtual void executeBehavior();		//!< execute defender behavior
	virtual void updateState();			//!< update the state

	/**
	 * Actions for the defender
	 */
	enum {
		SEARCH_BALL,		//!< search the ball
		MOVE_NEAR_BALL,		//!< move near to the ball
		PREPARE_KICK,		//!< prepare a kick
		KICK_BALL			//!< kick the ball
	};

	void entrySearchBall();		//!< entry state SEARCH_BALL
	int processSearchBall();	//!< process state SEARCH_BALL

	void entryMoveNearBall();	//!< entry state MOVE_NEAR_BALL
	int processMoveNearBall();	//!< process state MOVE_NEAR_BALL

	void entryPrepareKick();	//!< entry state PREPARE_KICK
	int processPrepareKick();	//!< process state PREPARE_KICK

	void entryKickBall();		//!< entry state KICK_BALL
	int processKickBall();		//!< process state KICK_BALL

	int16_t mTurningXMovement;			//!< turning x movement
	int mTowardsBallLeftTurnAngle;		//!< turn angle for moving towards ball left
	int mTowardsBallRightTurnAngle;		//!< turn angle for moving towards ball right
	int mTargetBallDistance;			//!< target distance to ball
	int mPrepareKickBallDistance;		//!< distance for prepare kick ball
	int mKickBallDistance;				//!< distance for kick ball
	int mKickLeftTurnAngle;				//!< turn angle for kick left
	int mKickRightTurnAngle;			//!< turn angle for kick right
	bool mAllowKick;					//!< allow kick (yes=true)
};

#endif /* BEHAVIORDEFENDER_H_ */
