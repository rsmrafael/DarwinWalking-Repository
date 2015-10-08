/*
 * MitecomRoledecider.h
 *
 *  Created on: 24.02.2014
 *      Author: Oliver Krebs
 */

#ifndef MITECOM_ROLEDECIDER_H_
#define MITECOM_ROLEDECIDER_H_

#include "MitecomData.h"

namespace Mitecom {

	/**
	 * Decide which role this robot will be, depending on the number of team mates
	 * and their distance to the ball.
	 *
	 * @param teamMates	collection of all other team mates
	 * @param me		my private informations
	 * @return new role for this robot or ROLE_OTHER if no ball position is known
	 */
	MITECOM_DATATYPE decideRole(const MixedTeamMates &teamMates, const MixedTeamMate &me);
}

#endif /* MITECOM_ROLEDECIDER_H_ */
