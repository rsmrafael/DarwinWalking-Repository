/*
 * PBFLoader.h
 *
 *  Created on: 23.08.2012
 *      Author: Stefan Krupop
 */

#ifndef PBFLOADER_H_
#define PBFLOADER_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/BodyStatus.h"
#include <string>
#include <list>

BEGIN_DECLARE_MODULE(PBFLoader)
	REQUIRE(BodyStatus,BodyStatus)
	PROVIDE(YUVImage,Image)
	PROVIDE(ObjectList, Annotations)
END_DECLARE_MODULE(PBFLoader)

/**
 * PBFLoader loads images with informations from PBF files
 */
class PBFLoader : public PBFLoaderBase {
public:

	/**
	 * pbf-data structure
	 */
	struct pbfData {
		std::string pbfPath;		//!< path to PBF file(s)
		int iterationDirection;		//!< iteration direction
		int repeatCount;			//!< repeat count
		bool repeatList;			//!< repeat list
		bool panTiltFromPBF;		//!< get pan tilt information from PBF
		bool autoIterate;			//!< auto iteration activated
	};

	typedef struct pbfData pbfData_t;	//!< typedef of pbfData

	/**
	 * Constructor
	 * @param data		pbfData informations
	 */
	PBFLoader(const pbfData_t *data);

	/**
	 * Destructor
	 */
	virtual ~PBFLoader();

	/**
	 * enable or disable automatic iteration of images
	 * @param autoIterate	if true, automatic iteration is enabled
	 */
	void setAutoIterate(bool autoIterate);

	/**
	 * select previous image
	 */
	bool selectPrevImage();

	/**
	 * select next image
	 */
	bool selectNextImage();

	/**
	 * load image and provide it
	 * @see Module::execute
	 */
	virtual bool execute();

private:
	//lint -e(1704)
    PBFLoader(const PBFLoader& cSource);
    PBFLoader& operator=(const PBFLoader& cSource);

	std::list<std::string>* mFileNames;					//!< file names of pbf images
	std::list<std::string>::iterator mFileNameIterator;	//!< iterator for file names
	int mCurrentRepetition;					//!< current repetition
	pbfData_t mData;						//!< data
};

#endif /* PBFLOADER_H_ */
