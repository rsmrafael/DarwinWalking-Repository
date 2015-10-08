/*
 * Odemetry.h
 *
 *  Created on: 28.01.2013
 *      Author: 7
 */

#ifndef ODEMETRY_H_
#define ODEMETRY_H_

struct position
{
	float xPos;
	float yPos;
	float viewDirection;
	//Value to change the probability that this Position is right
	unsigned char weight;

	unsigned char changed;
	unsigned char fallen;
};

void odometry_calculateWalkerPos(float xChange,float yChange,float viewDire );
void odometry_reset(void);
void odometry_initOdemetry(void);
unsigned char odometry_getWeight(void);
void odometry_setWeight(unsigned char weight);
void odometry_changeWeight(char change);
void odometry_saveChanges();
void odometry_setViewChange(float viewChange);
void odometry_setyChange(float yChange);
void odometry_setxChange(float xChange);
void odometry_setViewDirection(float direction);
void odometry_addFallen();
struct position* odometry_getPosition();
float ODOMETRY_XCHANGE;
float ODOMETRY_YCHANGE;
float ODOMETRY_VIEWCHANGE;

#endif /* ODEMETRY_H_ */
