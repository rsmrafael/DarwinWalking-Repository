/*
 * PBImage.h
 *
 *  Created on: 16.02.2012
 *      Author: Stefan
 */

#ifndef PBIMAGE_H_
#define PBIMAGE_H_

#include <string>
#include "../../Representations/YUVImage.h"
#include "../../Representations/ObjectList.h"

using namespace std;

namespace protobuf {
	class ProtoBufFrame;
}

class PBImage {
public:
	PBImage(string fileName);
	virtual ~PBImage();

	YUVImage* getImage(int id) const;
	ObjectList getAnnotations(int id) const;
	bool getPanTilt(int id, int* pan, int* tilt) const;

    static bool writePBI(string fileName, const YUVImage& image, int pan, int tilt);
    static void shutdown(void);
private:
	//lint -e(1704)
    PBImage(const PBImage& cSource);
    PBImage& operator=(const PBImage& cSource);


    protobuf::ProtoBufFrame* mLoadedFrame;
};

#endif /* PBIMAGE_H_ */
