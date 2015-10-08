/*
 * SettingData.h
 *
 * Data of environment and game objects
 * all sizes are in mm
 *
 *  Created on: 05.02.2014
 *      Author: Oliver Krebs
 */

#ifndef SETTINGDATA_H_
#define SETTINGDATA_H_

// the ball
const int BALL_DIAMETER = 100;

// the field of play
const int FIELD_LENGTH = 9000;				// A
const int FIELD_WIDTH = 6000;				// B
const int GOAL_DEPTH = 500;					// C
const int GOAL_WIDTH = 2250;				// D
const int GOAL_AREA_LENGTH = 600;			// E
const int GOAL_AREA_WIDTH = 3450;			// F
const int PENALTY_MARK_DISTANCE = 1800;		// G
const int CENTER_CIRCLE_DIAMETER = 1500;	// H
const int BORDER_STRIP_WIDTH_MIN = 700;		// I

const int AREA_LENGTH = 10400;				// field with border in x (A + 2*I)
const int AREA_WIDTH = 7400;				// field with border in y (B + 2*I)
const int AREA_LENGTH_HALF = 5200;			// AREA_LENGTH / 2
const int AREA_WIDTH_HALF = 3700;			// AREA_WIDTH / 2

const int FIELD_LENGTH_HALF = 4500;			// A/2
const int FIELD_WIDTH_HALF = 3000;			// B/2

//const int FIELD_DIAGONAL = 10817;		// diagonal of the field: sqrt(FIELD_LENGTH*FIELD_LENGTH + FIELD_WIDTH*FIELD_WIDTH)
//const int FIELD_DIAGONAL_HALF = 5408;	//

const int POLE_TO_POLE_DIAGONAL = 9277;

//const int PENALTY_MARK_TO_POLE_DISTANCE = 2123;
//const int PENALTY_MARK_TO_CORNER_DISTANCE = 3499;

const int CENTER_TO_GOAL_POLE = 4638;
const double GOAL_TO_CENTER_ANGLE = 1.326244; // 75.9°
const double CENTER_TO_GOAL_ANGLE = 0.244552; // 14.0°

const int LINE_WIDTH = 50;
const int LINE_SEGMENT_LENGTH = 100;		// penalty mark and center mark

const int GOAL_POLE_DIAMETER = 100;
const int GOAL_CROSSBAR_DIAMETER = 100;
const int GOAL_CROSSBAR_HEIGHT = 1100;
const int GOAL_NET_HEIGHT = 1000;

// robots
const int ROBOT_HEIGHT_MIN = 400;
const int ROBOT_HEIGHT_MEDIAN = 500;	// median of all robots height
const int ROBOT_HEIGHT_MAX = 900;
const int ROBOT_WIDTH_MEDIAN = 200;

const int OBSTACLE_HEIGHT_MEDIAN = 1000;
const int OBSTACLE_WIDTH_MEDIAN = 300;

#endif /* SETTINGDATA_H_ */
