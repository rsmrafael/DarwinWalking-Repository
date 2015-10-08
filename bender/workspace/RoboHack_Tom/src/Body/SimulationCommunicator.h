/*
 * SimulationCommunicator.h
 *
 *  Created on: 20.10.2010
 *      Author: Adam
 */

#pragma once

#include <string>
#include "AbstractCommunicator.h"
#include "../Network/Network.h"
/**
 * The SimulationCommunicator is a implementation off the AbstractCommunicator for the Simulation of the Robot.
 * If this Communicator is in use data will be recived from the Simulator over the network and will be send to it.
 */
class SimulationCommunicator : public AbstractCommunicator {
public:
    static SimulationCommunicator* GetInstance();//!< SimulationCommunicator is used as a singelton, here you get the instance.

	const struct statusMessage_t* GetStatusMessage();//!< Get the Statusmessage from the simulation.(Not Implemented at the moment)
	void SendData(uint8_t type, uint8_t instruction, uint8_t controller, size_t commandLen, uint8_t* commandContent);//!< Send Commands and other instructions to the Simulator.
	uint8_t* ReadImage(uint16_t* width, uint16_t* height);//!< The simulator send the Image the Robot get at the moment.

protected:
	SimulationCommunicator();//!< Construcor connect to simulator
	~SimulationCommunicator();//!< Delete Connection

private:
	//lint -e(1704)
    SimulationCommunicator(const SimulationCommunicator& cSource);//!< Constructor for the Copy Operation.
    SimulationCommunicator& operator=(const SimulationCommunicator& cSource);//!< Copy Operation.

    int readAll(char *buf, size_t bytes, size_t packetLen);//!< Used by readImage to check if bytes are okay.

    static SimulationCommunicator* mInstance;//!< Instance for the singelton Pattern.

	Network *mSimulationServerConnection;//!< Communication Network is used for connection with the Simulator.
};

