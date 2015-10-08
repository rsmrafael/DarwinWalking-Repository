/*
 * kinematic_scene_data.h
 *
 *  Created on: 04.05.2011
 *      Author: Stefan
 */

#ifndef KINEMATIC_SCENE_DATA_H_
#define KINEMATIC_SCENE_DATA_H_

#define LEG_DOWN			100
#define LEG_UP				90
#define LEG_DISP_X_L_WALK	15
#define LEG_DISP_X_R_WALK	15
#define LEG_DISP_Y_L_WALK	28
#define LEG_DISP_Y_R_WALK	27

#define LEG_DISP_X_L		15
#define LEG_DISP_X_R		15
#define LEG_DISP_Y			25
#define LEG_YAW				25
#define LEG_DISP_KICK		30

#define STEP_TIME_WALK		100
#define HOLD_TIME_WALK		0
#define STEP_TIME_Y_WALK	(STEP_TIME_WALK * 2)
#define HOLD_TIME_Y_WALK	0

#define STEP_TIME			200
#define HOLD_TIME			0
#define STEP_TIME_Y			(STEP_TIME * 2)
#define HOLD_TIME_2			200

#define STEP_TIME_KICK		100

struct KinematicSceneList Walk_Forward = {
		8, 						// Number of scenes
		{{
		// ==== SCENE ====
		STEP_TIME_Y_WALK,		// fadeInTime in ms
		HOLD_TIME_Y_WALK, {		// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  LEG_DISP_X_L_WALK,  LEG_DISP_Y_L_WALK, LEG_DOWN,   0},	// left  leg vector + yaw
		{ -LEG_DISP_X_R_WALK,  LEG_DISP_Y_L_WALK,   LEG_UP,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME_WALK,			// fadeInTime in ms
		HOLD_TIME_WALK, {		// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{                  0,  LEG_DISP_Y_L_WALK, LEG_DOWN,   0},	// left  leg vector + yaw
		{                  0,  LEG_DISP_Y_L_WALK,   LEG_UP,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME_WALK,			// fadeInTime in ms
		HOLD_TIME_WALK, {		// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{ -LEG_DISP_X_L_WALK,  LEG_DISP_Y_L_WALK, LEG_DOWN,   0},	// left  leg vector + yaw
		{  LEG_DISP_X_R_WALK,  LEG_DISP_Y_L_WALK,   LEG_UP,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME_Y_WALK,		// fadeInTime in ms
		HOLD_TIME_Y_WALK, {		// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{ -LEG_DISP_X_L_WALK,                  0, LEG_DOWN,   0},	// left  leg vector + yaw
		{  LEG_DISP_X_R_WALK,                  0, LEG_DOWN,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME_Y_WALK,		// fadeInTime in ms
		HOLD_TIME_Y_WALK, {		// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{ -LEG_DISP_X_L_WALK, -LEG_DISP_Y_R_WALK,   LEG_UP,   0},	// left  leg vector + yaw
		{  LEG_DISP_X_R_WALK, -LEG_DISP_Y_R_WALK, LEG_DOWN,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME_WALK,			// fadeInTime in ms
		HOLD_TIME_WALK, {		// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{                  0, -LEG_DISP_Y_R_WALK,   LEG_UP,   0},	// left  leg vector + yaw
		{                  0, -LEG_DISP_Y_R_WALK, LEG_DOWN,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME_WALK,			// fadeInTime in ms
		HOLD_TIME_WALK, {		// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  LEG_DISP_X_L_WALK, -LEG_DISP_Y_R_WALK,   LEG_UP,   0},	// left  leg vector + yaw
		{ -LEG_DISP_X_R_WALK, -LEG_DISP_Y_R_WALK, LEG_DOWN,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME_Y_WALK,		// fadeInTime in ms
		HOLD_TIME_Y_WALK, {		// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  LEG_DISP_X_L_WALK,                  0, LEG_DOWN,   0},	// left  leg vector + yaw
		{ -LEG_DISP_X_R_WALK,                  0, LEG_DOWN,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}}
};

struct KinematicSceneList Turn_Right = {
		8, 					// Number of scenes
		{{
		// ==== SCENE ====
		STEP_TIME,				// fadeInTime in ms
		HOLD_TIME_2, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,        -LEG_DISP_Y,   LEG_UP, 0},			// left  leg vector + yaw
		{      -LEG_DISP_X_R,        -LEG_DISP_Y, LEG_DOWN, 0}			// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME * 2,				// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,        -LEG_DISP_Y,   LEG_UP, -LEG_YAW},	// left  leg vector + yaw
		{      -LEG_DISP_X_R,        -LEG_DISP_Y, LEG_DOWN, 0}			// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME,				// fadeInTime in ms
		HOLD_TIME_2, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,        -LEG_DISP_Y, LEG_DOWN, -LEG_YAW},	// left  leg vector + yaw
		{      -LEG_DISP_X_R,        -LEG_DISP_Y, LEG_DOWN, 0}			// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME * 2,			// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,                  0, LEG_DOWN, -LEG_YAW},	// left  leg vector + yaw
		{      -LEG_DISP_X_R,                  0, LEG_DOWN, 0}			// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME * 2,			// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,         LEG_DISP_Y, LEG_DOWN, -LEG_YAW},	// left  leg vector + yaw
		{      -LEG_DISP_X_R,         LEG_DISP_Y, LEG_DOWN, 0}			// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME,				// fadeInTime in ms
		HOLD_TIME_2, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,         LEG_DISP_Y, LEG_DOWN, -LEG_YAW},	// left  leg vector + yaw
		{      -LEG_DISP_X_R,         LEG_DISP_Y,   LEG_UP, 0}			// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME * 2,			// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,         LEG_DISP_Y, LEG_DOWN, 0},			// left  leg vector + yaw
		{      -LEG_DISP_X_R,         LEG_DISP_Y,   LEG_UP, 0}			// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME,				// fadeInTime in ms
		HOLD_TIME_2, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,                  0, LEG_DOWN, 0},			// left  leg vector + yaw
		{      -LEG_DISP_X_R,                  0, LEG_DOWN, 0}			// right leg vector + yaw
		// ==== END OF SCENE ====
		}}}
};

struct KinematicSceneList Turn_Left = {
		8, 						// Number of scenes
		{{
		// ==== SCENE ====
		STEP_TIME,				// fadeInTime in ms
		HOLD_TIME_2, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,        -LEG_DISP_Y,   LEG_UP, 0},			// left  leg vector + yaw
		{      -LEG_DISP_X_R,        -LEG_DISP_Y, LEG_DOWN, 0}			// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME * 2,				// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,        -LEG_DISP_Y,   LEG_UP, 0},			// left  leg vector + yaw
		{      -LEG_DISP_X_R,        -LEG_DISP_Y, LEG_DOWN, -LEG_YAW}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME,				// fadeInTime in ms
		HOLD_TIME_2, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,        -LEG_DISP_Y, LEG_DOWN, 0},			// left  leg vector + yaw
		{      -LEG_DISP_X_R,        -LEG_DISP_Y, LEG_DOWN, -LEG_YAW}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME * 2,			// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,                  0, LEG_DOWN, 0},			// left  leg vector + yaw
		{      -LEG_DISP_X_R,                  0, LEG_DOWN, -LEG_YAW}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME * 2,			// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,         LEG_DISP_Y, LEG_DOWN, 0},			// left  leg vector + yaw
		{      -LEG_DISP_X_R,         LEG_DISP_Y, LEG_DOWN, -LEG_YAW}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME,				// fadeInTime in ms
		HOLD_TIME_2, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,        LEG_DISP_Y, LEG_DOWN, 0},			// left  leg vector + yaw
		{      -LEG_DISP_X_R,        LEG_DISP_Y,   LEG_UP, -LEG_YAW}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME * 2,			// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,        LEG_DISP_Y, LEG_DOWN, 0},		// left  leg vector + yaw
		{      -LEG_DISP_X_R,        LEG_DISP_Y,   LEG_UP, 0}		// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME,				// fadeInTime in ms
		HOLD_TIME_2, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{       LEG_DISP_X_L,                  0, LEG_DOWN, 0},		// left  leg vector + yaw
		{      -LEG_DISP_X_R,                  0, LEG_DOWN, 0}		// right leg vector + yaw
		// ==== END OF SCENE ====
		}}}
};

struct KinematicSceneList Kick = {
		7, 						// Number of scenes
		{{
		// ==== SCENE ====
		STEP_TIME * 2,			// fadeInTime in ms
		HOLD_TIME_2, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  LEG_DISP_X_L,  LEG_DISP_Y, LEG_DOWN,   0},	// left  leg vector + yaw
		{ -LEG_DISP_X_R,  LEG_DISP_Y, LEG_UP,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME,				// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{             0,  LEG_DISP_Y, LEG_DOWN,   0},	// left  leg vector + yaw
		{ -LEG_DISP_X_R,  LEG_DISP_Y,   LEG_UP,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME,				// fadeInTime in ms
		HOLD_TIME_2, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{             0,  LEG_DISP_Y, LEG_DOWN,   0},	// left  leg vector + yaw
		{-LEG_DISP_KICK,  LEG_DISP_Y,   LEG_UP,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME_KICK,			// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{             0,  LEG_DISP_Y, LEG_DOWN,   0},	// left  leg vector + yaw
		{ LEG_DISP_KICK,  LEG_DISP_Y,   LEG_UP,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME * 2,			// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{             0,  LEG_DISP_Y, LEG_DOWN,   0},	// left  leg vector + yaw
		{ -LEG_DISP_X_R,  LEG_DISP_Y,   LEG_UP,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME,				// fadeInTime in ms
		HOLD_TIME_2, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  LEG_DISP_X_L,  LEG_DISP_Y, LEG_DOWN,   0},	// left  leg vector + yaw
		{ -LEG_DISP_X_R,  LEG_DISP_Y,   LEG_UP,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE ====
		STEP_TIME * 2,			// fadeInTime in ms
		HOLD_TIME, {			// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  LEG_DISP_X_L,  0, LEG_DOWN,   0},	// left  leg vector + yaw
		{ -LEG_DISP_X_R,  0, LEG_DOWN,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}}
};

struct KinematicSceneList DoNothing = {
		1, 						// Number of scenes
		{{
		// ==== SCENE ====
		1000,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  LEG_DISP_X_L,  		   0, LEG_DOWN,   0},	// left  leg vector + yaw
		{ -LEG_DISP_X_R,  		   0, LEG_DOWN,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}}
};

/////////////////////////////////SINA//////////////////////
struct KinematicSceneList Hugh1 = {
		16, 					// Number of scenes
		{{
		// ==== SCENE 1====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   -6,   0,  96,   0},	// left  leg vector + yaw
		{   -6,   0,  96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 2====
	    550,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   -6, 32,  96,   0},	// left  leg vector + yaw
		{   -6, 32,  96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 3====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   -6, 32,  110,   0},	// left  leg vector + yaw
		{   -6, 32,  96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 4====
		150,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  -6,  32,  104,   0},	// left  leg vector + yaw
		{   0,  32,   90,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 5====
		150,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   -6,  32,  104,   0},// left  leg vector + yaw
		{   28,  32,  86,   0}	// right leg vector + yaw
				// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 6====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   -6,  32,  100,   0},// left  leg vector + yaw
		{   28,  32,  100,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 7====
		550,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   -6,  0,  100,   0},	// left  leg vector + yaw
		{   28,  0,  100,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 8====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   0,   0, 96,  0},	// left  leg vector + yaw
		{   0,   0, 96,  0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 9====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  -6,  0,  96,   0},	// left  leg vector + yaw
		{  -6,  0,  96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 10====
		550,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  -6,  -32,  96,   0},	// left  leg vector + yaw
		{  -6,  -32,  96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 11====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  -6, -32, 96,  0},	// left  leg vector + yaw
		{  -6, -32, 110,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 12====
		150,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   0,  -32, 90,  0},	// left  leg vector + yaw
		{   -6, -32, 104,   0}	// right leg vector + yaw
				// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 13====
		150,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   28, -32, 90,  0},	// left  leg vector + yaw
		{   -6, -32, 104,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 14====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   28, -32, 100,  0},	// left  leg vector + yaw
		{   -6,  -32, 100,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 15====
		550,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   28,   0, 100,   0},	// left  leg vector + yaw
		{   -6,   0, 100,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 16====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   0,   0, 96,   0},	// left  leg vector + yaw
		{   0,   0, 96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}}
};

struct KinematicSceneList SideWalk = {
		16, 					// Number of scenes
		{{
		// ==== SCENE 1====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   0,   0,  96,   0},	// left  leg vector + yaw
		{   0,   0,  96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 2====
	    550,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   0, 20,  96,   0},	// left  leg vector + yaw
		{   0, 0,  90,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 3====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   0, 20,  96,   0},	// left  leg vector + yaw
		{   0, -10,  90,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 4====
		150,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  0,  10,  96,   0},	// left  leg vector + yaw
		{   0,  -10,  96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 5====
		150,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   0,  0,  96,   0},// left  leg vector + yaw
		{   0,  -10,  96,   0}	// right leg vector + yaw
				// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 6====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   0,  0,  96,   0},// left  leg vector + yaw
		{   0,  0,  96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 7====
		550,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   -6,  0,  100,   0},	// left  leg vector + yaw
		{   28,  0,  100,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 8====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   0,   0, 96,  0},	// left  leg vector + yaw
		{   0,   0, 96,  0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 9====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  -6,  0,  96,   0},	// left  leg vector + yaw
		{  -6,  0,  96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 10====
		550,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  -6,  -32,  96,   0},	// left  leg vector + yaw
		{  -6,  -32,  96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 11====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{  -6, -32, 96,  0},	// left  leg vector + yaw
		{  -6, -32, 110,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 12====
		150,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   0,  -32, 90,  0},	// left  leg vector + yaw
		{   -6, -32, 104,   0}	// right leg vector + yaw
				// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 13====
		150,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   28, -32, 90,  0},	// left  leg vector + yaw
		{   -6, -32, 104,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 14====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   28, -32, 100,  0},	// left  leg vector + yaw
		{   -6,  -32, 100,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 15====
		550,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   28,   0, 100,   0},	// left  leg vector + yaw
		{   -6,   0, 100,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}, {
		// ==== SCENE 16====
		350,					// fadeInTime in ms
		0, {					// holdTime in ms
		{   0,   0,   0,   0},	// left  arm vector + yaw
		{   0,   0,   0,   0},	// right arm vector + yaw
		{   0,   0, 96,   0},	// left  leg vector + yaw
		{   0,   0, 96,   0}	// right leg vector + yaw
		// ==== END OF SCENE ====
		}}}
};

struct KinematicSceneList* kinematicscenes_motions[] = {
		&Walk_Forward, &Turn_Right, &Turn_Left, &Kick, &DoNothing, &Hugh1, &SideWalk
};

#endif /* KINEMATIC_SCENE_DATA_H_ */
