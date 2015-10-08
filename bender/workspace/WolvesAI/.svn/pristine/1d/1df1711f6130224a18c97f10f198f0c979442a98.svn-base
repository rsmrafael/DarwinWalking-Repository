/*
 * PBFLoader.cpp
 *
 *  Created on: 23.08.2012
 *      Author: Stefan Krupop
 */

#include "PBFLoader.h"
#include "PBImage.h"
#include "../../ModuleManager/ModuleManager.h"
#include "../../Blackboard/DataHolder.h"
#include <dirent.h>
#include <cstring>

PBFLoader::PBFLoader(const struct pbfData *data) {
	mData.pbfPath = data->pbfPath;
	mData.iterationDirection = data->iterationDirection;
	mData.repeatCount = data->repeatCount;
	mData.repeatList = data->repeatList;
	mData.panTiltFromPBF = data->panTiltFromPBF;
	mData.autoIterate = data->autoIterate;
	mFileNames = new list<string>;
	mCurrentRepetition = -1;

	if (mData.pbfPath != "") {
		Debugger::INFO("PBFLoader", "Loading from PBF: %s", mData.pbfPath.c_str());

		if (strcmp(mData.pbfPath.substr(mData.pbfPath.size() - 3, (size_t)3).c_str(), "pbf") == 0) { // Single File
			mFileNames->push_back(mData.pbfPath);
			mFileNameIterator = mFileNames->begin();
		} else { // Complete Folder
			if (mData.pbfPath.substr(mData.pbfPath.size() - 1, (size_t)1) != "/" && mData.pbfPath.substr(mData.pbfPath.size() - 1, (size_t)1) != "\\") {
				mData.pbfPath += "/";
			}

			DIR *maindir = opendir(mData.pbfPath.c_str());
			if (maindir != NULL) {
				struct dirent *entry;
				do {
					entry = readdir(maindir);
					if (entry != NULL) {
						string fullPath = mData.pbfPath + entry->d_name;
						if (strcmp(fullPath.substr(fullPath.size() - 3, (size_t)3).c_str(), "pbf") == 0) {
							Debugger::DEBUG("PBFLoader", "New File: %s", fullPath.c_str());
							mFileNames->push_back(fullPath);
						}
					}
				} while (entry);
				closedir(maindir);

				mFileNameIterator = mFileNames->begin();
			}
		}
	}
}

PBFLoader::~PBFLoader() {
	delete mFileNames;
}

bool PBFLoader::execute() {

	if (mFileNames->size() == (size_t)0) {
		return false;
	}

	if (mData.autoIterate) {
		if( mData.repeatCount > 0) {
			mCurrentRepetition++;
			if (mCurrentRepetition >= mData.repeatCount) {
				if( !selectNextImage()) {
					ModuleManager::GetInstance()->requestShutdown();
					return false;
				}
			}
		}
	}

	PBImage* pbImage = new PBImage(*mFileNameIterator);
	YUVImage* image = pbImage->getImage(0);
	//YUVImage* imgRight = pbImage->getImage(1);
	int pan = 0, tilt = 0;
	if (mData.panTiltFromPBF && pbImage->getPanTilt(0, &pan, &tilt)) {
		// TODO: Find cleaner solution for providing pan/tilt from PBFs
		Blackboard* bb = ModuleManager::GetInstance()->getBlackboardDirect(true);
		if (bb != NULL) {
			DataHolder<BodyStatus>& bodyStatus = bb->getRepresentation<DataHolder<BodyStatus> >("BodyStatus");
			(*bodyStatus).setPanTilt(pan, tilt);
		}
	}

	getAnnotations().clearList();
	getAnnotations().addObjects(pbImage->getAnnotations(0));
	getImage().copyImage(image->getImage(), image->getWidth(), image->getHeight());

	delete pbImage;
	delete image;

	return true;
}

void PBFLoader::setAutoIterate( bool autoIterate) {
	mData.autoIterate = autoIterate;
}

bool PBFLoader::selectPrevImage() {

	if (mFileNameIterator == mFileNames->begin()) {
		if (mData.repeatList) {
			mFileNameIterator = mFileNames->end();
			--mFileNameIterator;
		} else {
			return false;
		}
	} else {
		--mFileNameIterator;
	}
	mCurrentRepetition = 0;
	return true;
}

bool PBFLoader::selectNextImage() {
	++mFileNameIterator;

	if (mFileNameIterator == mFileNames->end()) {
		if (mData.repeatList) {
			mFileNameIterator = mFileNames->begin();
		} else {
			return false;
		}
	}
	mCurrentRepetition = 0;
	return true;
}
