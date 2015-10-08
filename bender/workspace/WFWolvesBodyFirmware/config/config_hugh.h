/*
 * config_2.h
 *
 *  Created on: 20.04.2011
 *      Author: Stefan
 */

#ifndef CONFIG_2_H_
#define CONFIG_2_H_

#define CONFIG_NAME_2	"Hugh"

// Represents values returned by this bot's servos in zero
// position defined by external scaffolding
// -> Robot-specific
#define CONFIG_SERVOS_ZEROPOS_2				{	\
	/*    0    1    2    3    4    5	*/		\
		 -6, -11,  -1,   6,  -2,  -3,			\
	/*    6    7    8    9   10   11	*/		\
		 -2,   2,  -6, -13,  -2,  -4,			\
	/*   12   13   14   15   16   17	*/		\
		 -7,   -4,   0,   0,  -1,   -1,			\
	/*   18   19                        */		\
		  0,   0								}

#endif /* CONFIG_2_H_ */
