/*
 * BehaviorGoalie.h
 *
 *  Created on: 26.03.2014
 *      Author: Oliver Krebs
 */

#ifndef BEHAVIORGOALIE_H_
#define BEHAVIORGOALIE_H_

#include "AbstractBehavior.h"

class BehaviorGoalie
:	public AbstractBehavior,
	public ConfigChangeListener
{
public:
	BehaviorGoalie(Game *game, MitecomCommunicator *communicator);
	virtual ~BehaviorGoalie();

	virtual void configChanged();

	virtual string getStateName(int state);

protected:
	virtual void executeBehavior();
	virtual void updateState();

	enum {
		SEARCH_BALL,
		TRACK_BALL,
		SAVE_BALL,
		LOCALIZE_AND_POSITION,
		PREPARE_KICK,
		KICK_BALL
	};

	void entrySearchBall();
	int processSearchBall();

	void entryTrackBall();
	int processTrackBall();

	void entrySaveBall();
	int processSaveBall();

	void entryLocalizeAndPosition();
	int processLocalizeAndPosition();

	void entryPrepareKick();
	int processPrepareKick();

	void entryKickBall();
	int processKickBall();

	int16_t mTurningXMovement;
	int mReactTilt;
	double mReactDistance;
	int mPrepareKickBallDistance;
	int mKickBallDistance;
	int mKickLeftTurnAngle;
	int mKickRightTurnAngle;
	bool mAllowKick;
	std::string mReactRole;
};

#endif /* BEHAVIORGOALIE_H_ */
