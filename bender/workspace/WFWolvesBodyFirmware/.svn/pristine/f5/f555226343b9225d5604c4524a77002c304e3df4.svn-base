/*
 * vector.h
 *
 *  Created on: 04.12.2012
 *      Author: Oliver
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#define sgn(x)			(x > 0) ? 1 : ((x < 0) ? -1 : 0)

struct vect3d {
	signed short x;
	signed short y;
	signed short z;
};

/**
 * extended vector for directions of extremities
 */
struct vect3d_ext {
	signed short x;
	signed short y;
	signed short z;
	signed short yaw;
};

signed short calc_distance_vect3d( struct vect3d *vec1, struct vect3d *vec2);

#endif /* VECTOR_H_ */
