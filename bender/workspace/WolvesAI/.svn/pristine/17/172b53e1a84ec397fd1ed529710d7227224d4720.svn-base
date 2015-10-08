/*
 * ImageService.cpp
 *
 *  Created on: 25.01.2010
 *      Author: Stefan
 */

#include <iostream>
#include <stdlib.h>
#include "ImageService.h"
#include "../Debugger.h"
#include "../../Modules/ImageSources/PBImage.h"
#include "../../ModuleManager/ModuleManager.h"
#include "../../Blackboard/Blackboard.h"
#include "../../Blackboard/DataHolder.h"
#include "../../Representations/BodyStatus.h"

#ifdef WIN32
	#define IMAGE_PATH "images/"
#else
	#define IMAGE_PATH "/mnt/sdcard/"
#endif

ImageService::ImageService() {
	mPBFLoader = NULL;
}

ImageService::~ImageService() {
	mPBFLoader = NULL;
}

void ImageService::setPBFLoader( PBFLoader *loader) {
	mPBFLoader = loader;
}

void ImageService::newPacket(uint8_t* packet, size_t size) {
	string request = (char*) (packet);
	Debugger::DEBUG("ImageService", "New packet: %s", request.c_str());

	if (request.substr((size_t)0, (size_t)4) == "GET ") {
		if (request.substr((size_t)13, (size_t)4) == "YUV ") {
			string fileName = request.substr((size_t)19, request.length() - (size_t)19).c_str();
			fileName = IMAGE_PATH + fileName + ".pbf";

			ModuleManager::GetInstance()->pauseExecutionAfterIteration();

			Blackboard* bb = ModuleManager::GetInstance()->getBlackboardDirect(true);
			if (bb != NULL) {
				DataHolder<YUVImage>& rep = bb->getRepresentation<DataHolder<YUVImage> >("Image");
				DataHolder<BodyStatus>& bodyStatus = bb->getRepresentation<DataHolder<BodyStatus> >("BodyStatus");
				PBImage::writePBI(fileName, (*rep), (*bodyStatus).getPan(), (*bodyStatus).getTilt());
				Debugger::INFO("ImageService", "Wrote PBF %s", fileName.c_str());
			}

			ModuleManager::GetInstance()->continueExecution();

			string ret = "READY " + fileName;
			Debugger::GetInstance()->sendPacket(getPacketID(), ret.c_str(), ret.size());
		}
	} else if (request.substr((size_t)0, (size_t)4) == "SET ") {

	} else if (request.substr((size_t)0, (size_t)5) == "LOAD ") {
		if( mPBFLoader != NULL) {
			if (request.substr((size_t)5, (size_t)4) == "PREV") {
				Debugger::DEBUG("ImageService", "Load prev");
				mPBFLoader->selectPrevImage();
			} else if (request.substr((size_t)5, (size_t)4) == "NEXT") {
				Debugger::DEBUG("ImageService", "Load next");
				mPBFLoader->selectNextImage();
			} else if (request.substr((size_t)5, (size_t)4) == "AUTO") {
				Debugger::DEBUG("ImageService", "Load auto");
				mPBFLoader->setAutoIterate( true);
			} else if (request.substr((size_t)5, (size_t)4) == "STOP") {
				Debugger::DEBUG("ImageService", "Load stop");
				mPBFLoader->setAutoIterate( false);
			}
		}
	}
}

string ImageService::getPacketID() {
	return "IMG";
}
