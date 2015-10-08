/*
 * Body.h
 *
 *  Created on: 17.06.2009
 *      Author: Stefan
 */

#ifndef BODY_H_
#define BODY_H_

using namespace std;

#include <string>
#include "ICommunicator.h"
#include "PanTiltUnit.h"

/*!
 * Body class is for the translation of basic commands into the right communication form for
 * the BodyCommunicator (Real Robot) or the SimulationCommunicator (Simulation).
 */
class Body {
public:
	/** BodyType.
	* The Possible Communicators for the commands of this class.
	*/
	enum BodyType {
		Regular = 0x00, 	//!< Regular means, it is the communication to the real robot
		Simulator = 0x01 	//!< Simulator is for communications with webots and other simulations
	};

	/**
	 * Constructor taking two Arguments. Stops the body and wait until the Inital Position Reached.(Only at Start)
	 * @param comm is the Communicator used by Body, so real robot or simulator.
	 * @param debugEnabled is for choose if you want debug messages(true) or not(false).
	 */
	Body(ICommunicator* comm, bool debugEnabled = true);

	/**
	 * Destructor
	 */
	~Body();

	/**
	 * Send a movement command to the body (forces the robot to walk if not paralyzed)
	 * @param x is the front/back movement of the robot in percent (-100..+100).
	 * @param y is the right/left movement of the robot in percent (-100..+100).
	 * @param r is the rotation movement of the robot in percent (-100..+100).
	 * @param turningAim if true, walker aims at a point while stepping to the side
	 */
	void setWalkerCommand(int8_t x, int8_t y, int8_t r, bool turningAim = false);

	/**
	 * Send a Stop command to the Body. The Body should stop walking.
	 */
	void Stop();

	/**
	 * Send a CancelKick command to the body (cancels kick)
	 */
	void CancelKick();

	/**
	 * @deprecated Send a KickLeft command to the Body.(Out of Date)
	 */
	void KickLeft();

	/**
	 * @deprecated Send a KickRight command to the Body.(Out of Date)
	 */
	void KickRight();

	/**
	 * Send a Kick command with the position of the Ball.
	 * @param ball relative position of ball
	 */
	void KickBall(const vect3d_t& ball);

	/**
	 * Send a Kick to the Kickengine with the Ball and the target Destination.
	 * @param ball relative position of ball
	 * @param target relative position of kick target
	 */
	void KickToTarget(const vect3d_t& ball, const vect3d_t& target);

	/**
	 * Send a Kick to the Kickengine with the Ball and the target Destination and add a Information the Kicktype.
	 * @param ball relative position of ball
	 * @param target relative position of kick target
	 * @param kicktype the type of kick
	 */
	void KickAdvanced(const vect3d_t& ball, const vect3d_t& target, uint8_t kicktype);

	/**
	 * Send a static GoalieRight command to the Body for a Motion.
	 */
	void GoaliMoveRight();

	/**
	 * Send a static GoalieLeft command to the Body for a Motion.
	 */
	void GoaliMoveLeft();

	/**
	 * Send a static Hi command to the Body for a Motion.
	 */
	void MoveHi();

	/**
	 * Send a static YeahGoal (Both hands up) command to the Body for a Motion.
	 */
	void YeahGoal();

	/**
	 * Send a Motion command to the Body.
	 * @param motion the motion to be executed
	 */
	void PlayMotion( enum ICommunicator::Motion motion);

	/**
	 * Get the PanTiltUnit.
	 * @return Returns a reference for the PanTiltUnit. PanTiltUnit controls all Pantilt actions and Corrections.
	 */
	PanTiltUnit* PanTilt();

	/**
	 * Get the paralysis status
	 * @return Returns true if the Robot is not allowed to move.
	 */
	bool isParalysed() const;

	/**
	 * Set the paralysis status. Stops the robot if value changed from false to true.
	 * @param paralyze is the robot allowed to move (true) or not (false).
	 */
	void setParalysis(bool paralyze);

	/**
	 * Get the time of the last kick
	 */
	uint64_t getKickingTime() const;

	/**
	 * Get the voltage of the battery
	 * @return The voltage of the battery if available, else 0.0.
	 */
	double GetBatteryVoltage();

	/**
	 * Return the Data from the IMU from the Statusmessage.
	 * @param pitch the pitch angle (degree)
	 * @param roll the roll angle (degree)
	 */
	void GetIMUData(int* pitch, int* roll);

	/**
	 * Get the Statusmessage send by the body.
	 * @return pointer to the last statusMessage send by the body
	 */
	const ICommunicator::statusMessage_t* GetStatusMessage();

private:
	//lint -e(1704)
    Body(const Body& cSource);				//!<  Private copy constructor
    Body& operator=(const Body& cSource);	//!<  Copy method

	PanTiltUnit* mPanTilt;	//!< A PanTiltUnit to handle all Commands going to the PanTilt.
	ICommunicator* mComm;	//!< The Communicator for the communication with the body.
	uint64_t mKickingTime;	//!< Kicking time. Time of the last kick.
	bool mParalysis;		//!< Boolean says if the robot is allowed to move.
	bool mDebugEnabled;		//!< If True Debugmessages will be send.
};

#endif /* BODY_H_ */
