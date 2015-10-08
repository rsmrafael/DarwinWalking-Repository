/*
 * BodyAction.h
 *
 *  Created on: 26.11.2012
 *      Author: Stefan Krupop
 */

#ifndef BODYACTION_H_
#define BODYACTION_H_

#include "../Body/ICommunicator.h"
#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"

/**
 * Representation for all body actions
 */
class BodyAction {
public:
	/**
	 * Constructor
	 */
	BodyAction();

	/**
	 * Destructor
	 */
	virtual ~BodyAction();

	/**
	 * a walk command
	 * @param x		front/back
	 * @param y		right/left
	 * @param r		rotation
	 */
	void setWalkerCommand(int16_t x, int16_t y, int16_t r);

	/**
	 * a walk command
	 * @param x				front/back
	 * @param y				right/left
	 * @param r				rotation
	 * @param turningAim	turn
	 */
	void setWalkerCommand(int16_t x, int16_t y, int16_t r, bool turningAim);

	/**
	 * stop walk commands
	 */
	void stop();

	/**
	 * kick with left side
	 */
	void kickLeft();

	/**
	 * kick with right side
	 */
	void kickRight();

	/**
	 * kick ball
	 * @param ball		relative position of ball
	 */
	void kickBall(const vect3d_t& ball);

	/**
	 * kick ball to target
	 * @param ball		relative position of ball
	 * @param target	relative position of kick target
	 */
	void kickToTarget(const vect3d_t& ball, const vect3d_t& target);

	/**
	 * kick advanced
	 * @param ball		relative position of ball
	 * @param target	relative position of kick target
	 * @param kicktype	type of kick
	 */
	void kickAdvanced(const vect3d_t& ball, const vect3d_t& target, uint8_t kicktype);

	/**
	 * cancel kick
	 */
	void cancelKick();

	/**
	 * goalie movement to save ball right side
	 */
	void goaliMoveRight();

	/**
	 * goalie movement to save ball left side
	 */
	void goaliMoveLeft();

	/**
	 * circle movement right
	 */
	void circleRight();

	/**
	 * circle movement left
	 */
	void circleLeft();

	/**
	 * run a static motion
	 * @param motion		the motion to be executed
	 */
	void playMotion( enum ICommunicator::Motion motion);

	/**
	 * return the ID of last change
	 */
	uint8_t getMovementChangeID() const { return mMovementChangeID; }

	/**
	 * return if last command was walker command
	 */
	bool getIsWalkerCommand() const { return mWalkerCommand; }

	/**
	 * return walker parameter x
	 */
	int8_t getWalkerX() const { return mWalkerX; }

	/**
	 * return walker parameter y
	 */
	int8_t getWalkerY() const { return mWalkerY; }

	/**
	 * return walker parameter r (rotation)
	 */
	int8_t getWalkerR() const { return mWalkerR; }

	/**
	 * return if walkerTurningAim enabled
	 */
	bool getWalkerTurningAim() const { return mWalkerTurningAim; }

	/**
	 * return last motion
	 */
	ICommunicator::Motion getMotion() const { return mMotion; }

	/**
	 * return ball position
	 */
	vect3d_t getPositionBall() const {return mBall; }

	/**
	 * return kick target position
	 */
	vect3d_t getPositionTarget() const {return mTarget; }

	/**
	 * return type of kick
	 */
	uint8_t getKicktype() const {return mKicktype; }

private:
	int8_t mWalkerX;				//!< walker parameter x
	int8_t mWalkerY;				//!< walker parameter y
	int8_t mWalkerR;				//!< walker parameter r
	bool mWalkerTurningAim;			//!< walker turning aim

	ICommunicator::Motion mMotion;	//!< static motion

	bool mWalkerCommand;			//!< was a walker command
	uint8_t mMovementChangeID;		//!< ID of last movement change

	vect3d_t mBall;					//!< ball position
	vect3d_t mTarget;				//!< target position
	uint8_t mKicktype;				//!< kick type
};

/**
 * spezialization of Serializer for BodyAction
 */
template<> class Serializer<BodyAction> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const BodyAction& representation, std::ostream& stream);

	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(std::istream& stream, BodyAction& representation);
};


#endif /* BODYACTION_H_ */
