/**
 * RemoteBehavior.cpp
 *
 *  Created on: 01.12.2012
 *  Author: Stefan
 */

#include "RemoteBehavior.h"
#include "../../Debugging/Debugger.h"
#include "../../Communication/MitecomData.h"

RemoteBehavior::RemoteBehavior() {
	mActive = false;
	mBodyCommand = "";
	mCommand = CMD_NULL;
	mWalkerX = 0;
	mWalkerY = 0;
	mWalkerR = 0;
	mWalkerTurningAim = false;
	mPanPosition = 0;
	mTiltPosition = 0;
	mBall.x = 0;
	mBall.y = 0;
	mBall.z = 0;
	mTarget.x = 0;
	mTarget.y = 0;
	mTarget.z = 0;
	mKicktype = 0;
}

RemoteBehavior::~RemoteBehavior() {
}

bool RemoteBehavior::execute() {
	if (IsActive()) {
		getBehaviorStatus().setActualRole(ROLE_REMOTE);
		switch( mCommand) {
		case CMD_MOTION:
			if (mBodyCommand == "stop") {
				getBodyAction().stop();
			} else if (mBodyCommand == "forward") {
				getBodyAction().setWalkerCommand(100, 0, 0);
			} else if (mBodyCommand == "turn_left") {
				getBodyAction().setWalkerCommand(0, 0, 100);
			} else if (mBodyCommand == "turn_right") {
				getBodyAction().setWalkerCommand(0, 0, -100);
			} else if (mBodyCommand == "kick_left") {
				getBodyAction().kickLeft();
			} else if (mBodyCommand == "kick_right") {
				getBodyAction().kickRight();
			} else if (mBodyCommand == "goalie_left") {
				getBodyAction().goaliMoveLeft();
			} else if (mBodyCommand == "goalie_right") {
				getBodyAction().goaliMoveRight();
			} else if(mBodyCommand == "circle_right") {
				getBodyAction().circleRight();
			} else if(mBodyCommand == "circle_left") {
				getBodyAction().circleLeft();
			} else if(mBodyCommand == "move_hi") {
				getBodyAction().playMotion( ICommunicator::MoveHi);
			} else if(mBodyCommand == "yeah_goal") {
				getBodyAction().playMotion( ICommunicator::YeahGoal);
			}
			mBodyCommand = "";
			break;
		case CMD_WALK:
			getBodyAction().setWalkerCommand((int16_t)mWalkerX, (int16_t)mWalkerY, (int16_t)mWalkerR, mWalkerTurningAim);
			break;
		case CMD_PAN_TILT:
			getHeadAction().setPanTilt(mPanPosition, mTiltPosition);
			break;
		case CMD_KICK:
			getBodyAction().kickBall(mBall);
			break;
		case CMD_KICK_TARGET:
			getBodyAction().kickToTarget(mBall, mTarget);
			break;
		case CMD_KICK_ADVANCED:
			getBodyAction().kickAdvanced(mBall, mTarget, mKicktype);
			break;
		case CMD_NULL:
		default:
			break;
		}
		mCommand = CMD_NULL;
		return true;
	}
	return false;
}

bool RemoteBehavior::IsActive() const {
	return mActive;
}

void RemoteBehavior::SetBodyCommand(string cmd) {
	mBodyCommand = cmd;
	mCommand = CMD_MOTION;
	Debugger::INFO("RemoteBehavior", "New body command: %s", cmd.c_str());
}

void RemoteBehavior::SetPanTiltPosition(int16_t pan, int16_t tilt) {
	mPanPosition = pan;
	mTiltPosition = tilt;
	mCommand = CMD_PAN_TILT;
	Debugger::INFO("RemoteBehavior", "New pan/tilt position: %d/%d", pan, tilt);
}

void RemoteBehavior::SetWalkerCommand(int8_t x, int8_t y, int8_t r, bool turningAim) {
	mWalkerX = x;
	mWalkerY = y;
	mWalkerR = r;
	mWalkerTurningAim = turningAim;
	mCommand = CMD_WALK;
	if (mWalkerTurningAim) {
		Debugger::INFO("RemoteBehavior", "New walker command: %d/%d/%d, turning aim on", x, y, r);
	} else {
		Debugger::INFO("RemoteBehavior", "New walker command: %d/%d/%d", x, y, r);
	}
}

void RemoteBehavior::SetActive(bool state) {
	mActive = state;
	if (state) {
		Debugger::INFO("RemoteBehavior", "Remote control enabled");
	} else {
		Debugger::INFO("RemoteBehavior", "Remote control disabled");
	}
}

void RemoteBehavior::SetKickCommand(const vect3d_t &ball) {
	mBall = ball;
	mCommand = CMD_KICK;
	Debugger::INFO("RemoteBehavior", "Kick ball: %d,%d,%d", mBall.x, mBall.y, mBall.z);
}

void RemoteBehavior::SetKickCommand(const vect3d_t &ball, const vect3d_t &target) {
	mBall = ball;
	mTarget = target;
	mCommand = CMD_KICK_TARGET;
	Debugger::INFO("RemoteBehavior", "Kick ball to target: %d,%d,%d - %d,%d,%d",
			mBall.x, mBall.y, mBall.z, mTarget.x, mTarget.y, mTarget.z);
}

void RemoteBehavior::SetKickCommand(const vect3d_t &ball, const vect3d_t &target, uint8_t kicktype) {
	mBall = ball;
	mTarget = target;
	mKicktype = kicktype;
	mCommand = CMD_KICK_ADVANCED;
	Debugger::INFO("RemoteBehavior", "Kick advanced: %d,%d,%d - %d,%d,%d (%d)",
			mBall.x, mBall.y, mBall.z, mTarget.x, mTarget.y, mTarget.z, mKicktype);
}
