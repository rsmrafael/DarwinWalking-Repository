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

/**
 * A protobuf image
 */
class PBImage {
public:

	/**
	 * Contructor, load protobuf image from file
	 * @param fileName		the filename of the image
	 */
	PBImage(string fileName);

	/**
	 * Destructor
	 */
	virtual ~PBImage();

	/**
	 * get YUV-image of given camera
	 * @param id	id of camera; 0: left, 1: right
	 */
	YUVImage* getImage(int id) const;

	/**
	 * get the annotations of the image
	 * @param id	id of camera; 0: left, 1: right
	 */
	ObjectList getAnnotations(int id) const;

	/**
	 * get pan/tilt info of camera
	 * @param id	id of camera; 0: left, 1: right
	 * @param pan	pan angle
	 * @param tilt	tilt angle
	 * @return true if successful read from PBF
	 */
	bool getPanTilt(int id, int* pan, int* tilt) const;

	/**
	 * save an image to a protobuf image file
	 * @param fileName	the filename for the creating file
	 * @param image		the image
	 * @param pan		the pan angle
	 * @param tilt		the tilt angle
	 * @return true if successful
	 */
    static bool writePBI(string fileName, const YUVImage& image, int pan, int tilt);

    /**
     * shutdown
     */
    static void shutdown(void);

private:
	//lint -e(1704)
    PBImage(const PBImage& cSource);
    PBImage& operator=(const PBImage& cSource);


    protobuf::ProtoBufFrame* mLoadedFrame;
};

#endif /* PBIMAGE_H_ */
