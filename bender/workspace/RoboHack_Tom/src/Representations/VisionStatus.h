/*
 * VisionStatus.h
 *
 *  Created on: 29.04.2014
 *      Author: Oliver Krebs
 */

#ifndef VISIONSTATUS_H_
#define VISIONSTATUS_H_

class VisionStatus {
public:

	enum ObjectStatus {
		OBJECT_SEARCHING,		// searching for object
		OBJECT_SEEN,			// object was seen last frame
		OBJECT_FOUND_STABLE,	// object is seen stable (multiple frames)
		OBJECT_LOST,			// object was seen but then lost
		OBJECT_NOT_FOUND		// object not found stable for one scan loop
	};

	VisionStatus();
	virtual ~VisionStatus();

	ObjectStatus getObjectStatus() const;
	void setObjectStatus(ObjectStatus status);

private:
	ObjectStatus mObjectStatus;
};

#endif /* VISIONSTATUS_H_ */
