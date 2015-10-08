/*
 * FieldRunner.h
 *
 *  Created on: 22.07.2014
 *      Author: da-v1n
 */

#ifndef FIELDRUNNER_H_
#define FIELDRUNNER_H_

#include "AbstractBehavior.h"

class FieldRunner
:	public AbstractBehavior,
	public ConfigChangeListener
{
public:
	virtual void executeBehavior();
	virtual void updateState();
	FieldRunner(Game *game, MitecomCommunicator *communicator);
	virtual ~FieldRunner();
	virtual string getStateName(int state);
	virtual void configChanged();
	void gameStateHasChanged(Game::GameState state);
protected:
	void entryScanfield();
	int processScanfield();

	void entryMoveFieldline();
	int processMoveFieldline();

	void entryTurnHalf();
	int processTurnHalf();

	void entryCheckTurn();
	int processCheckTurn();

private:
	enum {
			MOVE_TO_FIELDLINE,
			TURN_HALF,
			CHECK_TURN
		};

	int mMoveTiltAngle;
	int mTurnFieldBorderX;
	int mTurnFieldBorderMinY;
	uint64_t mTurnTime;
	uint64_t mTimeout;
	int mCheckTurnPan;
	int mCheckTurnTilt;
	int mCheckFieldBorderX;
	int mCheckFieldBorderMaxY;
	int16_t mTurningXMovement;
	int16_t mMoveForwardX;
	int16_t mMoveForwardY;
};

#endif /* FIELDRUNNER_H_ */
