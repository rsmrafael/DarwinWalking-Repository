/*
 * BehaviorGoalie.h
 *
 *  Created on: 26.03.2014
 *      Author: Oliver Krebs
 */

#ifndef BEHAVIORGOALIE_H_
#define BEHAVIORGOALIE_H_

#include "AbstractBehavior.h"

/**
 * Behavior for a goal keeper, who waits in the goal area for the ball
 */
class BehaviorGoalie
:	public AbstractBehavior,
	public ConfigChangeListener
{
public:
	/**
	 * Constructor
	 * @param game			information of the game
	 * @param communicator	mitecom communicator
	 */
	BehaviorGoalie(Game *game, MitecomCommunicator *communicator);

	/**
	 * Destructor
	 */
	virtual ~BehaviorGoalie();

	/**
	 * config has changed
	 * @see ConfigChangeListener::configChanged
	 */
	virtual void configChanged();

	/**
	 * get state name
	 * @param state 	index of state
	 * @return name of state
	 */
	virtual string getStateName(int state);

protected:
	virtual void executeBehavior();		//!< execute behavior
	virtual void updateState();			//!< update state

	/**
	 * Actions for goalie
	 */
	enum {
		SEARCH_BALL,			//!< search ball
		TRACK_BALL,				//!< track ball
		SAVE_BALL,				//!< save ball
		LOCALIZE_AND_POSITION,	//!< localize and position
		PREPARE_KICK,			//!< prepare kick
		KICK_BALL				//!< kick ball
	};

	void entrySearchBall();				//!< entry state SEARCH_BALL
	int processSearchBall();			//!< process state SEARCH_BALL

	void entryTrackBall();				//!< entry state TRACK_BALL
	int processTrackBall();				//!< process state TRACK_BALL

	void entrySaveBall();				//!< entry state SAVE_BALL
	int processSaveBall();				//!< process state SAVE_BALL

	void entryLocalizeAndPosition();	//!< entry state LOCALIZE_AND_POSITION
	int processLocalizeAndPosition();	//!< process state LOCALIZE_AND_POSITION

	void entryPrepareKick();			//!< entry state PREPARE_KICK
	int processPrepareKick();			//!< process state PREPARE_KICK

	void entryKickBall();				//!< entry state KICK_BALL
	int processKickBall();				//!< process state KICK_BALL

	int16_t mTurningXMovement;		//!< turning x movement
	int mReactTilt;					//!< react if tilt angle is smaller
	double mReactDistance;			//!< react if the distance to ball is less
	int mPrepareKickBallDistance;	//!< distance for prepare kick ball
	int mKickBallDistance;			//!< distance for kick ball
	int mKickLeftTurnAngle;			//!< turn angle for kick left
	int mKickRightTurnAngle;		//!< turn angle for kick right
	bool mAllowKick;				//!< allow kick (yes=true)
	std::string mReactRole;			//!< role for reacting
};

#endif /* BEHAVIORGOALIE_H_ */
