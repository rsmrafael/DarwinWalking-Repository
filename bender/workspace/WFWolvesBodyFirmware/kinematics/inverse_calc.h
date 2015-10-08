/*
 * inverse_calc.h
 *
 *  Created on: 22.04.2011
 *      Author: Stefan
 */

#ifndef INVERSE_CALC_H_
#define INVERSE_CALC_H_

#include "vector.h"

#define INVERSE_SERVOS_MAX	6

#ifdef DYNAMIXEL

	#define THIGH_LENGTH	93.0f
	#define CALF_LENGTH		93.0f
	#define ANKLE_LENGTH	33.5f
	#define LEG_LENGTH		(THIGH_LENGTH + CALF_LENGTH + ANKLE_LENGTH)
	#define ARM_LENGTH		170.0f

// Todo: change offsets!
	// Servos offsets in degree
	// 1: -45; 2: 45;
	#define INVERSE_SERVO_OFFSETS				{	\
		/*    1    2    3    4    5    6	*/		\
		    - 5,   5,  40, -40,  -6,   6,	    	\
		/*    7    8    9   10   11   12	*/		\
			  0,   0,   0,   0,18.0,-18.0,			\
		/*   13   14   15   16   17   18	*/		\
			  0,   0,   0,   0,   0,   0,			\
		/*   19   20                        */		\
			  0,   0								}

	// Minima, Maxima und Initialwerte des Bein-Vektors
	#define LEG_VEC_X_MIN	-150
	#define LEG_VEC_X_MAX    180
	#define LEG_VEC_X_INIT     0// Walker: -18.0
	#define LEG_VEC_Y_MIN   -150
	#define LEG_VEC_Y_MAX    150
	#define LEG_VEC_Y_INIT     0// Walker: +/-2.5
	#define LEG_VEC_Z_MIN     80
	#define LEG_VEC_Z_MAX    218
	#define LEG_VEC_Z_INIT   200
	// Minima, Maxima und Initialwerte des Arm-Vektors
	#define ARM_VEC_X_MIN   -170
	#define ARM_VEC_X_MAX    170
	#define ARM_VEC_X_INIT    50
	#define ARM_VEC_Y_MIN   -100
	#define ARM_VEC_Y_MAX    170
	#define ARM_VEC_Y_INIT     0
	#define ARM_VEC_Z_MIN  	-170
	#define ARM_VEC_Z_MAX    170
	#define ARM_VEC_Z_INIT   130

#else

	#define THIGH_LENGTH	59.0f
	#define CALF_LENGTH		59.0f
	#define ANKLE_LENGTH	25.0f
	#define LEG_LENGTH		(THIGH_LENGTH + CALF_LENGTH + ANKLE_LENGTH)
	#define ARM_LENGTH		114.0f

	// Servos offsets in degree
	// When the servos are set to these values,
	// the leg has to be straight and the foot plate parallel
	// to the body
	// 80° = Displacement by upper knee bracket (arcsin(10 mm/59 mm))
	// 28° = Displacement by lower knee bracket (arcsin(28 mm/59 mm))
	// 62° = 90° - 28° to correct displacement by lower bracket
	//  9° = Displacement by elbow joint
	#define INVERSE_SERVO_OFFSETS				{	\
		/*    0    1    2    3    4    5	*/		\
			  0, -45,  80,  28, -62,  90,			\
		/*    6    7    8    9   10   11	*/		\
			  0,  45, -80, -28,  62, -90,			\
		/*   12   13   14   15   16   17	*/		\
			  0, -90,   9,   0,  90,  -9,			\
		/*   18   19                        */		\
			  0,   0								}

	// Minima, Maxima und Initialwerte des Bein-Vektors
	#define LEG_VEC_X_MIN   -100
	#define LEG_VEC_X_MAX    100
	#define LEG_VEC_X_INIT     0
	#define LEG_VEC_Y_MIN   -100
	#define LEG_VEC_Y_MAX    100
	#define LEG_VEC_Y_INIT     0
	#define LEG_VEC_Z_MIN     70
	#define LEG_VEC_Z_MAX    118
	#define LEG_VEC_Z_INIT   100
	// Minima, Maxima und Initialwerte des Arm-Vektors
	#define ARM_VEC_X_MIN   -114
	#define ARM_VEC_X_MAX    114
	#define ARM_VEC_X_INIT     0
	#define ARM_VEC_Y_MIN      0
	#define ARM_VEC_Y_MAX    114
	#define ARM_VEC_Y_INIT     0
	#define ARM_VEC_Z_MIN   -114
	#define ARM_VEC_Z_MAX    114
	#define ARM_VEC_Z_INIT    80

#endif

enum body_extremity {
	LEG_LEFT, LEG_RIGHT, ARM_LEFT, ARM_RIGHT, COUNT_EXTREMITIES
};

/**
 * fit the target vector of extremity to valid values (for inverse kinematics)
 */
void inverse_fitVector(enum body_extremity ext, struct vect3d_ext *target);

void inverse_calculateAngles(enum body_extremity ext, struct vect3d_ext * target, float servoAngles[]);

#endif /* INVERSE_CALC_H_ */
