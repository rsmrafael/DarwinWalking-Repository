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
#include <string>
#include <list>
#include "../../Representations/ObjectList.h"
#include "../../Representations/BodyStatus.h"

BEGIN_DECLARE_MODULE(PBFLoader)
	REQUIRE(BodyStatus,BodyStatus) // Used to make shure it is executed after the real pan/tilt value is set in BodyStatus so that we do not get overwritten
	PROVIDE(YUVImage,Image)
	PROVIDE(ObjectList, Annotations)
END_DECLARE_MODULE(PBFLoader)

class PBFLoader : public PBFLoaderBase {
public:
	struct pbfData {
		std::string pbfPath;
		int iterationDirection;
		int repeatCount;
		bool repeatList;
		bool panTiltFromPBF;
		bool autoIterate;
	};

	PBFLoader(const struct pbfData *data);
	virtual ~PBFLoader();

	void setAutoIterate( bool autoIterate);
	bool selectPrevImage();
	bool selectNextImage();

	virtual bool execute();
private:
	//lint -e(1704)
    PBFLoader(const PBFLoader& cSource);
    PBFLoader& operator=(const PBFLoader& cSource);

	list<string>* mFileNames;
	list<string>::iterator mFileNameIterator;
	int mCurrentRepetition;
	struct pbfData mData;
};

#endif /* PBFLOADER_H_ */
