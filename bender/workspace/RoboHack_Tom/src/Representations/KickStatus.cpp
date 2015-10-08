/*
 * KickStatus.cpp
 *
 *  Created on: 21.06.2014
 *      Author: Oliver Krebs
 */

#include "KickStatus.h"

KickStatus::KickStatus()
:	mKickingTime(0)
{

}

KickStatus::~KickStatus()
{

}

void KickStatus::setKickingTime(uint64_t time)
{
	mKickingTime = time;
}

uint64_t KickStatus::getKickingTime() const
{
	return mKickingTime;
}
