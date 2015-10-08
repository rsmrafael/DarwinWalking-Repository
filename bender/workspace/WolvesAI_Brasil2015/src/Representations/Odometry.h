/*
 * Odometry.h
 *
 *  Created on: 20.02.2013
 *      Author: Jan Carstensen
 */


#ifndef Odometry_H_
#define Odometry_H_

#include <stdint.h>

/**
 * Representation of odometry
 */
class Odometry {
public:

	/**
	 * Constructor
	 */
	Odometry();

	/**
	 * Destructor
	 */
	virtual ~Odometry();

	/**
	 * update the odometry data
	 * @param deltax	delta x
	 * @param deltay	delta y
	 * @param deltao	delta orientation
	 * @param weight	weight
	 */
	void update(int16_t deltax, int16_t deltay, int16_t deltao, uint8_t weight);

	/**
	 * get the movement in x
	 * @return delta x
	 */
	int getX() const {return mDeltaX;}

	/**
	 * get the movement in y
	 * @return delta y
	 */
	int getY() const {return mDeltaY;}

	/**
	 * get the orientation
	 * @return delta orientation
	 */
	double getOrientation() const {return mDeltaO;}

	/**
	 * get the weight
	 * @return weight
	 */
	double getWeight() const { return mWeight; }

private:
	int mDeltaX;		//!< delta x
	int mDeltaY;		//!< delta y
	double mDeltaO;		//!< delta orientation
	double mWeight;		//!< weight
};

#endif /* Odometrie_H_ */
