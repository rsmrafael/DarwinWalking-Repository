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

class RemoteBehavior : public RemoteBehaviorBase {
public:
	enum eCommand {
		CMD_NULL,
		CMD_MOTION,
		CMD_WALK,
		CMD_PAN_TILT,
		CMD_KICK,
		CMD_KICK_TARGET,
		CMD_KICK_ADVANCED
	};

	RemoteBehavior();
	virtual ~RemoteBehavior();

	virtual bool execute();

	bool IsActive() const;
	void SetBodyCommand(string cmd);
	void SetPanTiltPosition(int16_t pan, int16_t tilt);
	void SetWalkerCommand(int8_t x, int8_t y, int8_t r, bool turningAim);
	void SetActive(bool state);
	void SetKickCommand(const vect3d_t &ball);
	void SetKickCommand(const vect3d_t &ball, const vect3d_t &target);
	void SetKickCommand(const vect3d_t &ball, const vect3d_t &target, uint8_t kicktype);

private:
	string mBodyCommand;
	vect3d_t mBall;
	vect3d_t mTarget;
	uint8_t mKicktype;
	bool mActive;
	int16_t mPanPosition;
	int16_t mTiltPosition;
	int8_t mWalkerX;
	int8_t mWalkerY;
	int8_t mWalkerR;
	bool mWalkerTurningAim;
	eCommand mCommand;
};

#endif /* REMOTEBEHAVIOR_H_ */
