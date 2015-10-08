/*-----------------------------------------------------------------------------
 *-----------------------------------------------------------------------------
 */

#ifndef BOARD_H
#define BOARD_H

#ifndef DEBUG_X86
#include "AT91SAM7X256.h"
#endif

#define AT91B_DBGU_BAUD_RATE	115200
#define AT91_BAUD_RATE_EXT		19200
#ifdef DYNAMIXEL
#define AT91_BAUD_RATE_SERVO	1000000
#else
#define AT91_BAUD_RATE_SERVO	115200
#endif

#define EXT_OC           	18432000   // External ocillator MAINCK
#define MCK               	((EXT_OC * 73 / 14) / 2)  // MCK (PLLRC div by 2)
#define MCKKHz            	(MCK / 1000)
#define FMCN_PLL_1_0_US		(MCK / 1000000) // Clocks in 1 us

#define AT91A_USB_PUP		AT91C_PIO_PA25  // USB pullup power
#define AT91A_USB_PR		AT91C_PIO_PA26  // USB power


#endif /* BOARD_H */
