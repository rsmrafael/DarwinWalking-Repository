/*
 * BodyDataService.h
 *
 *  Created on: 12.04.2010
 *      Author: Stefan
 */

#ifndef BODYDATASERVICE_H_
#define BODYDATASERVICE_H_

#include <stdint.h>
#include <vector>
#include "../PacketListener.h"
#include "../../Body/Body.h"
#include "../../Modules/Behavior/RemoteBehavior.h"

/**
 * BodyData Service
 */
class BodyDataService : public PacketListener {
public:

	/**
	 * Constructor
	 * @param body		connection to the body
	 * @param remoteAI	remote behavior
	 */
	BodyDataService(Body* body, RemoteBehavior* remoteAI);
	virtual ~BodyDataService();

	/**
	 * GET
	 *  IMU: get IMU data (pitch, roll)
	 *  BAT: get battery voltage
	 * SET
	 *  WLK {x},{y},{r},{t}: walk command
	 *  KCK {x},{y},{z}[,{tx},{ty},{tz}]: kick command
	 *  MOV: movement command
	 *  P/T {p},{t}: pan/tilt command
	 *  ACT
	 *   ENABLE: enable remote control
	 *   {*}: disable remote control
	 */
	virtual void newPacket(uint8_t* packet, size_t size);
	virtual string getPacketID();
private:
	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters) const;
	Body* mBody;
	RemoteBehavior* mRemoteAI;
};

#endif /* BODYDATASERVICE_H_ */
