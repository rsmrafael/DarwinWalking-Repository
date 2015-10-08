#include "bootloader.h"
#include "include/Board.h"

#ifndef DEBUG_X86
__attribute__ ((long_call, section(".fastrun")))
#endif
void setGPNVMBit() {
#ifndef DEBUG_X86
	// Disable interrupts
	unsigned int mask = AT91C_BASE_AIC->AIC_IMR;
	AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;

	// Clear GPNVM 2
	AT91C_BASE_MC->MC_FCR = (unsigned int)((0x5A << 24) | (AT91C_MC_PAGEN & ( 2 << 8 )) | AT91C_MC_FCMD_CLR_GP_NVM); // Clear GPNVM bit 2
	while ((AT91C_BASE_MC->MC_FSR & AT91C_MC_FRDY) != AT91C_MC_FRDY); //wait for flash write to complete

	// Restore interrupts
	AT91C_BASE_AIC->AIC_IECR = mask;

	// Reset processor and pheripherals
	AT91C_BASE_RSTC->RSTC_RCR = (unsigned int)((0xA5 << 24) | AT91C_RSTC_PROCRST | AT91C_RSTC_PERRST | AT91C_RSTC_EXTRST); //reset the processor and the peripherals
#endif
}

void Bootloader_start (void) {
#ifndef DEBUG_X86
	AT91C_BASE_MC->MC_FMR = (unsigned int)((FMCN_PLL_1_0_US << 16) | AT91C_MC_FWS_1FWS); //setup flash mode register (1.0us)
	while (!(AT91C_BASE_MC->MC_FSR & AT91C_MC_FRDY)); //wait for flash to become ready
	AT91C_BASE_PIOA->PIO_CODR   = AT91A_USB_PUP;		// Set to LOW to enable renew Enumbering
	setGPNVMBit(); //call to RAM routine - does not return
#endif
}
