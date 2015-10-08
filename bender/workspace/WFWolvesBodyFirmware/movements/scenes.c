/*
 * scenes.c
 *
 *  Created on: 23.03.2010
 *      Author: Stefan
 */

#include "scenes.h"
#include "servo/servo.h"
#include "tc/tc.h"
#include "movements/motionassigns.h"

#define MAX_wCK 31

// Data structure for the Motion Data
//      - hierarchy is : wCK < Frame < Scene < Motion < Action
//      - data is sent to the wCK for each frame on the timer
//      - frames are created from scenes by interpolation
//      - a motion comprises a sequence of scenes
//      - an action can invoke motions

struct TwCK_in_Motion {      // Structure for each wCK in a motion
	char Exist;				// 1 if wCK exists
	char RPgain;			// Runtime P setting
	char RDgain;			// Runtime D setting
	char RIgain;			// Runtime I setting
	char PortEn;			// (0 = disable, 1 = enable)
	char InitPos;			// Initial wCK position
};

struct TwCK_in_Scene {		// Structure for each wCK in a scene
	char Exist;				// 1 if wCK exists
	char SPos;				// wCK starting position
	char DPos;				// wCK destination position
	char Torq;				// Torque
	char ExPortD;			// External Port Data(1~3)
};

struct TMotion {			// Structure for a motion
	char PF;				//  ?
	char RIdx;				//  ?
	int	AIdx;				//  ?
	int	NumOfScenes;		// number of scenes in motion
	int	NumOfwCK;			// number of wCK included
	struct TwCK_in_Motion wCK[MAX_wCK];	// Motion data for each wCK
	int	FileSize;			// overall file size
};

struct TScene {				// Structure for a scene
	int Idx;				// index of scene (0~65535)
	int NumOfFrames;		// number of frames in scene
	int RunTime;			// running time of scene[msec]
	struct TwCK_in_Scene wCK[MAX_wCK];	// scene data for each wCK
};

#define MAX_SCENES		20
#define MAX_LEN_NAME	9

/*
struct EEPROMMotionData {
	unsigned char version;					// 1
	char name[MAX_LEN_NAME + 1];			// 10
	unsigned char PT[MAX_wCK];				// 31
	unsigned char DT[MAX_wCK];				// 31
	unsigned char IT[MAX_wCK];				// 31
	unsigned short NumOfFrames[MAX_SCENES];	// 40
	unsigned short RunTime[MAX_SCENES];		// 40
	unsigned char PoT[MAX_SCENES][MAX_wCK]; // 620
	unsigned char TT[MAX_SCENES][MAX_wCK];	// 620
	unsigned char ET[MAX_SCENES][MAX_wCK];	// 620
	short NumberOfScenes;					// 2
	short NumberOfwCK;						// 2
};
*/											// = 2048

// Prototypes
struct TMotion Scenes_GetMotion(struct MotionData* motionData);
struct TScene Scenes_GetScene(struct MotionData* motionData, unsigned int sceneID);
void Scenes_CalcInterpolationDistance(struct TScene* scene, float* interpolationDistanceArray);
int Scenes_CalcFrameInterval(struct TScene* scene);
void Scenes_SendTGain(struct TMotion* motion);
void Scenes_SendExPortD(struct TScene* scene);
void Scenes_SendFrame(struct TScene* scene, int frameIndex, float* interpolationDistanceArray);

struct TScene scenes_currentScene;
struct TMotion scenes_currentMotion;
unsigned int scenes_currentSceneIndex;
unsigned int scenes_currentFrameIndex;
float scenes_interpolationDistance[MAX_wCK];
volatile unsigned char scenes_playing;
struct MotionData* scenes_currentMotionPage;

void Scenes_Init(void) {
	scenes_playing = 0;
}

//------------------------------------------------------------------------------
// Fill the motion data structure
//------------------------------------------------------------------------------
struct TMotion Scenes_GetMotion(struct MotionData* motionData) {
	int i;
	struct TMotion motion;

	motion.NumOfScenes 	= motionData->NumberOfScenes;
	motion.NumOfwCK 	= motionData->NumberOfwCK;

	for (i = 0; i < MAX_wCK; i++) { // clear the wCK motion data
		motion.wCK[i].Exist = 0;
		motion.wCK[i].RPgain = 0;
		motion.wCK[i].RDgain = 0;
		motion.wCK[i].RIgain = 0;
		motion.wCK[i].PortEn = 0;
		motion.wCK[i].InitPos = 0;
	}

	for (i = 0; i < motion.NumOfwCK; i++) { // fill the wCK motion data
		motion.wCK[wCK_IDs[i]].Exist = 1;
		motion.wCK[wCK_IDs[i]].RPgain = motionData->PT[i];
		motion.wCK[wCK_IDs[i]].RDgain = motionData->DT[i];
		motion.wCK[wCK_IDs[i]].RIgain = motionData->IT[i];
		motion.wCK[wCK_IDs[i]].PortEn = 1;
		motion.wCK[wCK_IDs[i]].InitPos = MotionZeroPos[i];
	}
	return motion;
}

//------------------------------------------------------------------------------
// Fill the scene data structure
//------------------------------------------------------------------------------
struct TScene Scenes_GetScene(struct MotionData* motionData, unsigned int sceneID) {
	int i;
	struct TScene scene;

	int numOfwCK = motionData->NumberOfwCK;

	scene.NumOfFrames = motionData->NumOfFrames[sceneID]; // get the number of frames in scene
	scene.RunTime = motionData->RunTime[sceneID]; // get the run time of scene[msec]

	for (i = 0; i < MAX_wCK; i++) { // clear the data for the wCK in scene
		scene.wCK[i].Exist = 0;
		scene.wCK[i].SPos = 0;
		scene.wCK[i].DPos = 0;
		scene.wCK[i].Torq = 0;
		scene.wCK[i].ExPortD = 0;
	}

	for (i = 0; i < numOfwCK; i++) { // get the flash data for the wCK
		scene.wCK[wCK_IDs[i]].Exist = 1;
		scene.wCK[wCK_IDs[i]].SPos = ((unsigned char*) motionData->PoT)[numOfwCK * sceneID + i];
		scene.wCK[wCK_IDs[i]].DPos = ((unsigned char*) motionData->PoT)[numOfwCK * (sceneID + 1) + i];
		scene.wCK[wCK_IDs[i]].Torq = ((unsigned char*) motionData->TT)[numOfwCK * sceneID + i];
		scene.wCK[wCK_IDs[i]].ExPortD = ((unsigned char*) motionData->ET)[numOfwCK * sceneID + i];
	}

	return scene;
}

//------------------------------------------------------------------------------
// Calculate the interpolation steps
// gUnitD[] is in range -254 to +254
//------------------------------------------------------------------------------
void Scenes_CalcInterpolationDistance(struct TScene* scene, float* interpolationDistanceArray) {
	int i;

	for (i = 0; i < MAX_wCK; i++) {
		if (scene->wCK[i].Exist) { // if the wCK exists
			float val;
			if (scene->wCK[i].SPos != scene->wCK[i].DPos) {
				// if any movement is required
				val = (float)((int)scene->wCK[i].DPos - (int)scene->wCK[i].SPos);
				val = (float)(val / scene->NumOfFrames);
				if (val > 253) {
					val = 254;
				} else if (val < -253) {
					val = -254;
				}
			} else {
				val = 0;
			}
			interpolationDistanceArray[i] = val;
		}
	}
}

//------------------------------------------------------------------------------
// Return the time between to frames based on the number of frames and the run time of the scene
//------------------------------------------------------------------------------
int Scenes_CalcFrameInterval(struct TScene* scene) {
	float tmp;

	tmp = (float)scene->RunTime * 1;		// Timer  is clocked at 1KHz
	return (tmp / (float)scene->NumOfFrames);
}


//------------------------------------------------------------------------------
// Runtime P,D,I setting
//------------------------------------------------------------------------------
void Scenes_SendTGain(struct TMotion* motion) {
	for(int i = 0; i < MAX_wCK; i++) {					// Runtime P,D gain set from Motion structure
		if(motion->wCK[i].Exist)	{			// set P,D if wCK exists
			Servo_SendTGain(i, motion->wCK[i].RPgain, motion->wCK[i].RIgain, motion->wCK[i].RDgain);
		}
	}
}

//------------------------------------------------------------------------------
// Send external data for a scene
//------------------------------------------------------------------------------
void Scenes_SendExPortD(struct TScene* scene) {
	for(int i = 0; i < MAX_wCK; i++) {					// external data set from Motion structure
		if(scene->wCK[i].Exist) {				// set external data if wCK exists
			Servo_SendExPortD(i, scene->wCK[i].ExPortD);
		}
	}
}

void Scenes_SendFrame(struct TScene* scene, int frameIndex, float* interpolationDistanceArray) {
	int lwtmp;
	int i;

	Servo_SetPositionsTorque(scene->wCK[0].Torq);

	for (i = 0; i < MAX_wCK; i++) {	// for all wCK
		if (scene->wCK[i].Exist) {	// if wCK exists add the interpolation step
			lwtmp = (int)scene->wCK[i].SPos + (int)((float)frameIndex * interpolationDistanceArray[i]);
			if (lwtmp > 254) {		// bound result 1 to 254
				lwtmp = 254;
			} else if (lwtmp < 1) {
				lwtmp = 1;
			}
			Servo_SetPosition(i, (float)(lwtmp - 127) * 1.055);
		}
	}
	Servo_SendPositions();
}

void Scenes_PlayCallback(void) {
	if (scenes_currentFrameIndex < scenes_currentScene.NumOfFrames - 1) {
		scenes_currentFrameIndex++;
		Scenes_SendFrame(&scenes_currentScene, scenes_currentFrameIndex, &scenes_interpolationDistance[0]); // build new frame
	} else {
		if (scenes_currentSceneIndex < scenes_currentMotion.NumOfScenes - 1) {
			scenes_currentSceneIndex++;

			scenes_currentScene = Scenes_GetScene(scenes_currentMotionPage, scenes_currentSceneIndex);
			Scenes_SendExPortD(&scenes_currentScene); // Set external port data
			int interval = Scenes_CalcFrameInterval(&scenes_currentScene); // Set the interrupt for the frames
			Scenes_CalcInterpolationDistance(&scenes_currentScene, &scenes_interpolationDistance[0]); // Calculate the interpolation steps
			scenes_currentFrameIndex = 0; // reset frame to start of scene
			Scenes_SendFrame(&scenes_currentScene, scenes_currentFrameIndex, &scenes_interpolationDistance[0]); // build new frame
			TC_SetTimer1Interval(interval);
		} else {
			TC_AssignTimer1Callback(0); // Disable timer callback
			scenes_currentFrameIndex = 0;
			scenes_playing = 0;
		}
	}
}

void Scenes_StartPlay(unsigned int motionID) {
	scenes_currentMotionPage = &scenes_motionList[motionID];

	scenes_currentMotion = Scenes_GetMotion(scenes_currentMotionPage);
	Scenes_SendTGain(&scenes_currentMotion); // set the runtime P,D and I from motion structure
	scenes_currentSceneIndex = 0;
	scenes_playing = 1;

	scenes_currentScene = Scenes_GetScene(scenes_currentMotionPage, scenes_currentSceneIndex);
	Scenes_SendExPortD(&scenes_currentScene); // Set external port data
	int interval = Scenes_CalcFrameInterval(&scenes_currentScene); // Set the interrupt for the frames
	Scenes_CalcInterpolationDistance(&scenes_currentScene, &scenes_interpolationDistance[0]); // Calculate the interpolation steps
	scenes_currentFrameIndex = 0; // reset frame to start of scene
	Scenes_SendFrame(&scenes_currentScene, scenes_currentFrameIndex, &scenes_interpolationDistance[0]); // build new frame
	TC_SetTimer1Interval(interval);

	TC_AssignTimer1Callback(Scenes_PlayCallback);
}

unsigned char Scenes_IsPlaying(void) {
	return scenes_playing;
}

void Scenes_MoveToZeroPosition(void) {
	Servo_SetPositionsTorque(0); // Maximum torque
	for (unsigned char i = 0; i < sizeof(wCK_IDs); i++) {
		Servo_SetPosition(i, (float)(MotionZeroPos[i] - 127) * 1.055);
	}
	Servo_SendPositions();
}

unsigned char Scenes_GetServoCount(void) {
	return sizeof(wCK_IDs);
}

unsigned char Scenes_GetZeroPosition(unsigned char servoID) {
	return MotionZeroPos[servoID];
}
