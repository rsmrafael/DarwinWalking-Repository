/*
 * MitecomCommunicator.h
 *
 *  Created on: 25.02.2014
 *      Author: Oliver Krebs
 */

#ifndef MITECOMCOMMUNICATOR_H_
#define MITECOMCOMMUNICATOR_H_


#include <iostream>
#include "../Thread.h"
#include "../Network/Network.h"
#include "MitecomData.h"

class MitecomCommunicator : public Thread {
public:
	MitecomCommunicator();
	virtual ~MitecomCommunicator();

	void updateInformation( const MixedTeamMateStruct &info);
	void getTeamMates( MixedTeamMates &teamMates);

private:
	//lint -e(1704)
	MitecomCommunicator(const MitecomCommunicator& cSource);
	MitecomCommunicator& operator=(const MitecomCommunicator& cSource);

	void execute(void* arg);
	void handlePacket(const void* data, size_t len, uint64_t currentTime);
	void sendUpdate();

	Network *mNetwork;
	MixedTeamMates mTeamMates;
	MixedTeamMateStruct mInfo;
	uint16_t mTeamID;
	pthread_mutex_t mTeamMutex;
	pthread_mutex_t mInfoMutex;
};


#endif /* MITECOMCOMMUNICATOR_H_ */
