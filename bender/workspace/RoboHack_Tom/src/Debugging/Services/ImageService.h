/*
 * ImageService.h
 *
 *  Created on: 25.01.2010
 *      Author: Stefan
 */

#ifndef IMAGESERVICE_H_
#define IMAGESERVICE_H_

#include "../PacketListener.h"
#include "../../Modules/ImageSources/PBFLoader.h"

class ImageService : public PacketListener {
public:
	ImageService();
	virtual ~ImageService();

	virtual void setPBFLoader( PBFLoader *loader);
	virtual void newPacket(uint8_t* packet, size_t size);
	virtual string getPacketID();

private:
	PBFLoader *mPBFLoader;
};

#endif /* IMAGESERVICE_H_ */
