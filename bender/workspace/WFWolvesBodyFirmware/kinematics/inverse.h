/*
 * inverse.h
 *
 *  Created on: 26.04.2011
 *      Author: Stefan
 */

#ifndef INVERSE_H_
#define INVERSE_H_

#include "inverse_calc.h"

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

#define MIN(a, b)  		(((a) < (b)) ? (a) : (b))
#define MAX(a, b)  		(((a) > (b)) ? (a) : (b))
#define MID(a, b, c)	(((a) > (b)) ? (a) : (((b) < (c)) ? (b) : (c)))

void inverse_initBalance(void);
void inverse_startCycle(void);
void inverse_calc(enum body_extremity ext, struct vect3d_ext * target);

void inverse_setServoAngle(unsigned char id, float angle);
void inverse_addServoAngle(unsigned char id, float angle);
void inverse_endCycle(unsigned char doNotSend);

struct vect3d inverse_getCurrentPos(enum body_extremity ext);
struct vect3d_ext inverse_getCurrentPosExt(enum body_extremity ext);
signed short inverse_getCurrentPosX(enum body_extremity ext);
signed short inverse_getCurrentPosY(enum body_extremity ext);
signed short inverse_getCurrentPosZ(enum body_extremity ext);
signed short inverse_getCurrentPosYaw(enum body_extremity ext);
void inverse_setHipAngle(float angle);
float inverse_getHipAngle(void);

#endif /* INVERSE_H_ */
