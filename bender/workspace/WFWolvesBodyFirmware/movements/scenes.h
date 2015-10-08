/*
 * scenes.h
 *
 *  Created on: 23.03.2010
 *      Author: Stefan
 */

#ifndef SCENES_H_
#define SCENES_H_

void Scenes_Init(void);

void Scenes_StartPlay(unsigned int motionID);
unsigned char Scenes_IsPlaying(void);

void Scenes_MoveToZeroPosition(void);

unsigned char Scenes_GetServoCount(void);
unsigned char Scenes_GetZeroPosition(unsigned char servoID);

#endif /* SCENES_H_ */
