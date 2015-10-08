/*
 * Body.cpp
 *
 *  Created on: 17.06.2009
 *      Author: Stefan
 */

#include "Body.h"
#include "../Debugging/Debugger.h"
#include "BodyCommunicator.h"
#include "SimulationCommunicator.h"
#include "../Utils/Time.h"

/**
 * Constructor taking two Arguments. Stops the body and wait until the Inital Position Reached.(Only at Start)
 * @param *comm is the Communicator used by Body, so Real Robot or Simulator.
 * @param debugEnabled is for choose if you want debug messages(true) or not(false).
 */
Body::Body(ICommunicator *comm, bool debugEnabled) {
	mParalysis = false;
	mDebugEnabled = debugEnabled;
	mKickingTime = 0;
	mComm = comm;
	mPanTilt = new PanTiltUnit(mComm);

	Stop();
	// Wait till initial position reached
	#ifdef WIN32
		Sleep(1000);
	#else
		usleep(1000 * 1000);
	#endif
}
/**
 * Constructor taking three arguments. Stops the body and wait until the Inital Position Reached.(Only at Start)
 * @param device only needed for BodyCommunicator and not for SimulationCommunicator, is used to identifie the Device for debug
 * @param type is a Bodytype use for choose if the mComm is a Simmulator or a real Robot.
 * @param noComms is only needed for BodyCommunicator and is for disable communication to the robot (used mostly to Debug on Dev-Pcs)
 */
Body::Body(string device, BodyType type, bool noComms) {
	mParalysis = false;
	mDebugEnabled = true;
	mKickingTime = 0;

	if (type == Body::Simulator) {
		mComm = SimulationCommunicator::GetInstance();
	} else {
		mComm = new BodyCommunicator(device, noComms);
	}
	mPanTilt = new PanTiltUnit(mComm);

	Stop();

	// Wait till initial position reached
#ifdef WIN32
	Sleep(1000);
#else
	usleep(1000 * 1000);
#endif
}

Body::~Body() {
	delete mPanTilt;
	delete mComm;
}
/**
 * Get the PanTiltUnit.
 * @return Returns a reference for the PanTiltUnit. PanTiltUnit Controlls all Pantilt actions and Corrections.
 */
PanTiltUnit* Body::PanTilt() {
	return mPanTilt;
}
/**
 * Get the mParalysis boolean.
 * @return Returns if the Robot is allowed to move.
 */
bool Body::isParalysed() const {
	return mParalysis;
}
/**
 * Set the mParalysis boolean to a new value to determine if the Roboter should not move. Stops the Robot if mParalysis changed from false to true.
 * @param value determine if the robot is allowed to move (true) or not(false).
 */
void Body::setParalysis(bool value) {
	if (value && !mParalysis) {
		Stop();
	}
	mParalysis = value;
}

/**
 * Returns , if the statusmessage is avaidable, the BatteryVoltage(double) else return 0.0. The information come from the StatusMessage.
 * @return The Voltage of the Battery if avaidable or 0.0.
 */
double Body::GetBatteryVoltage() {
	const struct ICommunicator::statusMessage_t* status = mComm->GetStatusMessage();
	if (status != NULL) {
		return ((double)status->voltage) / 100.0;
	}
	return 0.0;
}
/**
 * Read the IMU data out of the last Statusmessage and return them.
 * @param pitch the variable reference with is filled with pitch value from last message. If no message avaidable 0 is insertet.
 * @param roll the variable reference with is filled with roll value from last message. If no message avaidable 0 is insertet.
 */
void Body::GetIMUData(int* pitch, int* roll) {
	const struct ICommunicator::statusMessage_t* status = mComm->GetStatusMessage();
	if (status != NULL) {
		*pitch = (int)status->imu.fb_accel;
		*roll = (int)status->imu.rl_accel;
	} else {
		*pitch = 0;
		*roll = 0;
	}
}
/**
 * Get the Status Messsage from the Body (Many information a send repeatly from the Body in this Message)
 * @return A reference off the last Statusmessage.
 */
const struct ICommunicator::statusMessage_t* Body::GetStatusMessage() {
	return mComm->GetStatusMessage();
}
/**
 * Send to the Body a movement Command if not paralysed, the walker should use it to walk.
 * @param x is the forward movement off the roboter in Procent. Can be from -100 - +100.
 * @param y is the siteward movement off the roboter in Procent. Can be from -100 - +100.
 * @param r is the rotation movement off the roboter in Procent. Can be from -100 - +100.
 * @param turningAim is a boolean determining if the Roboter main turning. TODO Check if it is correct.
 */
void Body::setWalkerCommand(int8_t x, int8_t y, int8_t r, bool turningAim) {
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

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

/*	if (turningAim) {
		Debugger::INFO("Body", "Walker command: %d/%d/%d, turningAim on", x, y, r);
	} else {
		Debugger::INFO("Body", "Walker command: %d/%d/%d", x, y, r);
	}
*/
    mComm->WalkerCommand(x, y, r, turningAim);
}

/**
 * Send a Stop command to the Body over the mComm (Real or Simulation) with 1 repetitions.
 */
void Body::Stop(void) {
	if( mDebugEnabled) {
		Debugger::DEBUG("Body", "Stop");
	}
    mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
}

/**
 * Send a Cancel Kick Command to the Body over the mComm(Real or Simmulation) with 1 repetitions.
 */
void Body::CancelKick() {
	mComm->RepeatedMovementCommand(ICommunicator::CancelKick, true, 1);
}
/**
 * If he is not Paralysis (mParalysis), he send a KickLeft motion command to the body with strenght and direction(If the body intepret it other).
 */
void Body::KickLeft() {
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

	if( mDebugEnabled) {
		Debugger::INFO("Body", "KickLeft");
	}
	mComm->StrengthDirectionMovementCommand(ICommunicator::KickLeft, true, 127, 127);
	mKickingTime = getCurrentTime();
}

/**
 * If he is not Paralysis (mParalysis), he send a KickRight motion command to the body with strenght and direction(If the body intepret it other).
 */
void Body::KickRight() {
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

	if( mDebugEnabled) {
		Debugger::INFO("Body", "KickRight");
	}
    mComm->StrengthDirectionMovementCommand(ICommunicator::KickRight, true, 127, 127);
    mKickingTime = getCurrentTime();
}
/**
 * KickBall send if not paralysed(mParalysis), a message with a Kicker command to the Kickerengine.
 * @param ball is the position vector off the Ball.
 */
void Body::KickBall(const vect3d_t& ball) {
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

	if( mDebugEnabled) {
		Debugger::INFO("Body", "KickBall");
	}
    mComm->KickerCommand(ball);
    mKickingTime = getCurrentTime();
}
/**
 * KickToTarget send if not paralysed(mParalysis), a message with a Kicker command to the Kickerengine.
 * @param ball is the position vector off the Ball.
 * @param target is the shooting target where the ball should be shooted to.
 */
void Body::KickToTarget(const vect3d_t& ball, const vect3d_t& target) {
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

	if( mDebugEnabled) {
		Debugger::INFO("Body", "KickToTarget");
	}
    mComm->KickerCommand(ball, target);
    mKickingTime = getCurrentTime();
}

/**
 * KickAdvanced send if not paralysed(mParalysis), a message with a Kicker command to the Kickerengine.
 * @param ball is the position vector off the Ball.
 * @param target is the shooting target where the ball should be shooted to.
 * @param kicktype is a type off the Kick (Highkick or so)
 */
void Body::KickAdvanced(const vect3d_t& ball, const vect3d_t& target, uint8_t kicktype) {
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}

	if( mDebugEnabled) {
		Debugger::INFO("Body", "KickAdvanced");
	}
	mComm->KickerCommand(ball, target, kicktype);
	mKickingTime = getCurrentTime();
}
/**
 * Send a Goalie movement command for a motion to the Body with 1 repetitions if not paralysed(mParalysis).
 */
void Body::GoaliMoveRight(){
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}
	mComm->RepeatedMovementCommand(ICommunicator::GoaliMoveRight, true, 1);
}

/**
 * Send a Goalie movement command for a motion to the Body with 1 repetitions if not paralysed(mParalysis).
 */
void Body::GoaliMoveLeft(){
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}
	mComm->RepeatedMovementCommand(ICommunicator::GoaliMoveLeft, true, 1);
}

/**
 * Send a GoaHilie movement command for a motion to the Body with 1 repetitions if not paralysed(mParalysis).
 * (Should be one hand up winking)
 */
void Body::MoveHi(){
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}
	mComm->RepeatedMovementCommand(ICommunicator::MoveHi, true, 1);
}

/**
 * Send a YeahGoal movement command for a motion to the Body with 1 repetitions if not paralysed(mParalysis).
 * (Should be both hands up)
 */
void Body::YeahGoal(){
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}
	mComm->RepeatedMovementCommand(ICommunicator::YeahGoal, true, 1);
}
/**
 * Send a Motion movement command to the Body with 1 repetitions if not paralysed(mParalysis).
 */
void Body::PlayMotion( enum ICommunicator::Motion motion) {
	if (mParalysis) {
		mComm->RepeatedMovementCommand(ICommunicator::Stop, true, 1);
		return;
	}
	mComm->RepeatedMovementCommand(motion, true, 1);
}

/**
 * TODO Returns the TICKING TIME UNUSED?
 */
uint64_t Body::getKickingTime() const {
	return mKickingTime;
}
