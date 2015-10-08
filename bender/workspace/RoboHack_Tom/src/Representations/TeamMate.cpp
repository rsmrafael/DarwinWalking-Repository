/*
 * TeamMate.cpp
 *
 *  Created on: 21.06.2014
 *      Author: Oliver Krebs
 */

#include "TeamMate.h"

TeamMate::TeamMate()
{
	mTeamMateData.robotID = 0;
	mTeamMateData.lastUpdate = 0;
}

TeamMate::~TeamMate()
{

}

void TeamMate::setData(const MixedTeamMate &data)
{
	mTeamMateData = data;
}

MixedTeamMate TeamMate::getData() const
{
	return mTeamMateData;
}

MITECOM_DATATYPE TeamMate::getValue(MITECOM_KEYTYPE key) const
{
	MixedTeamMateDataCIter it = mTeamMateData.data.find(key);
	if( it != mTeamMateData.data.end()) {
		return it->second;
	}
	return 0;
}

bool TeamMate::isConnected() const
{
	return (mTeamMateData.robotID > 0);
}
