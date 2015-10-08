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
 * The SimulationCommunicator is an implementation off the AbstractCommunicator
 * for the Simulation of the Robot. If this Communicator is in use data will be
 * received from the Simulator over the network and will be send to it.
 */
class SimulationCommunicator : public AbstractCommunicator {
public:
	/**
	 * SimulationCommunicator is used as a singelton, here you get the instance.
	 * @return instance of SimulationCommunicator
	 */
    static SimulationCommunicator* GetInstance();

    /**
     * @see ICommunicator::GetStatusMessage
     */
	virtual const statusMessage_t* GetStatusMessage();

	/**
	 * @see AbstractCommunicator::SendData
	 */
	void SendData(uint8_t type, uint8_t instruction, uint8_t controller, size_t commandLen, uint8_t* commandContent);

	/**
	 * The Robot use the Image from the Simulator as input instead of the normal Camera.
	 * With this method it reads the image from the simulator.
	 * @param width 	a pointer to the width of the image
	 * @param height 	a pointer to the height of the image
	 * @return a pointer to the image
	 */
	uint8_t* ReadImage(uint16_t* width, uint16_t* height);

protected:
	SimulationCommunicator();//!< Construcor connect to simulator
	~SimulationCommunicator();//!< Delete Connection

private:
	//lint -e(1704)
	/**
	 * Copy constructor
	 * @param cSource	source for the copy
	 */
    SimulationCommunicator(const SimulationCommunicator& cSource);

    /**
     * copy operation
     */
    SimulationCommunicator& operator=(const SimulationCommunicator& cSource);

    /**
     * read everything to a buffer until the packetLen or the end of received data is reached
     * @param buf 		here the data is stored.
     * @param bytes 	the number of bytes that should be read.
     * @param packetLen length of the packet get from the simulator
     * @return 0 if reading was correct, -1 if read bytes does not match to packetLen
     */
    int readAll(char *buf, size_t bytes, size_t packetLen);

    static SimulationCommunicator* mInstance;	//!< Instance for the singleton pattern.

	Network *mSimulationServerConnection;		//!< used for connection with the Simulator.
};

