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

class BodyDataService : public PacketListener {
public:
	BodyDataService(Body* body, RemoteBehavior* remoteAI);
	virtual ~BodyDataService();
	virtual void newPacket(uint8_t* packet, size_t size);
	virtual string getPacketID();
private:
	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters) const;
	Body* mBody;
	RemoteBehavior* mRemoteAI;
};

#endif /* BODYDATASERVICE_H_ */
