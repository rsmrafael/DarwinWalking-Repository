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

class PacketListener {
	public:
		virtual ~PacketListener() {}
		virtual void newPacket(uint8_t* packet, size_t size) = 0;
		virtual std::string getPacketID() = 0;
};

#endif /* PACKETLISTENER_H_ */
