/*
 * MitecomCommunicator.cpp
 *
 *  Created on: 25.02.2014
 *      Author: Oliver Krebs
 */

#include <cstdlib>
#include "MitecomCommunicator.h"
#include "MitecomHandler.h"
#include "../Debugging/Debugger.h"
#include "../Config.h"

#define MITECOM_SEND_INTERVAL 250 // in ms
#define MITECOM_UPDATE_TIME_LIMIT 3000 // in ms

MitecomCommunicator::MitecomCommunicator()
{
	pthread_mutex_init(&mTeamMutex, NULL);
	pthread_mutex_init(&mInfoMutex, NULL);
	mTeamID = (uint16_t)Config::GetInt("Game", "teamID", 23);
	mInfo.robotID = 0;
	uint16_t port = (uint16_t)Config::GetInt("Mitecom", "CommunicationPort", 4444);
	mNetwork = new Network(0, port, INADDR_ANY, port);
	this->start(this);
}

MitecomCommunicator::~MitecomCommunicator()
{
	this->stop();
	pthread_mutex_destroy(&mTeamMutex);
	pthread_mutex_destroy(&mInfoMutex);
	delete mNetwork;
}

void MitecomCommunicator::execute(void* arg)
{
	char networkBuffer[MITECOM_LENGTH_MAX];
	struct timeval timeout;     // timeout for socket access

	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	uint64_t currentTime;
	uint64_t lastSendTime = getCurrentTime();

	do {
		if (mNetwork == NULL) {
			break;
		}
		currentTime = getCurrentTime();
		size_t len = mNetwork->receiveData(networkBuffer, MITECOM_LENGTH_MAX, &timeout, NULL, NULL);
		if( len > 0) {
			handlePacket(networkBuffer, len, currentTime);
		}
		if( currentTime - lastSendTime >= MITECOM_SEND_INTERVAL) {
			sendUpdate();
			lastSendTime = currentTime;
			pthread_mutex_lock(&mTeamMutex);
			for (MixedTeamMates::iterator it = mTeamMates.begin(); it != mTeamMates.end(); ) {
				if( currentTime - (it->second).lastUpdate >= MITECOM_UPDATE_TIME_LIMIT) {
					Debugger::INFO("Mitecom", "Robot %d has exceeded time limit. Good bye.",
							it->first);
					mTeamMates.erase(it++);
				} else {
					++it;
				}
			}
			pthread_mutex_unlock(&mTeamMutex);
		}
	} while (IsRunning());
}

void MitecomCommunicator::handlePacket(const void* data, size_t len, uint64_t currentTime)
{
	MixedTeamMate teamMate = Mitecom::parseIncoming(data, len, mTeamID);
	// robot is valid (> 0) and not myself
	uint16_t myID;
	pthread_mutex_lock(&mInfoMutex);
	myID = mInfo.robotID;
	pthread_mutex_unlock(&mInfoMutex);
	if (teamMate.robotID > 0 && teamMate.robotID != myID) {
		if (mTeamMates.find(teamMate.robotID) == mTeamMates.end()) {
			Debugger::INFO("Mitecom", "Adding robot %d to team mates", teamMate.robotID);
		}
		pthread_mutex_lock(&mTeamMutex);
		// add team mate to our map
		mTeamMates[teamMate.robotID] = teamMate;

		// remember the last time (i.e. now) that we heard from this robot
		mTeamMates[teamMate.robotID].lastUpdate = currentTime;
		pthread_mutex_unlock(&mTeamMutex);
		Debugger::DEBUG("Mitecom", "TeamMate %d updated", teamMate.robotID);
	}
}

void MitecomCommunicator::sendUpdate()
{
	MixedTeamCommMessage *messageDataPtr;
	size_t messageDataLength = 0;

	pthread_mutex_lock(&mInfoMutex);
	if( mInfo.robotID == 0) {
		pthread_mutex_unlock(&mInfoMutex);
		//Debugger::INFO("Mitecom", "No information available to send");
		return;
	}

	// serialize and broadcast data
	messageDataPtr = Mitecom::create(messageDataLength, mInfo, mTeamID, mInfo.robotID);
	pthread_mutex_unlock(&mInfoMutex);
	if( messageDataPtr != NULL && mNetwork != NULL) {
		if( !mNetwork->sendData((const char*)messageDataPtr, messageDataLength)) {
			printf("<2> [Mitecom] sendUpdate: error\n");
		} else {
			Debugger::DEBUG("Mitecom", "Update sent");
		}
		free(messageDataPtr);
	}
}

void MitecomCommunicator::updateInformation( const MixedTeamMateStruct &info)
{
	pthread_mutex_lock(&mInfoMutex);
	mInfo = info;
	pthread_mutex_unlock(&mInfoMutex);
	Debugger::DEBUG("Mitecom", "Information updated");
}

void MitecomCommunicator::getTeamMates(MixedTeamMates &teamMates)
{
	pthread_mutex_lock(&mTeamMutex);
	teamMates = mTeamMates;
	pthread_mutex_unlock(&mTeamMutex);
}
