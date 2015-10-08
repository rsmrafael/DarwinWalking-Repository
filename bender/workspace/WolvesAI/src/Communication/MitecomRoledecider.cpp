/*
 * MitecomRoledecider.cpp
 *
 *  Created on: 24.02.2014
 *      Author: Oliver Krebs
 */

#include "MitecomRoledecider.h"

//#include <tgmath.h>

MITECOM_DATATYPE Mitecom::decideRole(const MixedTeamMates &teamMates, const MixedTeamMate &me)
{
	if (teamMates.empty()) {
		return ROLE_STRIKER;
	}

	MixedTeamMateDataCIter data = me.data.find(ROBOT_CURRENT_ROLE);
	if( data != me.data.end() && data->second == ROLE_GOALIE) {
		return ROLE_GOALIE;
	}

	MixedTeamMateDataCIter dataX, dataY;
	dataX = me.data.find(BALL_RELATIVE_X);
	dataY = me.data.find(BALL_RELATIVE_Y);
	if( dataX == me.data.end() || dataY == me.data.end()) {
		return ROLE_OTHER;
	}

	int32_t myBallX = dataX->second;
	int32_t myBallY = dataY->second;
	//quadratic euklidian distance
	int32_t myDistanceToBall = (myBallX * myBallX) + (myBallY * myBallY);

	bool closest = true;
	bool furthest = true;

	MixedTeamMatesCIter it;
	for ( it = teamMates.begin(); it != teamMates.end(); ++it) {
		MixedTeamMate mate = it->second;
		data = mate.data.find(ROBOT_CURRENT_ROLE);
		if( data == mate.data.end() || data->second != ROLE_GOALIE) {
			dataX = mate.data.find(BALL_RELATIVE_X);
			dataY = mate.data.find(BALL_RELATIVE_Y);
			if( dataX != mate.data.end() && dataY != mate.data.end()) {
				int32_t mateBallX = dataX->second;
				int32_t mateBallY = dataY->second;
				int32_t mateDistanceToBall = (mateBallX * mateBallX) + (mateBallY * mateBallY);

				//compare distances to ball
				if (mateDistanceToBall < myDistanceToBall) {
					closest = false;
				} else if (mateDistanceToBall > myDistanceToBall) {
					furthest = false;
				}
			}
		}
	}

	//decide on role
	if (closest) {
		return ROLE_STRIKER;
	} else if( furthest) {
		return ROLE_DEFENDER;
	} else {
		return ROLE_SUPPORTER;
	}
}


/*------------------------------------------------------------------------------------------------*/

