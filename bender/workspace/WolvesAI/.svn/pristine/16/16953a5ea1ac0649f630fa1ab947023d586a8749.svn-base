/*
 * TestCommunicator.h
 *
 *  Created on: 29.12.2013
 *      Author: Oliver Krebs
 */

#ifndef TESTCOMMUNICATOR_H_
#define TESTCOMMUNICATOR_H_

#include "AbstractCommunicator.h"

/**
 * Communicator for testing communication
 */
class TestCommunicator : public AbstractCommunicator {

public:
	/**
	 * status of a transmission
	 */
	enum TransmissionStatus {
		OKAY,					//!< transmission successful
		INVALID_TYPE,			//!< invalid type of packet
		INVALID_INSTRUCTION,	//!< invalid instruction of packet
		INVALID_CONTROLLER,		//!< invalid controller of packet
		INVALID_LENGTH,			//!< invalid length of packet
		INVALID_CONTENT			//!< invalid content of packet
	};

	/**
	 * Constructor, initializes variables
	 */
	TestCommunicator();

	/**
	 * Destructor
	 */
	~TestCommunicator();

	/**
	 * Get newest status message
	 * @see AbstractCommunicator::GetStatusMessage
	 */
	virtual const statusMessage_t* GetStatusMessage();

	/**
	 * Send data
	 * @see AbstractCommunicator::SendData
	 */
	void SendData(uint8_t type, uint8_t instruction, uint8_t controller, size_t commandLen, uint8_t* commandContent);

	/**
	 * get the status of transmission
	 * @return status of transmission (OKAY, INVALID_TYPE, ...)
	 */
	TransmissionStatus GetTransmissionStatus() const;

	/**
	 * set the voltage
	 * @param voltage 	the (virtual) voltage of robot
	 */
	void SetVoltage(uint16_t voltage);

	/**
	 * set the IMU data
	 * @param imu 		the (virtual) data of IMU
	 */
	void SetImuData(const imuData_t &imu);

	/**
	 * set the odometry data
	 * @param odometry	the (virtual) data of odometry
	 */
	void SetOdometryData(const odometryData_t &odometry);

	/**
	 * get the last pan angle in degree
	 */
	int16_t GetPan() const;

	/**
	 * get the last tilt angle in degree
	 */
	int16_t GetTilt() const;

	/**
	 * get the last walk right/left parameter
	 */
	int8_t GetWalkX() const;

	/**
	 * get the last walk front/back parameter
	 */
	int8_t GetWalkY() const;

	/**
	 * get the last walk rotation parameter
	 */
	int8_t GetWalkR() const;

	/**
	 * get the turning aim value for walk
	 */
	bool GetWalkTurningAim() const;

	/**
	 * get the position of ball for kick
	 */
	vect3d_t GetKickerBall() const;

	/**
	 * get the position of target for kick
	 */
	vect3d_t GetKickerTarget() const;

	/**
	 * get the last motion page
	 */
	Motion GetMotionPage() const;

	/**
	 * get the number of repetitions for movement
	 */
	uint16_t GetMovementRepetitions() const;

	/**
	 * get the strength for movement
	 */
	uint16_t GetMovementStrength() const;

	/**
	 * get the direction for movement
	 */
	uint16_t GetMovementDirection() const;

private:
	/**
	 * receive and process a walker command
	 * @param commandLen		the length of command in bytes
	 * @param commandContent	the content of the command
	 */
	void ReceiveWalkerCommand(size_t commandLen, const uint8_t* commandContent);

	/**
	 * receive and process a walker command
	 * @param commandLen		the length of command in bytes
	 * @param commandContent	the content of the command
	 */
	void ReceivePanTiltCommand(size_t commandLen, const uint8_t* commandContent);

	/**
	 * receive and process a kicker command
	 * @param commandLen		the length of command in bytes
	 * @param commandContent	the content of the command
	 */
	void ReceiveKickerCommand(size_t commandLen, const uint8_t* commandContent);

	/**
	 * receive and process a strength direction movement command
	 * @param commandLen		the length of command in bytes
	 * @param commandContent	the content of the command
	 */
	void ReceiveStrengthDirectionMovementCommand(size_t commandLen, const uint8_t* commandContent);

	/**
	 * receive and process a repeated movement command
	 * @param commandLen		the length of command in bytes
	 * @param commandContent	the content of the command
	 */
	void ReceiveRepeatedMovementCommand(size_t commandLen, const uint8_t* commandContent);

	TransmissionStatus transmissionStatus;	//!< the status of last transmission

	statusMessage_t statusMessage;	//!< the last status message

	int16_t pan;		//!< pan angle in degree
	int16_t tilt;		//!< tilt angle in degree

	int8_t walkX;		//!< walk right/left
	int8_t walkY;		//!< walk front/back
	int8_t walkR;		//!< walk rotation
	bool walkTurningAim;//!< walk turning aim

	vect3d_t kickerBall;	//!< position of ball for kick
	vect3d_t kickerTarget;	//!< position of target for kick

	Motion motionPage;		//!< motion page
	uint16_t movementRepetitions;	//!< number of movement repetitions
	uint16_t movementStrength;		//!< strength of movement
	uint16_t movementDirection;		//!< direction of movement

};

#endif /* TESTCOMMUNICATOR_H_ */
