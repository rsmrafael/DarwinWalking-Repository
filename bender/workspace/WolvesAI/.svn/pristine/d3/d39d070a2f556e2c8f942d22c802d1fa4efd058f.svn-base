/*
 * PlaceRobot.h
 *
 *  Created on: 05.02.2015
 *      Author: bombadil
 */

#ifndef PLACEROBOT_H_
#define PLACEROBOT_H_
#include "AbstractBehavior.h"
#include "../../Representations/Point.h"
#include <math.h>

/*
 * Place Robot
 */
class PlaceRobot:public AbstractBehavior {
public:
	/**
	 * Constructor
	 * @param game			information of the game
	 * @param communicator	mitecom communicator
	 */
	PlaceRobot(Game *game, MitecomCommunicator *communicator);

	/**
	 * Destructor
	 */
	virtual ~PlaceRobot();

	/**
	 * get the name of the given state
	 * @param state		index of state
	 * @return name of the given state
	 */
	virtual string getStateName(int state);

protected:
	virtual void executeBehavior();		//!< execute striker
	virtual void updateState();			//!< update state

	void entryMoveHalf();				//!< entry state MOVE_HALF
	int processMoveHalf();				//!< process state MOVE_HALF

	void entryRotateToGoal();			//!< entry state ROTATE_TO_GOAL
	int processRotateToGoal();			//!< process state ROTATE_TO_GOAL

	void entryMoveToGoal();				//!< entry state MOVE_TO_GOAL
	int processMoveToGoal();			//!< process state MOVE_TO_GOAL

private:
	enum {
		MOVE_TO_HALF,					//!< move to half
		ROTATE_TO_GOAL,					//!< rotate to goal
		MOVE_TO_GOAL					//!< move to goal
	};

	int often;							//!< often
	int lostCounterGoal;				//!< lost counter goal
};

#endif /* PLACEROBOT_H_ */
