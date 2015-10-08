/*
 * BodyController.h
 *
 *  Created on: 27.11.2012
 *      Author: Stefan Krupop
 */

#ifndef BODYCONTROLLER_H_
#define BODYCONTROLLER_H_

#include "../ModuleManager/Module.h"
#include "../Representations/HeadAction.h"
#include "../Representations/BodyAction.h"
#include "../Body/Body.h"

BEGIN_DECLARE_MODULE(BodyController)
	REQUIRE(HeadAction,HeadAction)
	REQUIRE(BodyAction,BodyAction)
END_DECLARE_MODULE(BodyController)

/**
 * BodyController
 */
class BodyController : public BodyControllerBase {
public:
	/**
	 * Constructor
	 * @param body		connection to body
	 */
	BodyController(Body* body);

	/**
	 * Destructor
	 */
	virtual ~BodyController();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

private:
	Body* mBody;
	uint8_t mPanTiltLastProcessedID;
	uint8_t mMovementLastProcessedID;

	//lint -e(1704)
	BodyController(const BodyController& cSource);
	BodyController& operator=(const BodyController& cSource);
};

#endif /* BODYCONTROLLER_H_ */
