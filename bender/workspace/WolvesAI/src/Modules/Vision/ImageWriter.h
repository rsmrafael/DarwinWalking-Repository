/*
 * ImageWriter.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef ImageWriter_H_
#define ImageWriter_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/BodyStatus.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(ImageWriter)
	REQUIRE(YUVImage,Image)
	REQUIRE(BodyStatus,BodyStatus)
END_DECLARE_MODULE(ImageWriter)

/**
 * ImageWriter creates image files every x frame
 */
class ImageWriter: public ImageWriterBase, public ConfigChangeListener {
public:
	ImageWriter();
	virtual ~ImageWriter();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

	/**
	 * config has changed
	 * @see ConfigChangeListener::configChanged
	 */
	virtual void configChanged();

private:
	string mPath;
	string mDate;
	int mCounter;
	int mImageCount;
	bool mUseImageWriter;

};

#endif /* ImageWriter_H_ */
