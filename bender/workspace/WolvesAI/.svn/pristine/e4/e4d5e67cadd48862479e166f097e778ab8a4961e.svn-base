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

/**
 * Communicator for Mixed-Team-Communication (Mitecom)
 */
class MitecomCommunicator : public Thread {
public:
	/**
	 * Constructor for MitecomCommunicator
	 */
	MitecomCommunicator();

	/**
	 * Destructor for MitecomCommunicator
	 */
	virtual ~MitecomCommunicator();

	/**
	 * updates the private informations
	 * @param info reference to informations in form of MixedTeamMateStruct
	 */
	void updateInformation( const MixedTeamMateStruct &info);

	/**
	 * get information of team mates
	 * @param teamMates reference where information is updated
	 */
	void getTeamMates( MixedTeamMates &teamMates);

private:
	//lint -e(1704)
	MitecomCommunicator(const MitecomCommunicator& cSource);
	MitecomCommunicator& operator=(const MitecomCommunicator& cSource);

	/**
	 * execute communicator, update informations
	 */
	virtual void execute(void* arg);

	/**
	 * handle next packet
	 * @param data 			the data of the packet
	 * @param len 			the length of the packet
	 * @param currentTime 	the current time
	 */
	void handlePacket(const void* data, size_t len, uint64_t currentTime);

	/**
	 * sends an update of private informations
	 */
	void sendUpdate();

	Network *mNetwork;				//!< network for the communication
	MixedTeamMates mTeamMates;		//!< the team mates data (received)
	MixedTeamMateStruct mInfo;		//!< the private data to send
	uint16_t mTeamID;				//!< the ID of own team (read from config)
	pthread_mutex_t mTeamMutex;		//!< mutex for public team data
	pthread_mutex_t mInfoMutex;		//!< mutex for private data
};


#endif /* MITECOMCOMMUNICATOR_H_ */
