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

	/**
	 * Take an incoming message and check if the message was received correctly
	 * and store the data in data struct defined in @mitecom-data.h
	 * @param messageData		pointer to received data
	 * @param messageLength		length of received message
	 * @param teamID			ID of the own team
	 * @return the team mate parsed from the incoming data
	 */
	MixedTeamMate parseIncoming(const void *messageData, size_t messageLength, uint16_t teamID);

	/**
	 ** Create the serialization of a team mate's information. The result of this
	 ** function can be directly broadcasted.
	 **
	 ** @param messageSizePtr   pointer to an integer to store the size of the serialized message
	 ** @param mate             the team member data to serialize (see @mitecom-data.h)
	 ** @param teamID           the ID of the team this team mate belongs to
	 ** @param robotID          the ID of the robot
	 **
	 ** @return pointer to allocated structure (must be released with free!)
	 */
	MixedTeamCommMessage *create(size_t &messageSizePtr, const MixedTeamMate &mate, uint16_t teamID, uint16_t robotID);
}


#endif
