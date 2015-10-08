/*
 * TeamMate.h
 *
 *  Created on: 21.06.2014
 *      Author: Oliver Krebs
 */

#ifndef TEAMMATE_H_
#define TEAMMATE_H_

#include "../Communication/MitecomData.h"

class TeamMate {
public:
	TeamMate();
	virtual ~TeamMate();

	void setData(const MixedTeamMate &data);

	MixedTeamMate getData() const;
	MITECOM_DATATYPE getValue(MITECOM_KEYTYPE key) const;
	bool isConnected() const;

private:
	MixedTeamMate mTeamMateData;
};

#endif /* TEAMMATE_H_ */
