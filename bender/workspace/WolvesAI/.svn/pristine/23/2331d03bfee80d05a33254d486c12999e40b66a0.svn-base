/*
 * TeamMate.h
 *
 *  Created on: 21.06.2014
 *      Author: Oliver Krebs
 */

#ifndef TEAMMATE_H_
#define TEAMMATE_H_

#include "../Communication/MitecomData.h"

/**
 * Representation of a teammate
 */
class TeamMate {
public:

	/**
	 * Constructor
	 */
	TeamMate();

	/**
	 * Destructor
	 */
	virtual ~TeamMate();

	/**
	 * set data of teammate
	 * @param data		MixedTeamMate data
	 */
	void setData(const MixedTeamMate &data);

	/**
	 * get the data
	 * @return data
	 */
	MixedTeamMate getData() const;

	/**
	 * get the value to the given key
	 * @return value to the given key
	 */
	MITECOM_DATATYPE getValue(MITECOM_KEYTYPE key) const;

	/**
	 * check if teammate is connected
	 * @return true if teammate is connected
	 */
	bool isConnected() const;

private:
	MixedTeamMate mTeamMateData;	//!< data of teammate
};

#endif /* TEAMMATE_H_ */
