#include "usb.h"
#include "../include/Board.h"
#include "cdc_enumerate.h"
#include "tc/tc.h"

#ifndef DEBUG_X86
static struct _AT91S_CDC pCDC;
#endif

void USB_Init(void) {
#ifndef DEBUG_X86
	// Init USB device
	// Set the PLL USB Divider
	AT91C_BASE_CKGR->CKGR_PLLR |= AT91C_CKGR_USBDIV_1 ;

	// Specific Chip USB Initialisation
	// Enables the 48MHz USB clock UDPCK and System Peripheral USB Clock
	AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_UDP;
	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_UDP);

	// Enable UDP PullUp (USB_DP_PUP) : enable & Clear of the corresponding PIO
	// Set in PIO mode and Configure in Output
	AT91C_BASE_PIOA->PIO_PER 	= AT91A_USB_PUP; 		// Set in PIO mode
	AT91C_BASE_PIOA->PIO_OER    = AT91A_USB_PUP;		// Configure as output

	// Set for set the Pull up resistor
	AT91C_BASE_PIOA->PIO_SODR   = AT91A_USB_PUP;		// Set to HIGH

	// CDC Open by structure initialization
	AT91F_CDC_Open(&pCDC, AT91C_BASE_UDP);
#endif
}

void USBWaitForConfig(void) {
#ifndef DEBUG_X86
	while (!pCDC.IsConfigured(&pCDC));
#endif
}

unsigned char USBWaitForConfigTimeout(unsigned int timeout) {
#ifndef DEBUG_X86
	while (!pCDC.IsConfigured(&pCDC) && timeout > 0) {
		TC_DelayMs(1);
		timeout--;
	}
	return pCDC.IsConfigured(&pCDC);
#else
	return 0;
#endif
}

unsigned int USBRead(char *pData, unsigned int maxLength) {
#ifndef DEBUG_X86
	return pCDC.Read(&pCDC, pData, maxLength);
#else
	return 0;
#endif
}

unsigned int USBWrite(const char *pData, unsigned int length) {
#ifndef DEBUG_X86
	return pCDC.Write(&pCDC, pData, length);
#else
	return 0;
#endif
}

void USBDisconnect(void) {
#ifndef DEBUG_X86
	// Disable USB clock
	AT91C_BASE_PMC->PMC_PCDR = (1 << AT91C_ID_UDP);
	// Disable pullup resistor
	AT91C_BASE_PIOA->PIO_CODR   = AT91A_USB_PUP;		// Set to LOW
#endif
}

void USB_print_ascii(const char *buffer) {
    unsigned int length = 0;

    while (*(buffer + length) != '\0') {
    	length++;
    }
    USBWrite(buffer, length);
}

void USB_print_char(const unsigned char ch) {
	char buffer[1];
	
	buffer[0] = (ch & 0x1FF);
	USBWrite(buffer, 1);
}

void USB_send(const char* buffer, unsigned int length) {
	USBWrite(buffer, length);
}

// Returns the total number of bytes read.
int USBRead_nonblocking(char *buf, unsigned int len) {
#ifndef DEBUG_X86
  AT91PS_UDP pUdp = pCDC.pUdp;
  int packetSize;
  int nbBytesRcv = 0;
  unsigned int bank = pCDC.currentRcvBank;

  if (!pCDC.IsConfigured(&pCDC))
    return 0;

  // UDP_CSR : UDP Control and status register
  // bank : Points to the currently used receive bank. In endpoints with
  //        ping-pong attributes, two receivebanks are used.
  while (1) {
    if (pUdp->UDP_CSR[AT91C_EP_OUT] & bank) {   // Check the currently used databank for pending data
      packetSize = pUdp->UDP_CSR[AT91C_EP_OUT] >> 16;
      if (packetSize > len)
        return nbBytesRcv;
      len -= packetSize;
      while (packetSize--)
        buf[nbBytesRcv++] = pUdp->UDP_FDR[AT91C_EP_OUT];
      pUdp->UDP_CSR[AT91C_EP_OUT] &= ~(bank);   // Notify USB device that data has been read
      if (bank == AT91C_UDP_RX_DATA_BK0)        // Read the other data bank next time
        bank = AT91C_UDP_RX_DATA_BK1;
      else
        bank = AT91C_UDP_RX_DATA_BK0;
      pCDC.currentRcvBank = bank;
    } else {
      return nbBytesRcv;
    }
  }
#endif
  return 0;
}