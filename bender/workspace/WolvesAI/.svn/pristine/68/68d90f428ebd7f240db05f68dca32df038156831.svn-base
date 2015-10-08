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

/**
 * Provides services for images
 */
class ImageService : public PacketListener {
public:
	ImageService();
	virtual ~ImageService();

	/**
	 * set the PBF loader
	 * @param loader	the new PBF loader
	 */
	virtual void setPBFLoader( PBFLoader *loader);

	/**
	 * Handle new packet, packet sequence can start like followed:
	 *
	 * GET: get a camera image
	 *  YUV: get a YUV image in PBF format
	 * SET: not implemented
	 * LOAD: functions for PBF-Loader
	 * 	PREV: load previous image
	 * 	NEXT: load next image
	 * 	AUTO: enable auto iteration
	 * 	STOP: disable auto iteration
	 *
	 * @param packet	data of packet
	 * @param size		length of packet in bytes
	 */
	virtual void newPacket(uint8_t* packet, size_t size);

	/**
	 * @see PacketListener
	 */
	virtual string getPacketID();

private:
	PBFLoader *mPBFLoader; //!< current PBF loader
};

#endif /* IMAGESERVICE_H_ */
