/*
 * VisionStatus.h
 *
 *  Created on: 29.04.2014
 *      Author: Oliver Krebs
 */

#ifndef VISIONSTATUS_H_
#define VISIONSTATUS_H_

/**
 * Representation of vision status
 */
class VisionStatus {
public:

	/**
	 * status of object
	 */
	enum ObjectStatus {
		OBJECT_SEARCHING,		//!< searching for object
		OBJECT_SEEN,			//!< object was seen last frame
		OBJECT_FOUND_STABLE,	//!< object is seen stable (multiple frames)
		OBJECT_LOST,			//!< object was seen but then lost
		OBJECT_NOT_FOUND		//!< object not found stable for one scan loop
	};

	/**
	 * Constructor
	 */
	VisionStatus();

	/**
	 * Destructor
	 */
	virtual ~VisionStatus();

	/**
	 * get vision status of object
	 * @return object status
	 */
	ObjectStatus getObjectStatus() const;

	/**
	 * set the vision status of object
	 * @param status	status of object
	 */
	void setObjectStatus(ObjectStatus status);

private:
	ObjectStatus mObjectStatus;		//!< vision status of object
};

#endif /* VISIONSTATUS_H_ */
