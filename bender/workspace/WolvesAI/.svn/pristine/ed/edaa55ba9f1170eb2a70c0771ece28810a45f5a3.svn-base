/*
 * SimulationCommunicator.cpp
 *
 *  Created on: 20.10.2010
 *      Author: Stefan Krupop
 */

#include "SimulationCommunicator.h"
#include "../Debugging/Debugger.h"
#include "../Config.h"
#include <stdlib.h>

#define PACKET_SIZE 1460

SimulationCommunicator* SimulationCommunicator::mInstance = 0;

SimulationCommunicator* SimulationCommunicator::GetInstance() {
	if (mInstance == 0) {
		mInstance = new SimulationCommunicator();
	}
	return mInstance;
}

SimulationCommunicator::SimulationCommunicator() {
	ConfigFile *config = Config::getInstance();
	string host = config->getString("Simulation", "address", "127.0.0.1");
	uint32_t ip = Network::resolveHost(host);
	uint16_t port = config->get<uint16_t>("Simulation", "port", 3490);
	Debugger::INFO("SimulationCommunicator", "Will connect to %s:%d", host.c_str(), port);
	mSimulationServerConnection = new Network(ip, port, true);
	if (!mSimulationServerConnection->isConnected()) {
		Debugger::ERR("SimulationCommunicator", "Could not connect to simulator, exiting...");
		exit(1);
	}
}

SimulationCommunicator::~SimulationCommunicator() {
	delete mSimulationServerConnection;
}

void SimulationCommunicator::SendData(uint8_t type, uint8_t instruction, uint8_t controller, size_t commandLen, uint8_t* commandContent){
	uint8_t data[13];

	if (commandLen > 12)
		commandLen = 12;

	data[0] = instruction;

	if (commandContent != NULL) {
		for(size_t i = 1; i <= 12; i++){
			if (i <= commandLen) {
				data[i] = commandContent[i-1];
			} else {
				data[i] = 0;
			}
		}
	}

	if( !mSimulationServerConnection->sendData((char*)&data[0], (size_t)13)) {
		printf("<2> [SimulationCommunicator] SendData: error\n");
	}
}

uint8_t* SimulationCommunicator::ReadImage(uint16_t* width, uint16_t* height) {
	struct timeval timeout; // timeout for socket access
	uint8_t header[4];

	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	if (mSimulationServerConnection->receiveData((char*) &header[0], (size_t) 4, &timeout, NULL, NULL) > 0) {
		uint16_t recvWidth = header[1] << 8 | header[0];
		uint16_t recvHeight = header[3] << 8 | header[2];
		uint32_t length = recvWidth * recvHeight * 3; // RGB format
		//Debugger::INFO("SimulationCommunicator", "Image %dx%d, %d bytes", recvWidth, recvHeight, length);
		uint8_t* data = (uint8_t*) malloc((size_t) length);
		if (data != NULL) {
			if (readAll((char*) &data[0], (size_t) length, (size_t) PACKET_SIZE) != -1) {
				//Debugger::DEBUG("SimulationCommuincator", "Received image");
				*width = recvWidth;
				*height = recvHeight;
				return data;
			} else {
				free(data);
			}
		}
	}
	return NULL;
}

int SimulationCommunicator::readAll(char *buf, size_t bytes, size_t packetLen) {
	size_t bytesleft = bytes;
	size_t total = 0;
	size_t numbytes = 0;
	struct timeval timeout; // timeout for socket access

	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	while (bytesleft > 0) {
		numbytes = mSimulationServerConnection->receiveData(&buf[total], bytesleft < packetLen ? bytesleft : packetLen, &timeout, NULL, NULL);
		//Debugger::INFO("SimulationServerConnection", "Read %d bytes", numbytes);
		if (numbytes == 0) {
			break;
		}
		bytesleft -= numbytes;
		total += numbytes;
		//Debugger::INFO("SimulationServerConnection", "Bytes left: %d", bytesleft);
	}
	return numbytes == 0 ? -1 : 0;
}

const ICommunicator::statusMessage_t* SimulationCommunicator::GetStatusMessage() {
	/*if (mStatusMessageAvailable) {
		return &mStatusMessage;
	}*/
	return NULL;
}
