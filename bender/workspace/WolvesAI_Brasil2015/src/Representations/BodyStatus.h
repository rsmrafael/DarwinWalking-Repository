/*
 * BodyStatus.h
 *
 *  Created on: 11.12.2012
 *      Author: Stefan Krupop
 */

#ifndef BODYSTATUS_H_
#define BODYSTATUS_H_

#include <map>
#include "ServoStatus.h"
#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"
#include "../Messages/BodyStatus.pb.h"

/**
 * Representation for all body status values (servos, voltage, etc.)
 */
class BodyStatus {
public:
	/**
	 * Constructor
	 */
	BodyStatus();

	/**
	 * Destructor
	 */
	virtual ~BodyStatus();

	/**
	 * set pan and tilt (head)
	 * @param panDeg	pan angle in degree
	 * @param tiltDeg	tilt angle in degree
	 */
	void setPanTilt(int panDeg, int tiltDeg);

	/**
	 * set voltage
	 * @param voltage	voltage
	 */
	void setVoltage(double voltage);

	/**
	 * set status of one servo
	 * @param id				id of the servo
	 * @param maxPositionError	maximal position error
	 * @param temperature		temperature of servo
	 * @param voltage			voltage of servo
	 */
	void setServoStatus(int id, double maxPositionError, double temperature, double voltage);

	/**
	 * return pan angle in degree
	 */
	int getPan() const;

	/**
	 * return tilt angle in degree
	 */
	int getTilt() const;

	/**
	 * return voltage
	 */
	double getVoltage() const;

	/**
	 * return all available servos in a map
	 */
	const map<int, ServoStatus*> getServos() const;

	/**
	 * return servo status
	 * @param id 	ID of the servo
	 */
	ServoStatus* getServo(int id);

private:
	//lint -e(1704)
	BodyStatus(const BodyStatus& cSource);
	BodyStatus& operator=(const BodyStatus& cSource);

	int mPan;			//!< pan angle in degree
	int mTilt;			//!< tilt angle in degree

	double mVoltage;	//!< voltage

	std::map<int, ServoStatus*> mServoStatus;	//!< status of servos
	typedef map<int, ServoStatus*>::iterator ServoStatusIter;
	typedef map<int, ServoStatus*>::const_iterator ServoStatusIterC;
};


/**
 * spezialization of Serializer for BodyStatus
 */
template<>
class Serializer<BodyStatus> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const BodyStatus& representation, ostream& stream);

	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(istream& stream, BodyStatus& representation);
};

#endif /* BODYSTATUS_H_ */
