/*
 * config_1.h
 *
 *  Created on: 16.04.2011
 *      Author: Stefan
 */

#ifndef CONFIG_1_H_
#define CONFIG_1_H_

#define CONFIG_NAME_1	"Locutus"

#define CONFIG_LEAN_FORWARD_ANGLE_1		4 // Degrees

// Represents values returned by this bot's servos in zero
// position defined by external scaffolding
// -> Robot-specific
#define CONFIG_SERVOS_ZEROPOS_1				{	\
	/*    0    1    2    3    4    5	*/		\
		 -7,   -6,   0,   0,   0,  0,			\
	/*    6    7    8    9   10   11	*/		\
		  0,   6,   0,   0,   0,  -10,			\
	/*   12   13   14   15   16   17	*/		\
		  0,  -4,  14,  0,   0,   0,			\
	/*   18   19                        */		\
		  0,   0								}

#endif /* CONFIG_1_H_ */
