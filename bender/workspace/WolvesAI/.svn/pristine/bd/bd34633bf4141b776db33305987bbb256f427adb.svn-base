/*
 * BehaviorSupporter.h
 *
 *  Created on: 27.03.2014
 *      Author: Oliver Krebs
 */

#ifndef BEHAVIORSUPPORTER_H_
#define BEHAVIORSUPPORTER_H_

#include "AbstractBehavior.h"

/**
 * Supporter is the role who should be the second nearest to the ball
 * and supports the striker
 */
class BehaviorSupporter
:	public AbstractBehavior,
	public ConfigChangeListener
{
public:
	/**
	 * Constructor
	 * @param game			information of the game
	 * @param communicator	mitecom communicator
	 */
	BehaviorSupporter(Game *game, MitecomCommunicator *communicator);

	/**
	 * Destructor
	 */
	virtual ~BehaviorSupporter();

	/**
	 * called when config has changed
	 * @see ConfigChangeListener::configChanged
	 */
	virtual void configChanged();

	/**
	 * get the name of the given state
	 * @param state		index of state
	 * @return name of the given state
	 */
	virtual string getStateName(int state);

protected:
	virtual void executeBehavior();		//!< execute supporter
	virtual void updateState();			//!< update state

	/**
	 * Actions for supporter
	 */
	enum {
		SEARCH_BALL,	//!< search ball
		MOVE_NEAR_BALL,	//!< move near the ball position
		PREPARE_KICK,	//!< prepare kick
		KICK_BALL		//!< kick ball
	};

	void entrySearchBall();			//!< entry state SEARCH_BALL
	int processSearchBall();		//!< process state SEARCH_BALL

	void entryMoveNearBall();		//!< entry state MOVE_NEAR_BALL
	int processMoveNearBall();		//!< process state MOVE_NEAR_BALL

	void entryPrepareKick();		//!< entry state PREPARE_KICK
	int processPrepareKick();		//!< process state PREPARE_KICK

	void entryKickBall();			//!< entry state KICK_BALL
	int processKickBall();			//!< process state KICK_BALL

	int16_t mTurningXMovement;		//!< turning x movement
	int mTowardsBallLeftTurnAngle;	//!< turn angle for moving towards ball left
	int mTowardsBallRightTurnAngle;	//!< turn angle for moving towards ball right
	int mTargetBallDistance;		//!< target ball distance
	int mPrepareKickBallDistance;	//!< prepare kick ball distance
	int mKickBallDistance;			//!< kick ball distance
	int mKickLeftTurnAngle;			//!< turn angle kick left
	int mKickRightTurnAngle;		//!< turn angle kick right
	bool mAllowKick;				//!< allow kick (yes=true)
};

#endif /* BEHAVIORSUPPORTER_H_ */
