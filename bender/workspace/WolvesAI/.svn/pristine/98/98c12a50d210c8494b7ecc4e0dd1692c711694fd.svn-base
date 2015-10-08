/*
 * BodyAction.cpp
 *
 *  Created on: 26.11.2012
 *      Author: Stefan Krupop
 */

#include "BodyAction.h"
#include "../Config.h"
#include "../Messages/BodyAction.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"

BodyAction::BodyAction() {
	mWalkerX = 0;
	mWalkerY = 0;
	mWalkerR = 0;
	mWalkerTurningAim = false;
	mMotion = ICommunicator::Stop;
	mWalkerCommand = false;

	mBall.x = 0;
	mBall.y = 0;
	mBall.z = 0;
	mTarget.x = 0;
	mTarget.y = 0;
	mTarget.z = 0;
	mKicktype = 0;

	mMovementChangeID = 0;
}

BodyAction::~BodyAction() {
}

void BodyAction::setWalkerCommand(int16_t x, int16_t y, int16_t r) {
	setWalkerCommand(x, y, r, false);
}

void BodyAction::setWalkerCommand(int16_t x, int16_t y, int16_t r, bool turningAim) {
	if (x > 100) {
		x = 100;
	} else if (x < -100) {
		x = -100;
	}
	if (y > 100) {
		y = 100;
	} else if (y < -100) {
		y = -100;
	}
	if (r > 100) {
		r = 100;
	} else if (r < -100) {
		r = -100;
	}

	mWalkerX = (int8_t)x;
	mWalkerY = (int8_t)y;
	mWalkerR = (int8_t)r;
	mWalkerTurningAim = turningAim;
	mWalkerCommand = true;
	mMovementChangeID++;
}

void BodyAction::stop() {
	mMotion = ICommunicator::Stop;
	mWalkerCommand = false;
	mMovementChangeID++;
}

void BodyAction::kickLeft() {
	mMotion = ICommunicator::KickLeft;
	mWalkerCommand = false;
	mMovementChangeID++;
}

void BodyAction::kickRight() {
	mMotion = ICommunicator::KickRight;
	mWalkerCommand = false;
	mMovementChangeID++;
}

void BodyAction::kickBall(const vect3d_t& ball) {
	mMotion = ICommunicator::KickBall;
	mBall = ball;
	mWalkerCommand = false;
	mMovementChangeID++;
}

void BodyAction::kickToTarget(const vect3d_t& ball, const vect3d_t& target) {
	mMotion = ICommunicator::KickToTarget;
	mBall = ball;
	mTarget = target;
	mWalkerCommand = false;
	mMovementChangeID++;
}

void BodyAction::kickAdvanced(const vect3d_t& ball, const vect3d_t& target, uint8_t kicktype) {
	mMotion = ICommunicator::KickAdvanced;
	mBall = ball;
	mTarget = target;
	mKicktype = kicktype;
	mWalkerCommand = false;
	mMovementChangeID++;
}

void BodyAction::cancelKick() {
	mMotion = ICommunicator::CancelKick;
	mWalkerCommand = false;
	mMovementChangeID++;
}

void BodyAction::goaliMoveLeft() {
	mMotion = ICommunicator::GoaliMoveLeft;
	mWalkerCommand = false;
	mMovementChangeID++;
}

void BodyAction::goaliMoveRight() {
	mMotion = ICommunicator::GoaliMoveRight;
	mWalkerCommand = false;
	mMovementChangeID++;
}

void BodyAction::playMotion( enum ICommunicator::Motion motion) {
	mMotion = motion;
	mWalkerCommand = false;
	mMovementChangeID++;
}

void BodyAction::circleRight(){
	int16_t x = Config::GetValue<int16_t>("Body", "circleRightX", -40);
	int16_t y = Config::GetValue<int16_t>("Body", "circleRightY", 50);
	int16_t r = Config::GetValue<int16_t>("Body", "circleRightR", 70);
	bool turningAim = Config::GetBoolean("Body", "circleRightTurningAim", false);
	setWalkerCommand(x, y, r, turningAim);
	Debugger::INFO("Body", "Circling right");
}

void BodyAction::circleLeft(){
	int16_t x = Config::GetValue<int16_t>("Body", "circleLeftX", -40);
	int16_t y = Config::GetValue<int16_t>("Body", "circleLeftY", -70);//Bot4: -65
	int16_t r = Config::GetValue<int16_t>("Body", "circleLeftR", -50);
	bool turningAim = Config::GetBoolean("Body", "circleLeftTurningAim", false);
	setWalkerCommand(x, y, r, turningAim);
	Debugger::INFO("Body", "Circling left");
}

void Serializer<BodyAction>::serialize(const BodyAction& representation, ostream& stream) {
	protobuf::BodyAction action;

	action.set_walkercommand(representation.getIsWalkerCommand());
	action.set_walkerx(representation.getWalkerX());
	action.set_walkery(representation.getWalkerY());
	action.set_walkerr(representation.getWalkerR());
	action.set_walkerturningaim(representation.getWalkerTurningAim());
	action.set_motion((protobuf::BodyAction_StaticMotion)representation.getMotion());
	protobuf::Point3D* ball = action.mutable_ball();
	ball->set_x(representation.getPositionBall().x);
	ball->set_y(representation.getPositionBall().y);
	ball->set_z(representation.getPositionBall().z);
	protobuf::Point3D* target = action.mutable_target();
	target->set_x(representation.getPositionTarget().x);
	target->set_y(representation.getPositionTarget().y);
	target->set_z(representation.getPositionTarget().z);

	OstreamOutputStream buf(&stream);
	action.SerializeToZeroCopyStream(&buf);
}

void Serializer<BodyAction>::deserialize(istream& stream, BodyAction& representation) {
	protobuf::BodyAction action;

	IstreamInputStream buf(&stream);
	action.ParseFromZeroCopyStream(&buf);

	if (action.walkercommand()) {
		representation.setWalkerCommand((int16_t)action.walkerx(), (int16_t)action.walkery(), (int16_t)action.walkerr(), action.walkerturningaim());
	}
}
