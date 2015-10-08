/*
 * inverse_calc.c
 *
 *  Created on: 22.04.2011
 *      Author: Stefan
 */

#include "inverse_calc.h"
#include "inverse.h"
#include "dbgu/dbgu.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

// fit target to a valid vector
void inverse_fitVector(enum body_extremity ext, struct vect3d_ext *target) {
	if (ext == LEG_LEFT || ext == LEG_RIGHT) {
		target->x = MID( LEG_VEC_X_MIN, target->x, LEG_VEC_X_MAX);
		target->y = MID( LEG_VEC_Y_MIN, target->y, LEG_VEC_Y_MAX);
		target->z = MID( LEG_VEC_Z_MIN, target->z, LEG_VEC_Z_MAX);
		float length = sqrt( target->x*target->x + target->y*target->y + target->z*target->z);
		if( length > LEG_LENGTH) {
			float factor = LEG_LENGTH / length;
			target->x = MID( LEG_VEC_X_MIN, target->x * factor, LEG_VEC_X_MAX);
			target->y = MID( LEG_VEC_Y_MIN, target->y * factor, LEG_VEC_Y_MAX);
			target->z = MID( LEG_VEC_Z_MIN, target->z * factor, LEG_VEC_Z_MAX);
		}
	} else if (ext == ARM_LEFT || ext == ARM_RIGHT) {
		target->x = MID( ARM_VEC_X_MIN, target->x, ARM_VEC_X_MAX);
		target->y = MID( ARM_VEC_Y_MIN, target->y, ARM_VEC_Y_MAX);
		target->z = MID( ARM_VEC_Z_MIN, target->z, ARM_VEC_Z_MAX);
		float length = sqrt( target->x*target->x + target->y*target->y + target->z*target->z);
		if( length > ARM_LENGTH) {
			float factor = ARM_LENGTH / length;
			target->x = MID( ARM_VEC_X_MIN, target->x * factor, ARM_VEC_X_MAX);
			target->y = MID( ARM_VEC_Y_MIN, target->y * factor, ARM_VEC_Y_MAX);
			target->z = MID( ARM_VEC_Z_MIN, target->z * factor, ARM_VEC_Z_MAX);
		}
	}
}

void inverse_calculateAngles(enum body_extremity ext, struct vect3d_ext * target, float servoAngles[]) {
	// ensure that vector is in valid range
	inverse_fitVector( ext, target);

	if (ext == LEG_LEFT || ext == LEG_RIGHT) {
		// Calculations based on http://www.fumanoids.de/wp-content/uploads/2011/01/Bachelorarbeit-Otte.pdf, chapter 5.4.2
		// 1st step: Rotate foot
		// HIP_YAW is only servo available to rotate foot, so set it
		float yawrad = target->yaw * (M_PI / 180.0f);
		servoAngles[0] = yawrad; // 0 = HIP_YAW

		// Rotate coordinates accordingly
		float x2 = target->x * cos(yawrad) + target->y * sin(yawrad);
		float y2 = target->x * sin(yawrad) + target->y * cos(yawrad);
		float z2 = target->z;
		//dbgu_printf("[inverse_calc] x,y,z 2 = %3.5f, %3.5f, %3.5f\r\n", x2, y2, z2);

		// 2nd step: Set roll angles
		// Calculate HIP_ROLL from y2 and z2
		float x3 = x2;
		//float y3 = y2;
		float z3 = sqrt((y2 * y2) + (z2 * z2));
#ifdef DYNAMIXEL
		servoAngles[1] = -atan2(y2, z2); // HIP_ROLL
#else
		servoAngles[1] = atan2(y2, z2); // HIP_ROLL
#endif
		// Set foot parallel
		servoAngles[5] = servoAngles[1]; // FOOT_ROLL

		//dbgu_printf("[inverse_calc] x,y,z 3 = %3.5f, %3.5f, %3.5f\r\n", x3, y3, z3);
		// 3rd step:
		// Calculate effective leg length
		float df = sqrt((z3 * z3) + (x3 * x3));

		//float val = MID( -1.f, (df/ 2.f) / THIGH_LENGTH, 1.f);
		float val = MID( -1.f, df / LEG_LENGTH, 1.f);
		float beta = acos(val);
		servoAngles[3] = (2 * beta); // KNEE
		// Calculate hip pitch
		float alpha = atan2(x3, z3);
		servoAngles[2] = -(alpha + beta); // HIP_PITCH
		// Set foot pitch to be parallel
		servoAngles[4] = -alpha + beta; // FOOT_PITCH

		if (ext == LEG_RIGHT) {
			servoAngles[2] = -servoAngles[2];
			servoAngles[3] = -servoAngles[3];
			servoAngles[4] = -servoAngles[4];
		}
	} else if (ext == ARM_LEFT || ext == ARM_RIGHT) {
		// 1nd step: Set roll angles
		// Calculate SHOULDER_ROLL from y2 and z2
		float x3 = -target->x;
		float z3 = sqrt((target->y * target->y) + (target->z * target->z));
		servoAngles[1] = atan2(target->y, target->z); // SHOULDER_ROLL

		// 2nd step:
		// Calculate effective arm length
		float df = sqrt((z3 * z3) + (x3 * x3));
		float val = MID( -1.f, df / ARM_LENGTH, 1.f);
		float beta = acos(val);

		servoAngles[2] = -(2 * beta); // ELBOW
		// Calculate shoulder pitch
		float alpha = atan2(x3, z3);
		servoAngles[0] = -(alpha + beta); // SHOULDER_PITCH

		if (ext == ARM_RIGHT) {
			servoAngles[0] = -servoAngles[0];
			servoAngles[1] = -servoAngles[1];
			servoAngles[2] = -servoAngles[2];
		}
	}
}

