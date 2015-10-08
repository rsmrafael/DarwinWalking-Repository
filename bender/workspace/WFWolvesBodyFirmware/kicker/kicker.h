/*
 * kicker.h
 *
 *  Created on: 13.11.2012
 *      Author: Oliver
 */

#ifndef KICKER_H_
#define KICKER_H_

#include "kinematics/inverse.h"

struct kicker_Parameters {
	unsigned char version;
	signed short balance_y_l_offset;
	signed short balance_y_r_offset;
	float balance_rl_p_gain;
	float balance_rl_i_gain;
	float balance_rl_d_gain;
	float balance_fb_p_gain;
	float balance_fb_i_gain;
	float balance_fb_d_gain;
	unsigned char balance_enable;
} kicker_Params;

struct kicker_Servo_Parameters {
	float servo_gain_p;
	float servo_gain_i;
	float servo_gain_d;
} kicker_ServoParams;

struct kicker_Offset_Parameters {
	signed short hip_roll_offset_l;
	signed short hip_roll_offset_r;
	signed short hip_pitch_offset_l;
	signed short hip_pitch_offset_r;
	signed short ankle_roll_offset_l;
	signed short ankle_roll_offset_r;
	signed short ankle_pitch_offset_l;
	signed short ankle_pitch_offset_r;
} kicker_OffsetParams;

enum kicker_type {
	KICKER_TYPE_MINIMAL,	// very short range kick	(stable)
	KICKER_TYPE_SHORT,		// short range kick		 	(stable)
	KICKER_TYPE_MEDIUM,		// medium range kick		(medium stable)
	KICKER_TYPE_POWER,		// high range kick 			(light unstable)
	KICKER_TYPE_MAXIMAL		// very high range kick 	(unstable)
};

#define ABS(x)			(x >= 0 ? x : -x)
#define SGN(x)			(x > 0 ? 1 : (x < 0 ? -1 : 0))

/**
 * initializes the kicker
 * call one-time before first use of kicker functions
 */
void Kicker_Init(void);

/**
 * load the parameters of kicker from eeprom
 */
void Kicker_LoadParams(void);

/**
 * Save the parameters of kicker to eeprom
 */
void Kicker_SaveParams(void);

/**
 * returns 1 if a kick is executed currently, else 0
 */
unsigned char Kicker_IsRunning(void);

/**
 * abort actual kick and reset to standing position
 */
void Kicker_CancelKick(void);

/**
 * Kicks the ball with short range
 * returns 0 by failure
 */
unsigned char Kicker_Kick( struct vect3d* ball);

/**
 * Kicks the ball forward with given kicker type (range)
 * returns 0 by failure
 */
unsigned char Kicker_KickForward( struct vect3d* ball, enum kicker_type type);

/**
 * Kicks the ball backward with given kicker type (range)
 * returns 0 by failure
 */
unsigned char Kicker_KickBackward( struct vect3d* ball, enum kicker_type type);

/**
 * Kicks the ball sideward with given kicker type (range)
 * returns 0 by failure
 */
unsigned char Kicker_KickSideward( struct vect3d* ball, enum kicker_type type);

/**
 * Tries to kick the ball to the target
 * returns 0 by failure
 * WARN: This function is unfinished and may be incorrect
 */
unsigned char Kicker_KickToTarget( struct vect3d* ball, struct vect3d* target);

/**
 * Kicks the ball forward and high (ball: back position, target: target position)
 * returns 0 by failure
 * WARN: Do not use this function, it is only for testing!
 */
unsigned char Kicker_KickHigh( struct vect3d* ball, struct vect3d* target, enum kicker_type type);

/**
 * Test functions
 */
void Kicker_AddBalanceLeftOffset( signed short a);
void Kicker_AddBalanceRightOffset( signed short a);
signed short Kicker_GetBalanceLeftOffset(void);
signed short Kicker_GetBalanceRightOffset(void);
void Kicker_Balance(void);

#endif /* KICKER_H_ */
