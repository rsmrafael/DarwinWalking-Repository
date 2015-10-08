/*
 * KickStatus.h
 *
 *  Created on: 21.06.2014
 *      Author: Oliver Krebs
 */

#ifndef KICKSTATUS_H_
#define KICKSTATUS_H_

#include <inttypes.h>

/**
 * Status of kick
 */
class KickStatus {
public:
	/**
	 * Constructor
	 */
	KickStatus();

	/**
	 * Destructor
	 */
	virtual ~KickStatus();

	/**
	 * set the kicking time
	 * @param time 		time of kicking
	 */
	void setKickingTime(uint64_t time);

	/**
	 * get the kicking time
	 * @return kicking time
	 */
	uint64_t getKickingTime() const;

private:
	uint64_t mKickingTime; //!< kicking time
};

#endif /* KICKSTATUS_H_ */
