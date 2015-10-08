/*
 * kinematic_scenes.c
 *
 *  Created on: 04.05.2010
 *      Author: Stefan
 */

#include "kinematic_scenes.h"
#include "kinematics/inverse.h"
#include "tc/tc.h"
#include "dbgu/dbgu.h"
#include <stdlib.h>

struct vect3d_yaw {
	signed short x;
	signed short y;
	signed short z;
	signed short yaw;
};

struct BodyPose {
	struct vect3d_yaw leftArm;
	struct vect3d_yaw rightArm;
	struct vect3d_yaw leftLeg;
	struct vect3d_yaw rightLeg;
};

struct KinematicScene {
	unsigned short fadeInTime;
	unsigned short holdTime;
	struct BodyPose pose;
};

struct KinematicSceneList {
	unsigned short numberOfScenes;
	struct KinematicScene scenes[];
};

struct Bresenham3DParameters {
	struct vect3d_yaw currentPos;
	struct vect3d_yaw stepSize;
	struct vect3d_yaw delta;
	unsigned char swap_xy;
	unsigned char swap_xz;
	unsigned char swap_xyaw;
	signed short drift_xy;
	signed short drift_xz;
	signed short drift_xyaw;
	signed short targetX;
	unsigned char enabled;
};

#include "kinematic_scene_data.h"

volatile unsigned char kinematicscenes_playing;
struct KinematicSceneList* kinematicscenes_currentMotion;
unsigned short kinematicscenes_currentSceneInMotion;
unsigned short kinematicscenes_currentSceneSteps;
unsigned short kinematicscenes_currentSceneCurStep;
struct Bresenham3DParameters kinematicscenes_paramsLeftLeg;
struct Bresenham3DParameters kinematicscenes_paramsRightLeg;
struct Bresenham3DParameters kinematicscenes_paramsLeftArm;
struct Bresenham3DParameters kinematicscenes_paramsRightArm;
SceneDoneCallback kinematicscenes_SceneDoneCallbackFnct = 0;
unsigned int kinematicscenes_currentMotionID;
unsigned short kinematicscenes_repetitions;
unsigned char kinematicscenes_doNotSend;
float kinematicscenes_timingFactor = 1.0;

void KinematicScenes_StartSceneStep(struct BodyPose* start, struct BodyPose* end);
void KinematicScenes_CalculateBresenhamSteps(struct vect3d_yaw from, struct vect3d_yaw to, struct Bresenham3DParameters* data);
unsigned char KinematicScenes_DoBresenhamStep(enum body_extremity ext, struct Bresenham3DParameters* data);

void KinematicScenes_Init(void) {
	kinematicscenes_playing = 0;
	kinematicscenes_timingFactor = 1.0;
}

void KinematicScenes_PlayCallback(void) {
	unsigned char ret;
	unsigned char done = 1;

	kinematicscenes_currentSceneCurStep++;

    //step through longest delta (which we have swapped to x)
	inverse_startCycle();
	ret = KinematicScenes_DoBresenhamStep(LEG_LEFT, &kinematicscenes_paramsLeftLeg);
   	if (ret) { done = 0; }
   	ret = KinematicScenes_DoBresenhamStep(LEG_RIGHT, &kinematicscenes_paramsRightLeg);
   	if (ret) { done = 0; }
   	ret = KinematicScenes_DoBresenhamStep(ARM_LEFT, &kinematicscenes_paramsLeftArm);
   	if (ret) { done = 0; }
   	ret = KinematicScenes_DoBresenhamStep(ARM_RIGHT, &kinematicscenes_paramsRightArm);
   	if (ret) { done = 0; }
   	inverse_endCycle(kinematicscenes_doNotSend);

   	if (done) {
   		if (kinematicscenes_currentSceneCurStep >= kinematicscenes_currentSceneSteps) {
   			kinematicscenes_currentSceneInMotion++;
   			if (kinematicscenes_currentSceneInMotion < kinematicscenes_currentMotion->numberOfScenes) {
   				// More scenes, start next one
   				KinematicScenes_StartSceneStep(&kinematicscenes_currentMotion->scenes[kinematicscenes_currentSceneInMotion - 1].pose, &kinematicscenes_currentMotion->scenes[kinematicscenes_currentSceneInMotion].pose);
   			} else {
   				// No more scenes, stop playback
   				TC_AssignTimer1Callback(0); // Disable timer callback
   				kinematicscenes_playing = 0;
   				kinematicscenes_repetitions--;
   				if (kinematicscenes_repetitions > 0) {
   					KinematicScenes_StartPlay(kinematicscenes_currentMotionID, kinematicscenes_repetitions, kinematicscenes_doNotSend);
   				} else {
					if (kinematicscenes_SceneDoneCallbackFnct != 0) {
						kinematicscenes_SceneDoneCallbackFnct();
					}
   				}
   			}
   		}
   	}
}

void KinematicScenes_StartSceneStep(struct BodyPose* start, struct BodyPose* end) {
	KinematicScenes_CalculateBresenhamSteps(start->leftLeg, end->leftLeg, &kinematicscenes_paramsLeftLeg);
	KinematicScenes_CalculateBresenhamSteps(start->rightLeg, end->rightLeg, &kinematicscenes_paramsRightLeg);
	KinematicScenes_CalculateBresenhamSteps(start->leftArm, end->leftArm, &kinematicscenes_paramsLeftArm);
	KinematicScenes_CalculateBresenhamSteps(start->rightArm, end->rightArm, &kinematicscenes_paramsRightArm);

	unsigned short maxdist = 0;
	if (abs(kinematicscenes_paramsLeftLeg.delta.x) > maxdist && kinematicscenes_paramsLeftLeg.enabled) { maxdist = abs(kinematicscenes_paramsLeftLeg.delta.x); }
	if (abs(kinematicscenes_paramsRightLeg.delta.x) > maxdist && kinematicscenes_paramsRightLeg.enabled) { maxdist = abs(kinematicscenes_paramsRightLeg.delta.x); }
	if (abs(kinematicscenes_paramsLeftArm.delta.x) > maxdist && kinematicscenes_paramsLeftArm.enabled) { maxdist = abs(kinematicscenes_paramsLeftArm.delta.x); }
	if (abs(kinematicscenes_paramsRightArm.delta.x) > maxdist && kinematicscenes_paramsRightArm.enabled) { maxdist = abs(kinematicscenes_paramsRightArm.delta.x); }

	unsigned short fadeTime = kinematicscenes_currentMotion->scenes[kinematicscenes_currentSceneInMotion].fadeInTime;
	fadeTime = fadeTime * kinematicscenes_timingFactor;
	int interval = fadeTime;
	if (maxdist != 0) {
		interval = fadeTime / maxdist; // Set the interrupt for the frames
	}
	unsigned short holdTime = kinematicscenes_currentMotion->scenes[kinematicscenes_currentSceneInMotion].holdTime;
	holdTime = holdTime * kinematicscenes_timingFactor;
	kinematicscenes_currentSceneSteps = (fadeTime + holdTime) / interval;
	kinematicscenes_currentSceneCurStep = 0;
//	dbgu_printf("[kinematic_scenes] StartSceneStep: Max distance = %d, time = %d ms => interval = %d ms; %d steps overall\r\n", maxdist, fadeTime, interval, kinematicscenes_currentSceneSteps);
	TC_SetTimer1Interval(interval);
}

void KinematicScenes_StartPlay(unsigned int motionID, unsigned short repetitions, unsigned char doNotSend) {
	kinematicscenes_playing = 1;
	kinematicscenes_repetitions = repetitions;
	kinematicscenes_doNotSend = doNotSend;

	// Determine current pose
	// Currently fixed, should better be calculated from current
	// servo values with forward kinematics
	// TODO: Implement and use forward kinematics
	struct BodyPose current;
	struct vect3d_yaw* cur;
	struct vect3d cur2;
	cur = &current.leftArm;
	cur2 = inverse_getCurrentPos(ARM_LEFT);
	cur->x = cur2.x; cur->y = cur2.y; cur->z = cur2.z; cur->yaw = inverse_getCurrentPosYaw(ARM_LEFT);
	cur = &current.rightArm;
	cur2 = inverse_getCurrentPos(ARM_RIGHT);
	cur->x = cur2.x; cur->y = cur2.y; cur->z = cur2.z; cur->yaw = inverse_getCurrentPosYaw(ARM_RIGHT);
	cur = &current.leftLeg;
	cur2 = inverse_getCurrentPos(LEG_LEFT);
	cur->x = cur2.x; cur->y = cur2.y; cur->z = cur2.z; cur->yaw = inverse_getCurrentPosYaw(LEG_LEFT);
	if (cur->z == 0) cur->z = 100;
	cur = &current.rightLeg;
	cur2 = inverse_getCurrentPos(LEG_RIGHT);
	cur->x = cur2.x; cur->y = cur2.y; cur->z = cur2.z; cur->yaw = inverse_getCurrentPosYaw(LEG_RIGHT);
	if (cur->z == 0) cur->z = 100;

	kinematicscenes_currentMotionID = motionID;
	kinematicscenes_currentMotion = kinematicscenes_motions[motionID];
	kinematicscenes_currentSceneInMotion = 0;

	KinematicScenes_StartSceneStep(&current, &kinematicscenes_currentMotion->scenes[kinematicscenes_currentSceneInMotion].pose);

	inverse_startCycle();
   	KinematicScenes_DoBresenhamStep(LEG_LEFT, &kinematicscenes_paramsLeftLeg);
   	KinematicScenes_DoBresenhamStep(LEG_RIGHT, &kinematicscenes_paramsRightLeg);
   	KinematicScenes_DoBresenhamStep(ARM_LEFT, &kinematicscenes_paramsLeftArm);
   	KinematicScenes_DoBresenhamStep(ARM_RIGHT, &kinematicscenes_paramsRightArm);
   	inverse_endCycle(kinematicscenes_doNotSend);

	TC_AssignTimer1Callback(KinematicScenes_PlayCallback);
}

void KinematicScenes_Swap(signed short* one, signed short* two) {
	signed short temp = *one;
	*one = *two;
	*two = temp;
}

void KinematicScenes_CalculateBresenhamSteps(struct vect3d_yaw from, struct vect3d_yaw to, struct Bresenham3DParameters* data) {
	// Uses Bresenham's algorithm adopted for 3D (and extended to include yaw) as on this page:
	// http://www.cobrabytes.com/index.php?topic=1150.0

//	dbgu_printf("[kinematic_scenes] CalculateMovement: Moving from %d, %d, %d (yaw %d) to %d, %d, %d (yaw %d)\r\n", from.x, from.y, from.z, from.yaw, to.x, to.y, to.z, to.yaw);

	if (to.x == 0 && to.y == 0 && to.z == 0 && to.yaw == 0) {
		data->enabled = 0;
	} else {
		data->enabled = 1;
	}

    // 'steep' xy Line, make longest delta x plane
    data->swap_xy = (abs(to.y - from.y) > abs(to.x - from.x));
    if (data->swap_xy) {
    	KinematicScenes_Swap(&from.x, &from.y);
		KinematicScenes_Swap(&to.x, &to.y);
    }

    // do same for xz
    data->swap_xz = (abs(to.z - from.z) > abs(to.x - from.x));
    if (data->swap_xz) {
    	KinematicScenes_Swap(&from.x, &from.z);
		KinematicScenes_Swap(&to.x, &to.z);
    }

    // do same for xyaw
    data->swap_xyaw = (abs(to.yaw - from.yaw) > abs(to.x - from.x));
    if (data->swap_xyaw) {
    	KinematicScenes_Swap(&from.x, &from.yaw);
		KinematicScenes_Swap(&to.x, &to.yaw);
    }

    //delta is Length in each plane
    data->delta.x = abs(to.x - from.x);
    data->delta.y = abs(to.y - from.y);
    data->delta.z = abs(to.z - from.z);
    data->delta.yaw = abs(to.yaw - from.yaw);

    //drift controls when to step in 'shallow' planes
    //starting value keeps Line centered
    data->drift_xy = (data->delta.x / 2);
    data->drift_xz = (data->delta.x / 2);
    data->drift_xyaw = (data->delta.x / 2);

    //direction of line
    data->stepSize.x = 1;  if (from.x > to.x) { data->stepSize.x = -1; }
    data->stepSize.y = 1;  if (from.y > to.y) { data->stepSize.y = -1; }
    data->stepSize.z = 1;  if (from.z > to.z) { data->stepSize.z = -1; }
    data->stepSize.yaw = 1;  if (from.yaw > to.yaw) { data->stepSize.yaw = -1; }

    //starting point
    data->currentPos.x = from.x;
    data->currentPos.y = from.y;
    data->currentPos.z = from.z;
    data->currentPos.yaw = from.yaw;
    data->targetX = to.x;

    // Do first step because we're already at the starting position
    // Description is in KinematicScenes_DoBresenhamStep
    data->drift_xy -= data->delta.y;
    data->drift_xz -= data->delta.z;
    data->drift_xyaw -= data->delta.yaw;
    if (data->drift_xy < 0) {
    	data->currentPos.y += data->stepSize.y;
        data->drift_xy += data->delta.x;
    }
    if (data->drift_xz < 0) {
    	data->currentPos.z += data->stepSize.z;
        data->drift_xz += data->delta.x;
    }
    if (data->drift_xyaw < 0) {
    	data->currentPos.yaw += data->stepSize.yaw;
        data->drift_xyaw += data->delta.x;
    }
    data->currentPos.x += data->stepSize.x;
}

unsigned char KinematicScenes_DoBresenhamStep(enum body_extremity ext, struct Bresenham3DParameters* data) {
 	if (!data->enabled) return 0;
	if (!(((data->currentPos.x <= data->targetX) && data->stepSize.x > 0) || ((data->currentPos.x >= data->targetX) && data->stepSize.x < 0))) {
		return 0;
	}

	//copy position
	struct vect3d_ext c;
    c.x = data->currentPos.x;
    c.y = data->currentPos.y;
    c.z = data->currentPos.z;
    c.yaw = data->currentPos.yaw;

    //unswap (in reverse)
    if (data->swap_xyaw) { KinematicScenes_Swap(&c.x, &c.yaw); }
    if (data->swap_xz) { KinematicScenes_Swap(&c.x, &c.z); }
    if (data->swap_xy) { KinematicScenes_Swap(&c.x, &c.y); }

    //passes through this point
    //dbgu_printf("[kinematic_scenes] CalculateMovement: New point %d, %d, %d (yaw %d) for ext %d\r\n", c.x, c.y, c.z, cyaw, ext);
    inverse_calc(ext, &c);

    //update progress in other planes
    data->drift_xy -= data->delta.y;
    data->drift_xz -= data->delta.z;
    data->drift_xyaw -= data->delta.yaw;

    //step in y plane
    if (data->drift_xy < 0) {
    	data->currentPos.y += data->stepSize.y;
        data->drift_xy += data->delta.x;
    }

    //same in z
    if (data->drift_xz < 0) {
    	data->currentPos.z += data->stepSize.z;
        data->drift_xz += data->delta.x;
    }

    //same for yaw
    if (data->drift_xyaw < 0) {
    	data->currentPos.yaw += data->stepSize.yaw;
        data->drift_xyaw += data->delta.x;
    }

    data->currentPos.x += data->stepSize.x;
    return 1;
}

unsigned char KinematicScenes_IsPlaying(void) {
	return kinematicscenes_playing;
}

void KinematicScenes_CancelPlay(void) {
	if (kinematicscenes_currentMotion != 0) {
		kinematicscenes_currentSceneInMotion = kinematicscenes_currentMotion->numberOfScenes;
	}
}

SceneDoneCallback KinematicScenes_AssignSceneDoneCallback(SceneDoneCallback callback) {
	 SceneDoneCallback temp;

	 temp = kinematicscenes_SceneDoneCallbackFnct;
	 kinematicscenes_SceneDoneCallbackFnct = callback;
	 return temp;
}

void KinematicScenes_SetTimingFactor(float factor) {
	kinematicscenes_timingFactor = factor;
}

float KinematicScenes_GetTimingFactor(void) {
	return kinematicscenes_timingFactor;
}
