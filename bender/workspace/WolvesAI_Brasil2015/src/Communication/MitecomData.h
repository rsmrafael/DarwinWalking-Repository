/*
 * MitecomData.h
 *
 * Definitions of enums and structures for the Mixed Team Protocol.
 *
 *  Created on: 24.02.2014
 *      Author: Oliver Krebs
 */

#ifndef MITECOM_DATA_H__
#define MITECOM_DATA_H__

#include <cstdlib>
#include <inttypes.h>
#include <sys/time.h>
//#include <limits.h>
#include <climits>
#include <map>
#include "../Utils/Time.h"

static const size_t MITECOM_LENGTH_MAX = SHRT_MAX; //SHRT_MAX

/// the magic value for network packets
static const uint32_t MITECOM_MAGIC = 0x4D585443; //'MXTC' little endian

/// the version number
static const uint32_t MITECOM_VERSION = 1;

typedef uint32_t MITECOM_KEYTYPE;
typedef int32_t MITECOM_DATATYPE;

/*------------------------------------------------------------------------------------------------*/

/// enum for the global roles
//enum MixedTeamRoleEnum {
static const MITECOM_DATATYPE ROLE_FIELD_RUNNER = -2;
static const MITECOM_DATATYPE ROLE_REMOTE = -1;

/// robot is not doing anything
static const MITECOM_DATATYPE ROLE_IDLING = 0;

/// undefined role, no role or some weird stuff
static const MITECOM_DATATYPE ROLE_OTHER = 1;

/// A striker is actively pursuing the ball.
static const MITECOM_DATATYPE ROLE_STRIKER = 2;

/// A supporter is positioning itself close to the striker or the
/// ball to be able to take over the striker role if necessary,
/// and/or to block access to the ball or to be the first line of
/// defense once the striker is removed/fails.
static const MITECOM_DATATYPE ROLE_SUPPORTER = 3;

/// A defender is positioning itself not too far away from the goal
/// in order to defend the goal against opponent strikers
static const MITECOM_DATATYPE ROLE_DEFENDER = 4;

/// A goalie is positioned inside the penalty box, and the last line
/// of defense. It may touch the ball, and it has special protection.
static const MITECOM_DATATYPE ROLE_GOALIE = 5;
static const MITECOM_DATATYPE ROLE_PLACE_ROBOT = 6;
//};

//typedef MixedTeamRoleEnum MixedTeamRole;

/*------------------------------------------------------------------------------------------------*/

/// enum describing the current action performed by the robot
//enum MixedTeamActionEnum {
/// undefined action (if nothing else matches)
static const MITECOM_DATATYPE ACTION_UNDEFINED = 0;

/// robot is trying to position at a certain position
static const MITECOM_DATATYPE ACTION_POSITIONING = 1;

/// robot is trying to reach the ball
static const MITECOM_DATATYPE ACTION_GOING_TO_BALL = 2;

/// robot is in the possession of the ball (i.e. ball is in front)
/// and is actively trying to move the ball into the opponent goal
/// by e.g. dribbling or kicking ...
static const MITECOM_DATATYPE ACTION_TRYING_TO_SCORE = 3;

/// robot is waiting for an event (e.g. ball coming closer)
static const MITECOM_DATATYPE ACTION_WAITING = 4;
//};

//typedef MixedTeamActionEnum MixedTeamAction;

/*------------------------------------------------------------------------------------------------*/

/// the playing state of the robot
//enum MixedTeamStateEnum {
/// robot is not doing anything or incapable of doing anything
static const MITECOM_DATATYPE STATE_INACTIVE = 0;

/// the robot is ready to play or is playing already
static const MITECOM_DATATYPE STATE_ACTIVE = 1;

/// The robot is penalized
static const MITECOM_DATATYPE STATE_PENALIZED = 2;
//};

//typedef MixedTeamStateEnum MixedTeamState;

/*------------------------------------------------------------------------------------------------*/

static const MITECOM_KEYTYPE MITECOM_RANGE_STATE = 0x00000000;
static const MITECOM_KEYTYPE MITECOM_RANGE_COGNITION = 0x00010000;
static const MITECOM_KEYTYPE MITECOM_RANGE_CAPABILITIES = 0x00020000;
static const MITECOM_KEYTYPE MITECOM_RANGE_USERDEFINED = 0x10000000; // individual settings

/// enum for addressing the different values the team shares
//enum MixedTeamKeyEnum {

/* ******************************************************************
 ** CURRENT ROBOT STATE AND PLANNING
 ** *****************************************************************/

/// current role
static const MITECOM_KEYTYPE ROBOT_CURRENT_ROLE = MITECOM_RANGE_STATE + 0;

/// current action
static const MITECOM_KEYTYPE ROBOT_CURRENT_ACTION = MITECOM_RANGE_STATE + 1;

/// current state (init, active, inactive)
static const MITECOM_KEYTYPE ROBOT_CURRENT_STATE = MITECOM_RANGE_STATE + 2;

/* ******************************************************************
 ** modelling and percept data
 ** *****************************************************************/

/**
 * center for absolute positions is the center of the field
 *
 *   y
 *        my side               op side
 *  +3   -------------------------------
 *   |   |              |              |
 *   |   |--            |            --|
 *   |   o |           -|-           | o
 *   0   | |    +     | + |     +    | |
 *   |   o |           -|-           | o
 *   |   |--            |            --|
 *   |   |              |              |
 *  -3   -------------------------------
 *
 *     -4.5-------------0------------+4.5    x
 */

/// absolute position on the field, x-coordinate (in mm)
static const MITECOM_KEYTYPE ROBOT_ABSOLUTE_X = MITECOM_RANGE_COGNITION + 0;

/// absolute position on the field, y-coordinate (in mm)
static const MITECOM_KEYTYPE ROBOT_ABSOLUTE_Y = MITECOM_RANGE_COGNITION + 1;

/// orientation on the field, in degree
static const MITECOM_KEYTYPE ROBOT_ABSOLUTE_ORIENTATION =
		MITECOM_RANGE_COGNITION + 2;

/// belief of absolute position on the field (0..255), with 0 = no confidence
/// and 255 = highest confidence
static const MITECOM_KEYTYPE ROBOT_ABSOLUTE_BELIEF = MITECOM_RANGE_COGNITION
		+ 3;

/// relative position of ball to robot, x-coordinate (in mm)
static const MITECOM_KEYTYPE BALL_RELATIVE_X = MITECOM_RANGE_COGNITION + 4;

/// relative position of ball to robot, y-coordinate (in mm)
static const MITECOM_KEYTYPE BALL_RELATIVE_Y = MITECOM_RANGE_COGNITION + 5;

/// absolute position of ball on the field, x-coordinate (in mm)
static const MITECOM_KEYTYPE BALL_ABSOLUTE_X = MITECOM_RANGE_COGNITION + 6;

/// absolute position of ball on the field, y-coordinate (in mm)
static const MITECOM_KEYTYPE BALL_ABSOLUTE_Y = MITECOM_RANGE_COGNITION + 7;

/* ******************************************************************
 ** ROBOT CAPABILITIES
 ** *****************************************************************/

/// average walking speed of robot (in cm/s)
static const MITECOM_KEYTYPE ROBOT_AVG_WALKING_SPEED_IN_CM_PER_SECOND =
		MITECOM_RANGE_CAPABILITIES + 1;

/// expected time (in seconds) for robot to reach the ball and
/// position itself behind it correctly (i.e. aligned to opp goal)
static const MITECOM_KEYTYPE ROBOT_TIME_TO_POSITION_AT_BALL_IN_SECONDS =
		MITECOM_RANGE_CAPABILITIES + 2;

/// the maximum (realistic) distance the ball can be kicked
static const MITECOM_KEYTYPE ROBOT_MAX_KICKING_DISTANCE =
		MITECOM_RANGE_CAPABILITIES + 3;
//};

//typedef MixedTeamKeyEnum MixedTeamKey;

/*------------------------------------------------------------------------------------------------*/

/** Structure for a single value
 **
 */
struct MixedTeamCommValueStruct {
	MITECOM_KEYTYPE key; 	//!< key (MixedTeamKey)
	MITECOM_DATATYPE value; //!< value
};

typedef MixedTeamCommValueStruct MixedTeamCommValue;

/*------------------------------------------------------------------------------------------------*/

/**
 * The message transmitted over the network.
 */
struct MixedTeamCommMessageStruct {
	uint32_t messageMagic; 		//!< protocol header magic bytes 'MXTC', little endian

	uint16_t messageVersion;	//!< the version of the message protocol, little endian
	uint16_t messageLength; 	//!< number of values, little endian

	uint32_t messageFlags; 		//!< flags

	uint16_t teamID; 			//!< ID of the team, little endian
	uint16_t robotID; 			//!< ID of the sending robot, little endian

	MixedTeamCommValueStruct values[1];	//!< values
};

typedef MixedTeamCommMessageStruct MixedTeamCommMessage;

/*------------------------------------------------------------------------------------------------*/

typedef std::map<MITECOM_KEYTYPE, MITECOM_DATATYPE> MixedTeamMateData;
typedef std::map<MITECOM_KEYTYPE, MITECOM_DATATYPE>::const_iterator MixedTeamMateDataCIter;

/**
 * Data of a team mate
 */
struct MixedTeamMateStruct {
	uint16_t robotID; 			//!< ID of the sending robot
	uint64_t lastUpdate; 		//!< time of last update (in milliseconds)

	MixedTeamMateData data;		//!< data
};

typedef MixedTeamMateStruct MixedTeamMate;

/*------------------------------------------------------------------------------------------------*/

typedef std::map<uint16_t, MixedTeamMate> MixedTeamMates;
typedef std::map<uint16_t, MixedTeamMate>::const_iterator MixedTeamMatesCIter;

/*------------------------------------------------------------------------------------------------*/

#endif // MITECOM_DATA_H__
