/*
 * kinematic_scenes.h
 *
 *  Created on: 04.05.2011
 *      Author: Stefan
 */

#ifndef KINEMATIC_SCENES_H_
#define KINEMATIC_SCENES_H_

typedef void (*SceneDoneCallback)(void);

void KinematicScenes_Init(void);

void KinematicScenes_StartPlay(unsigned int motionID, unsigned short repetitions, unsigned char doNotSend);
unsigned char KinematicScenes_IsPlaying(void);
void KinematicScenes_CancelPlay(void);
SceneDoneCallback KinematicScenes_AssignSceneDoneCallback(SceneDoneCallback callback);
void KinematicScenes_SetTimingFactor(float factor);
float KinematicScenes_GetTimingFactor(void);


#endif /* KINEMATIC_SCENES_H_ */
