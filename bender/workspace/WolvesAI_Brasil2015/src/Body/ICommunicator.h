/*
 * ICommunicator.h
 *
 *  Created on: 07.03.2011
 *      Author: Stefan
 */

#ifndef ICOMMUNICATOR_H_
#define ICOMMUNICATOR_H_

#include <stdint.h>
#include <string>
#include "../Utils/Geometry/Geometry.h"
#include "../Utils/Constants.h"

/*!
 * ICommunicator is an Interface used for declaring structures and Motion information for communication
 * with the Body (Real or Simulation).
 */
class ICommunicator {

public:
	/**
	 * Motion enum is the declaration of indicies value to send to the body. In the Body the Motions have the same
	 * Hex values so they can be recognised.
	 */
	enum Motion {
		Stop 			= 0x00,	//!< Stop Let the robot halt
		KickLeft 		= 0x06,	//!< KickLeft let the Robot Kick with left leg
		KickRight		= 0x07,	//!< KickRight let the Robot Kick with right leg
		CancelKick		= 0x08,	//!< Cancel Kick let the KICKENGINE cancel the current kick
		GoaliMoveLeft 	= 0x0B,	//!< Goalie move should halt a ball to the left
		GoaliMoveRight 	= 0x0C,	//!< Goalie move should halt a ball to the right
		MoveHi			= 0x0E,	//!< Raise a arm and wink with him
		YeahGoal		= 0x0F,	//!< Both Hands should be to the air
		RaiseLeftHand	= 0x20,	//!< Left hand raised (Visual Debug)
		RaiseRightHand	= 0x21,	//!< Right hand raised (Visual Debug)
		RaiseBothHands	= 0x22,	//!< Both hands raised (Visual Debug)
		KickBall		= 0x51,
		KickForward		= 0x52,
		KickSideward	= 0x53,
		KickBackward	= 0x54,
		KickToTarget	= 0x55,
		KickAdvanced	= 0x56
	};

	/**
	 * ServoData struct is used get a message from the body with data over the Servo.
	 */
	struct servoData {
		uint8_t maxPositionError;	//!< anomaly of the target position and the real position
		uint8_t temperature;		//!< temperature of the servo
		uint8_t voltage;			//!< voltage at the servo
	} __attribute__((packed));		//!< minimize size, no padding between members

	typedef struct servoData servoData_t;	//!< typedef for servoData

	/**
	 * ImuData struct is used to get a message from the body with the Imudata (Accelometer,Gyro)
	 */
	struct imuData {
		int fb_accel;	//!< Front/Back accelometer data
		int rl_accel;	//!< Right/Left accelometer data
		int fb_gyro;	//!< Front/Back gyro data
		int rl_gyro;	//!< Right/Left gyro data
	} __attribute__((packed));	//!< minimize size, no padding between members

	typedef struct imuData imuData_t;		//!< typedef for imuData

	/**
	 * OdometryData struct is used to get a message from the body with the Odometry data since the last sending
	 * So it is only the differnce since the last message.
	 */
	struct odometryData {
		int16_t xPos;			//!< xPos is the xChange since the last message
		int16_t yPos;			//!< yPos is the yChange since the last message
		int16_t viewDirection;	//!< viewDirection is the change of rotation since the last message
		uint8_t weight;			//!< weight is the Possability that the data are right
	} __attribute__((packed));	//!< minimize size, no padding between members

	typedef struct odometryData odometryData_t;		//!< typedef for odometryData

	/**
	 * Status message is the Message come from the body off the Robot very often.
	 */
	struct statusMessage {
		uint16_t 		voltage;						//!< voltage of the battery
		servoData_t 	servo_status[SERVO_COUNT_MAX];	//!< data of servo like temperature
		imuData_t 		imu;							//!< IMU data of the robot
		odometryData_t 	odometry;						//!< Odometry get from the robot (X,Y,R,Probability)
		uint16_t 		currentState;					//!< The state of the Body (Stop, Walking, Motion)
	} __attribute__((packed));							//!< minimize size, no padding between members

	typedef struct statusMessage statusMessage_t;		//!< typedef for statusMessage

	/**
	 * Destructor off the Communicator
	 */
    virtual ~ICommunicator() {};

    /**
     * RepeatedMovementCommand send a motion to the body to say him what to do
     * @param page the Page witch should be place from the enum Motion.
     * @param overwriteRunning change the running page with the new one(true) or not(false).
     * @param repetitions to send. How often the message should be repeated.
     */
	virtual void RepeatedMovementCommand(Motion page, bool overwriteRunning, uint16_t repetitions) = 0;

	/**
	 * StrengthDirectionMovementCommand send a motion to the body to say him what to do
	 * @param page the Page witch should be place from the enum Motion.
	 * @param overwriteRunning change the running page with the new one(true) or not(false).
	 * @param strength of the motion.
	 * @param direction where the motion should be directed.
	 */
	virtual void StrengthDirectionMovementCommand(Motion page, bool overwriteRunning, uint16_t strength, uint16_t direction) = 0;

	/**
	 * KickerCommand send a message to the Kickengine.
	 * @param ball the position off the ball to kick.
	 */
	virtual void KickerCommand(vect3d_t ball) = 0;

	/**
	 * KickerCommand send a message to the Kickengine.
	 * @param ball the position off the ball to kick.
	 * @param target the direction where to kick.
	 */
	virtual void KickerCommand(vect3d_t ball, vect3d_t target) = 0;

	/**
	 * KickerCommand send a message to the Kickengine.
	 * @param ball the position off the ball to kick.
	 * @param target the direction where to kick.
	 * @param kicktype determine if it should
	 */
	virtual void KickerCommand(vect3d_t ball, vect3d_t target, uint8_t kicktype) = 0;

	/**
	 * PanTiltCommand is used to set the Pan Tilt of the robot.
	 * @param pan is used for right/left in degree
	 * @param tilt is used for top/down in degree.
	 */
	virtual void PanTiltCommand(int16_t pan, int16_t tilt) = 0;

	/**
	 * Walkercommand send the walker data to the body to move.
	 * @param x is used for forward-
	 * @param y is used for sideward.
	 * @param r is the rotation of the robot.
	 * @param turningAim is true if he wants only to turn.
	 */
	virtual void WalkerCommand(int8_t x, int8_t y, int8_t r, bool turningAim) = 0;

	/**
	 * Get the newest status message with data of the robot.
	 * @return pointer to the status message or NULL if not available
	 */
	virtual const statusMessage_t* GetStatusMessage() = 0;
};

#endif /* ICOMMUNICATOR_H_ */
