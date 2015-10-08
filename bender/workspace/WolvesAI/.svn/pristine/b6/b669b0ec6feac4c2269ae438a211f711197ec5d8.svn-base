/*
 * FieldRunner.h
 *
 *  Created on: 22.07.2014
 *      Author: da-v1n
 */

#ifndef FIELDRUNNER_H_
#define FIELDRUNNER_H_

#include "AbstractBehavior.h"

/**
 * FieldRunner is the behavior for the technical challenge "Artificial Grass Challenge" from 2014
 */
class FieldRunner
:	public AbstractBehavior,
	public ConfigChangeListener
{
public:
	/**
	 * Constructor
	 * @param game			information of the game
	 * @param communicator	mitecom communicator
	 */
	FieldRunner(Game *game, MitecomCommunicator *communicator);

	/**
	 * Destructor
	 */
	virtual ~FieldRunner();

	/**
	 * called when config has changed
	 * @see ConfigChangeListener::configChanged
	 */
	virtual void configChanged();

	/**
	 * get the name of the given state
	 * @param state		index of state
	 * @return name of the given state
	 */
	virtual string getStateName(int state);

protected:
	virtual void executeBehavior();	//!< execute field runner
	virtual void updateState();		//!< update state

	void entryMoveFieldline();		//!< entry state MOVE_FIELD_LINE
	int processMoveFieldline();		//!< process state MOVE_FIELD_LINE

	void entryTurnHalf();			//!< entry state TURN_HALF
	int processTurnHalf();			//!< process state TURN_HALF

	void entryCheckTurn();			//!< entry state CHECK_TURN
	int processCheckTurn();			//!< process state CHECK_TURN

private:

	/**
	 * Actions for FieldRunner
	 */
	enum {
		MOVE_TO_FIELDLINE,	//!< move to field line
		TURN_HALF,			//!< turn a half circle (180 degrees)
		CHECK_TURN			//!< check turn
	};

	int mMoveTiltAngle;			//!< move tilt angle
	int mTurnFieldBorderX;		//!< turn field border x
	int mTurnFieldBorderMinY;	//!< turn field border min y
	uint64_t mTurnTime;			//!< turn time
	uint64_t mTimeout;			//!< timeout
	int mCheckTurnPan;			//!< check turn pan
	int mCheckTurnTilt;			//!< check turn tilt
	int mCheckFieldBorderX;		//!< check field border x
	int mCheckFieldBorderMaxY;	//!< check field border max y
	int16_t mTurningXMovement;	//!< turning x movement
	int16_t mMoveForwardX;		//!< move forward x
	int16_t mMoveForwardY;		//!< move forward y
};

#endif /* FIELDRUNNER_H_ */
