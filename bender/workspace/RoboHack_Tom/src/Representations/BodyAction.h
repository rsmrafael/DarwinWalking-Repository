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

class BodyAction {
public:
	BodyAction();
	virtual ~BodyAction();

	void setWalkerCommand(int16_t x, int16_t y, int16_t r);
	void setWalkerCommand(int16_t x, int16_t y, int16_t r, bool turningAim);

	void stop();
	void kickLeft();
	void kickRight();
	void kickBall(const vect3d_t& ball);
	void kickToTarget(const vect3d_t& ball, const vect3d_t& target);
	void kickAdvanced(const vect3d_t& ball, const vect3d_t& target, uint8_t kicktype);
	void cancelKick();
	void goaliMoveRight();
	void goaliMoveLeft();
	void circleRight();
	void circleLeft();
	void playMotion( enum ICommunicator::Motion motion);

	uint8_t getMovementChangeID() const { return mMovementChangeID; }
	bool getIsWalkerCommand() const { return mWalkerCommand; }
	int8_t getWalkerX() const { return mWalkerX; }
	int8_t getWalkerY() const { return mWalkerY; }
	int8_t getWalkerR() const { return mWalkerR; }
	bool getWalkerTurningAim() const { return mWalkerTurningAim; }
	ICommunicator::Motion getMotion() const { return mMotion; }
	vect3d_t getPositionBall() const {return mBall; }
	vect3d_t getPositionTarget() const {return mTarget; }
	uint8_t getKicktype() const {return mKicktype; }

private:
	int8_t mWalkerX;
	int8_t mWalkerY;
	int8_t mWalkerR;
	bool mWalkerTurningAim;

	ICommunicator::Motion mMotion;

	bool mWalkerCommand;
	uint8_t mMovementChangeID;

	vect3d_t mBall;
	vect3d_t mTarget;
	uint8_t mKicktype;
};

template<> class Serializer<BodyAction> {
	public:
		static void serialize(const BodyAction& representation, std::ostream& stream);
		static void deserialize(std::istream& stream, BodyAction& representation);
};


#endif /* BODYACTION_H_ */
