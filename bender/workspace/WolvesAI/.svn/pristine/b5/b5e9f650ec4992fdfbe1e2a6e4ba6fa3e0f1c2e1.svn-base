/*
 * CaptureSimulator.cpp
 *
 *  Created on: 17.07.2013
 *      Author: Stefan Krupop
 */

#include "CaptureSimulator.h"
#include "../../Debugging/Debugger.h"
#include "../../Blackboard/DataHolder.h"
#include "../../ModuleManager/ModuleManager.h"
#include "../../Body/SimulationCommunicator.h"
#include <malloc.h>
#include <string.h>

CaptureSimulator::CaptureSimulator() {
}

CaptureSimulator::~CaptureSimulator() {
}

bool CaptureSimulator::execute() {
	// Request new image
	SimulationCommunicator::GetInstance()->SendData(COMMAND, 0xf0, CONTROLLER_MOVEMENT, (size_t)0, NULL);

	int timeout = 10; // 10 * 100 ms = 1 s Timeout
	uint8_t* rgbImage;
	uint16_t imageWidth;
	uint16_t imageHeight;
	while ((rgbImage = SimulationCommunicator::GetInstance()->ReadImage(&imageWidth, &imageHeight)) == NULL) {
		timeout--;
		if (timeout <= 0) {
			Debugger::ERR("CaptureSimulator", "Timeout while waiting for image!");
			break;
		}
	}

	if(rgbImage){
		char* yuvImage = (char*)malloc((size_t)imageWidth * (size_t)imageHeight * (size_t)2);
		if (yuvImage != NULL) {
			// Wolves Simulator
			//ConvertRGB24BitToYUV444(rgbImage, yuvImage, (size_t)(IMAGE_HEIGHT * IMAGE_WIDTH));
			// WeBots
			ConvertBGR24BitToYUV444(rgbImage, yuvImage, (size_t)imageWidth * (size_t)imageHeight);
			getImage().copyImage(yuvImage, imageWidth, imageHeight);
			free(yuvImage);
		}
		free(rgbImage);
	}
	return true;
}

#define RGB2YUV(r, g, b, y, u, v)\
  y = (((( 66 * r + 129 * g) +  25 * b) + 128) >> 8) + 16;\
  u = ((((-38 * r -  74 * g) + 112 * b) + 128) >> 8) + 128;\
  v = ((((112 * r -  94 * g) -  18 * b) + 128) >> 8) + 128;\
  y = y < 0 ? 0 : y;\
  u = u < 0 ? 0 : u;\
  v = v < 0 ? 0 : v;\
  y = y > 255 ? 255 : y;\
  u = u > 255 ? 255 : u;\
  v = v > 255 ? 255 : v

void CaptureSimulator::ConvertBGR24BitToYUV444(const uint8_t *BGR, char *YUV, size_t pixels) const {
	size_t i, j;
	size_t max = (pixels + (pixels << 1));
	for (i = 0, j = 0; i < max; i += 6, j += 4) {
		int y0, u0, v0, y1, u1, v1;
		int r, g, b;
		//lint --e{702} Shift of signed quantity
		b = (uint8_t) BGR[i];
		g = (uint8_t) BGR[i + 1];
		r = (uint8_t) BGR[i + 2];
		RGB2YUV(r, g, b, y0, u0, v0);
		b = (uint8_t) BGR[i + 3];
		g = (uint8_t) BGR[i + 4];
		r = (uint8_t) BGR[i + 5];
		RGB2YUV(r, g, b, y1, u1, v1);
		YUV[j] = (char)y0;
		YUV[j + 1] = (char)((v0 + v1) >> 1);
		YUV[j + 2] = (char)y1;
		YUV[j + 3] = (char)((u0 + u1) >> 1);
	}
} //lint !e429
