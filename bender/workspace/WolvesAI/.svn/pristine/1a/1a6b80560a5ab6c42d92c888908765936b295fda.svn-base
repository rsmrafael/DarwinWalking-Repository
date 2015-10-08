/*
 * Odometry.h
 *
 *  Created on: 20.02.2013
 *      Author: Jan Carstensen
 */


#ifndef Odometry_H_
#define Odometry_H_

#include <stdint.h>

class Odometry {
public:
	Odometry();
	virtual ~Odometry();

	void update(int16_t deltax, int16_t deltay, int16_t deltao, uint8_t weight);

	int getX() const {return mDeltaX;}
	int getY() const {return mDeltaY;}
	double getOrientation() const {return mDeltaO;}
	double getWeight() const { return mWeight; }

private:
	int mDeltaX;
	int mDeltaY;
	double mDeltaO;
	double mWeight;
};

#endif /* Odometrie_H_ */
