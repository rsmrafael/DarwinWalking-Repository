/*
 * motionselect.h
 *
 *  Created on: 08.03.2012
 *      Author: Stefan
 */

#ifndef MOTIONSELECT_H_
#define MOTIONSELECT_H_

#define MAX_MOTION_SELECTS	20

enum MotionSelectIDs {
	GETUP_FRONT = 0,
	GETUP_BACK = 1,
	KICK_LEFT = 2,
	KICK_RIGHT = 3,
	GOALIE_MOVE_LEFT = 4,
	GOALIE_MOVE_RIGHT = 5,
	END_OF_MOTION_SELECT_LIST
};

struct t_motion_select {
	unsigned char version;
	unsigned char motionIDs[MAX_MOTION_SELECTS];
};

#ifdef USE_NAME_RES
char* GetMotionSelectIDName(unsigned char id) {
#define IDTOCASE(n)	case n: return #n
	switch (id) {
		IDTOCASE(GETUP_FRONT);
		IDTOCASE(GETUP_BACK);
		IDTOCASE(KICK_LEFT);
		IDTOCASE(KICK_RIGHT);
		IDTOCASE(GOALIE_MOVE_LEFT);
		IDTOCASE(GOALIE_MOVE_RIGHT);
		default:
			return "<unknown>";
	}
}
#endif

#endif /* MOTIONSELECT_H_ */