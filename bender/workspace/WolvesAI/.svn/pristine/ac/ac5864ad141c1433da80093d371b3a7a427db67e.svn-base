/*
 * ImageWriter.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#include "ImageWriter.h"
#include "../ImageSources/PBImage.h"
#include <time.h>
#include "../../Config.h"

ImageWriter::ImageWriter()
:	mPath("/mnt/sdcard/images/"),
	mDate(""),
	mCounter(0),
	mImageCount(30),
	mUseImageWriter(false)
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, (size_t) 80, "%Y.%m.%d-%H-%M-%S", timeinfo);
	mDate = string(buffer);

	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

ImageWriter::~ImageWriter() {
	Config::getInstance()->removeConfigChangeHandler(this);
}

bool ImageWriter::execute() {

	if (mUseImageWriter) {
		if (mCounter % mImageCount == 0) {
			char buffer[8];

			sprintf(&buffer[0], "%06i", mCounter);

			string fileName = mPath + mDate + "_" + string(buffer) + ".pbf";

			cout << fileName << endl;

			PBImage::writePBI(fileName, getImage(), getBodyStatus().getPan(),
					getBodyStatus().getTilt());

		}

		++mCounter;
	}

	return true;
}

void ImageWriter::configChanged() {
	Debugger::INFO("ImageWriter", "config changed");

	ConfigFile *config = Config::getInstance();
	mImageCount = config->get<int>("Modules", "ImageWriter_ImageCount", 30);
	mUseImageWriter = config->get<bool>("Modules", "useImageWriter", false);
}
