/*
 * AbstractBehavior.h
 *
 *  Created on: 26.03.2014
 *      Author: Oliver Krebs
 */

#ifndef ABSTRACTBEHAVIOR_H_
#define ABSTRACTBEHAVIOR_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/Vector.h"
#include "../../Representations/STLVector.h"
#include "../../Representations/Object.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/RobotConfiguration.h"
#include "../../Representations/BodyStatus.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/BehaviorStatus.h"
#include "../../Representations/HeadAction.h"
#include "../../Representations/BodyAction.h"
#include "../../Representations/GameStatus.h"
#include "../../Representations/GoalSide.h"
#include "../../Representations/VisionStatus.h"
#include "../../Representations/KickStatus.h"
#include "../../Representations/TeamMate.h"
#include "../../Representations/FieldLines.h"
#include "../../Communication/MitecomCommunicator.h"
#include "../../Network/Network.h"
#include "../../GameEventListener.h"
#include "../../Game.h"

BEGIN_DECLARE_MODULE(AbstractBehavior)
	REQUIRE(RobotConfiguration,RobotConfiguration)
	REQUIRE(BodyStatus,BodyStatus)
	REQUIRE(YUVImage,Image)
	REQUIRE(FieldLines, FieldLines)
	REQUIRE(Vector, BallVector)
	//REQUIRE(Vector, BallPredictVector)
	REQUIRE(STLVector<Vector>, GoalPolesVectors)
	REQUIRE(GameStatus,GameStatus)
	//REQUIRE(GoalSide,GoalSidePrediction)
	REQUIRE(VisionStatus, BallStatus)
	REQUIRE(VisionStatus, GoalStatus)
	REQUIRE(KickStatus, KickStatus)
	REQUIRE(ObjectList, GoalPoles)
	//-----------------------------------------------
	PROVIDE(BehaviorStatus,BehaviorStatus)
	PROVIDE(HeadAction,HeadAction)
	PROVIDE(BodyAction,BodyAction)
	PROVIDE(TeamMate,Goalie)
END_DECLARE_MODULE(AbstractBehavior)

/**
 * An abstract behavior, every role behavior should inherit from this class
 */
class AbstractBehavior
:	public AbstractBehaviorBase,
	public GameEventListener
{
public:

	/**
	 * Constructor
	 */
	AbstractBehavior(Game *game, MitecomCommunicator *communicator);

	/**
	 * Destructor
	 */
	virtual ~AbstractBehavior();

	/**
	 * execute is called every cycle
	 * @see Module::execute
	 */
	virtual bool execute();

	/**
	 * call when penalized state has changed
	 * @param penalized		the new state, true if robot is penalized
	 */
	virtual void isPenalizedHasChanged(bool penalized);

	/**
	 * call when game result has changed
	 * @param result		the new game result
	 */
	virtual void gameResultHasChanged(Game::GameResult result);

	/**
	 * call when game state has changed
	 * @param state			the new game state
	 */
	virtual void gameStateHasChanged(Game::GameState state);

	/**
	 * call when kick-off has changed
	 * @param haveKickOff	true if our team has kick-off
	 */
	virtual void kickOffHasChanged(bool haveKickOff);

	/**
	 * get the name of the given state
	 * @param state		index of state
	 * @return name of the given state
	 */
	virtual string getStateName(int state);

protected:

	/**
	 * update the state, check if state has changed and entry the new one
	 */
	virtual void updateState() = 0;

	/**
	 * execute the behavior, that is implemented in the specific role behavior
	 */
	virtual void executeBehavior() = 0;

	/**
	 * change the role
	 * @param role		index of new role
	 */
	static void changeRole( int role);

	/**
	 * change the role
	 * @param role		name of new role
	 */
	static void changeRole(string role);

	/**
	 * update team mates
	 */
	void updateTeamMates();

	/**
	 * update communicator
	 */
	void updateCommunicator();

	/**
	 * states of AbstractBehavior
	 */
	enum {
		DELAY_LOOP = -1		//!< delay loop
	};

	/**
	 * initialize delay loop
	 * @param delay_ms		delay of loop in ms
	 */
	void initDelayLoop(uint64_t delay_ms);

	/**
	 * initialize delay loop
	 * @param delay_ms		delay of loop in ms
	 * @param goToState		index of state after delay loop
	 */
	void initDelayLoop(uint64_t delay_ms, int goToState);

	/**
	 * process delay loop
	 */
	int processDelayLoop() const;

	/**
	 * calculate walk steps
	 * @param pan		current pan angle
	 * @param tilt		current tilt angle
	 * @param rlStep	step for right/left (output)
	 * @param fbStep	step for front/back (output)
	 */
	void calculateWalkSteps( int pan, int tilt, int16_t &rlStep, int16_t &fbStep) const;

	/**
	 * calculate fit walk steps
	 * @param pan		current pan angle
	 * @param rlStep	step for right/left (output)
	 * @param fbStep	step for front/back (output)
	 */
	void calculateFitWalkSteps( int pan, int16_t &rlStep, int16_t &fbStep) const;

	/**
	 * calculate turn walk steps
	 * @param pan		current pan angle
	 * @param rlStep	step for right/left (output)
	 * @param yStep		step for turning (output)
	 */
	void calculateTurnWalkSteps( int pan, int16_t &rlStep, int16_t &yStep) const;

	static int32_t mCurrentRole;	//!< current role
	int32_t mThisRole;				//!< this role
	bool mIsPenalized;				//!< robot is penalized
	bool mIsKickOff;				//!< own team has kick-off
	bool mHaveGoalie;				//!< own team has goal-keeper
	bool mUseMitecom;				//!< use Mitecom and communicate with team mates
	bool mDynamicRoleChange;		//!< dynamic role change activated

	int mWalkingSpeedAvg;			//!< average walking speed
	int mKickingDistanceMax;		//!< maximal kick distance

	int mLastState;					//!< last state
	int mCurrentState;				//!< current state
	int mReturnState;				//!< return state
	uint64_t mDelayLoopDuration;	//!< delay loop duration
	uint64_t mLoopStartTime;		//!< loop start time
	uint64_t mLastRoleDecideTime;	//!< last role decide time

	std::string mRoleWhenLosingGame;//!< role when losing game

	MixedTeamMates mTeamMates;		//!< team mates
	MixedTeamMate mMyInfo;			//!< my published informations
	Game *mGame;					//!< the game
	MitecomCommunicator *mCommunicator;	//!< communicator

	int mMinWalkingTurnAngle;		//!< minimal walking turn angle
	int mMinTurnAngle;				//!< minimal turning angle
	int mYTurnAngle;				//!< y turning angle
	int mLeftTurnSpeed;				//!< left turn speed
	int mRightTurnSpeed;			//!< right turn speed
};

#endif /* ABSTRACTBEHAVIOR_H_ */
