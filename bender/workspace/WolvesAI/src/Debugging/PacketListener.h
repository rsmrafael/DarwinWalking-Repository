/*
 * PacketListener.h
 *
 *  Created on: 18.01.2010
 *      Author: Stefan
 */

#ifndef PACKETLISTENER_H_
#define PACKETLISTENER_H_

#include <stdint.h>
#include <string>

/**
 * interface for packet listeners
 */
class PacketListener {
public:
	/**
	 * Destructor
	 */
	virtual ~PacketListener() {}

	/**
	 * handle a new packet
	 * @param packet	pointer to the data of packet
	 * @param size		the length of packet in bytes
	 */
	virtual void newPacket(uint8_t* packet, size_t size) = 0;

	/**
	 * get the packet identifier
	 * e.g. IMG for image services
	 */
	virtual std::string getPacketID() = 0;
};

#endif /* PACKETLISTENER_H_ */
