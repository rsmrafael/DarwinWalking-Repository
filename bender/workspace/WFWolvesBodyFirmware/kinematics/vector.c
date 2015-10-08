/*
 * vector.c
 *
 *  Created on: 04.12.2012
 *      Author: Oliver
 */

#include "vector.h"
#include <math.h>

signed short calc_distance_vect3d( struct vect3d *vec1, struct vect3d *vec2)
{
	signed short x = vec2->x - vec1->x;
	signed short y = vec2->y - vec1->y;
	signed short z = vec2->z - vec1->z;
	return sqrt( x*x + y*y + z*z);
}
