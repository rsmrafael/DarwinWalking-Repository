/*
 * RemoteBehavior.h
 *
 *  Created on: 01.12.2012
 *      Author: Stefan
 */

#ifndef REMOTEBEHAVIOR_H_
#define REMOTEBEHAVIOR_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/HeadAction.h"
#include "../../Representations/BodyAction.h"
#include "../../Representations/BehaviorStatus.h"
#include <string>

BEGIN_DECLARE_MODULE(RemoteBehavior)
	PROVIDE(HeadAction,HeadAction)
	PROVIDE(BodyAction,BodyAction)
	PROVIDE(BehaviorStatus,BehaviorStatus)
END_DECLARE_MODULE(RemoteBehavior)

/**
 * RemoteBehavior is the behavior for remote control
 */
class RemoteBehavior : public RemoteBehaviorBase {
public:
	/**
	 * Remote control commands
	 */
	enum eCommand {
		CMD_NULL,				//!<
		CMD_MOTION,				//!<
		CMD_WALK,				//!<
		CMD_PAN_TILT,			//!<
		CMD_KICK,				//!<
		CMD_KICK_TARGET,		//!<
		CMD_KICK_ADVANCED		//!<
	};

	/**
	 * Constructor
	 */
	RemoteBehavior();

	/**
	 * Destructor
	 */
	virtual ~RemoteBehavior();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

	/**
	 * check if active
	 * @return true if it is active
	 */
	bool IsActive() const;

	/**
	 * set a body command
	 * @param cmd 		command name
	 */
	void SetBodyCommand(string cmd);

	/**
	 * set pan and tilt position
	 * @param pan		pan angle
	 * @param tilt		tilt angle
	 */
	void SetPanTiltPosition(int16_t pan, int16_t tilt);

	/**
	 * set a walk command
	 * @param x			walk in x (forward/backward)
	 * @param y			walk in y (right/left)
	 * @param r			walk rotation
	 * @param turningAim	turningAim
	 */
	void SetWalkerCommand(int8_t x, int8_t y, int8_t r, bool turningAim);

	/**
	 * set active
	 * @param state		true if active
	 */
	void SetActive(bool state);

	/**
	 * set kick command
	 * @param ball		relative ball position
	 */
	void SetKickCommand(const vect3d_t &ball);

	/**
	 * set kick to target command
	 * @param ball		relative ball position
	 * @param target	relative target position
	 */
	void SetKickCommand(const vect3d_t &ball, const vect3d_t &target);

	/**
	 * set advanced kick command
	 * @param ball		relative ball position
	 * @param target	relative target position
	 * @param kicktype	type of kick
	 */
	void SetKickCommand(const vect3d_t &ball, const vect3d_t &target, uint8_t kicktype);

private:
	eCommand mCommand;			//!< command
	string mBodyCommand;		//!< body command
	vect3d_t mBall;				//!< ball position
	vect3d_t mTarget;			//!< target position
	uint8_t mKicktype;			//!< type of kick
	int16_t mPanPosition;		//!< pan position
	int16_t mTiltPosition;		//!< tilt position
	int8_t mWalkerX;			//!< walk x
	int8_t mWalkerY;			//!< walk y
	int8_t mWalkerR;			//!< walk rotation
	bool mWalkerTurningAim;		//!< walk turning aim
	bool mActive;				//!< is active
};

#endif /* REMOTEBEHAVIOR_H_ */
