/*
 * MitecomHandler.h
 *
 *  Created on: 24.02.2014
 *      Author: Oliver Krebs
 */

#ifndef MITECOM_HANDLER_H__
#define MITECOM_HANDLER_H__

#include <cstdlib>
//#include <cstdio>
#include "MitecomData.h"


/*------------------------------------------------------------------------------------------------*/

namespace Mitecom {
	MixedTeamMate parseIncoming(const void *messageData, size_t messageLength, uint16_t teamID);
	MixedTeamCommMessage *create(size_t &messageSizePtr, const MixedTeamMate &mate, uint16_t teamID, uint16_t robotID);
}


#endif
