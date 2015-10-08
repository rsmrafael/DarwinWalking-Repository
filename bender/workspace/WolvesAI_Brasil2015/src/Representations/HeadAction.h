/*
 * HeadAction.h
 *
 *  Created on: 24.04.2014
 *      Author: Oliver Krebs
 */

#ifndef HEADACTION_H_
#define HEADACTION_H_

#include "../Body/ICommunicator.h"
#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"
#include "Object.h"

/**
 * Representation for all head actions
 */
class HeadAction {
public:
	/**
	 * Constructor
	 */
	HeadAction();

	/**
	 * Destructor
	 */
	virtual ~HeadAction();

	/**
	 * set pan and tilt angle for head
	 * @param panDeg	pan angle in degree
	 * @param tiltDeg	tilt angle in degree
	 */
	void setPanTilt(int panDeg, int tiltDeg);

	/**
	 * activate pan/tilt tracking
	 * @param disablePan disable pan tracking
	 */
	void PanTiltTracking(bool disablePan);

	/**
	 * activate pan/tilt tracking with given error values
	 * @param errorX		the tracking error in x
	 * @param errorY		the tracking error in y
	 * @param disablePan	disbale pan tracking
	 */
	void PanTiltTracking(double errorX, double errorY, bool disablePan);

	/**
	 * get the pan angle in degree
	 */
	int getPan() const { return mPan; }

	/**
	 * get the tilt angle in degree
	 */
	int getTilt() const { return mTilt; }

	/**
	 * get pan/tilt change id
	 */
	uint8_t getPanTiltChangeID() const { return mPanTiltChangeID; }

	/**
	 * return if pan/tilt is set directly
	 */
	bool getPanTiltSetDirect() const { return mPanTiltSetDirect; }

	/**
	 * returns if pan/tilt is used with error value
	 */
	bool getPanTiltWithError() const { return mWithError; }

	/**
	 * returns the error of pan/tilt angle in x
	 */
	double getPanTiltErrorX() const { return mErrorX; }

	/**
	 * returns the error of pan/tilt angle in y
	 */
	double getPanTiltErrorY() const { return mErrorY; }

	/**
	 * returns if pan error is disabled
	 */
	bool getDisablePanError() const { return mDisablePanError; }

	/**
	 * activate search for object
	 * @param type			the object type which is searched
	 * @param resetSearch	start new search (true) or not
	 */
	void searchObject( Object::ObjectType type, bool resetSearch);

	/**
	 * returns the type of object which is searched
	 */
	Object::ObjectType getSearchObjectType();

	/**
	 * returns if search should be reset
	 */
	bool getResetSearch() const;

private:
	Object::ObjectType mSearchObjectType;	//!< type of searched object

	int mPan;								//!< pan angle in degree
	int mTilt;								//!< tilt angle in degree
	double mErrorX;							//!< error in x
	double mErrorY;							//!< error in y
	bool mDisablePanError;					//!< disable error of pan
	bool mWithError;						//!< pan/tilt with error
	bool mPanTiltSetDirect;					//!< set pan/tilt direct
	bool mResetSearch;						//!< start new search

	uint8_t mPanTiltChangeID;				//!< id for register changes of pan/tilt
};

#endif /* HEADACTION_H_ */
