// ----------------------------------------------------------------------------
//         ATMEL Microcontroller Software Support  -  ROUSSET  -
// ----------------------------------------------------------------------------
// DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
// DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
// OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ----------------------------------------------------------------------------
// File Name           : interrupt_Usart.c
// Object              : USART Interrupt Management
// Creation            : JPP   08-Sep-2005
// ----------------------------------------------------------------------------

#ifndef DEBUG_X86

// Include Standard LIB  files
#include "uart.h"
#include "include/Board.h"
#include "include/lib_AT91SAM7XC256.h"

#define USART0_INTERRUPT_LEVEL		7
#define USART1_INTERRUPT_LEVEL		8

AT91PS_USART COM0 = AT91C_BASE_US0;
AT91PS_USART COM1 = AT91C_BASE_US1;

CharReceivedCallback uart0_CharReceivedCallbackFnct = 0;
CharReceivedCallback uart1_CharReceivedCallbackFnct = 0;
TxReadyCallback uart0_TxBufferEmptyCallbackFnct = 0;

//* \fn    AT91F_US_Baudrate
//* \brief Calculate the baudrate
//* Standard Asynchronous Mode : 8 bits , 1 stop , no parity
#define AT91C_US_ASYNC_MODE ( AT91C_US_USMODE_NORMAL + \
                        AT91C_US_NBSTOP_1_BIT + \
                        AT91C_US_PAR_NONE + \
                        AT91C_US_CHRL_8_BITS + \
                        AT91C_US_CLKS_CLOCK )

// Prototypes
void Usart0IRQHandler(void);
void Usart1IRQHandler(void);

void UART_Init(void) {
	// Configure PIO controllers to periph mode
	AT91F_PIO_CfgPeriph(AT91C_BASE_PIOA, ((unsigned int) AT91C_PA0_RXD0)
			| ((unsigned int) AT91C_PA1_TXD0) | ((unsigned int) AT91C_PA3_RTS0)
			| ((unsigned int) AT91C_PA4_CTS0), // Peripheral A
			0); // Peripheral B

	// Configure PIO controllers to periph mode
	AT91F_PIO_CfgPeriph(AT91C_BASE_PIOA, ((unsigned int) AT91C_PA5_RXD1)
			| ((unsigned int) AT91C_PA6_TXD1) | ((unsigned int) AT91C_PA8_RTS1)
			| ((unsigned int) AT91C_PA9_CTS1), // Peripheral A
			0); // Peripheral B

	// First, enable the clock of the USART
	AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << AT91C_ID_US0);
	AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << AT91C_ID_US1);
	// Usart Configure
	AT91F_US_Configure(COM0, MCK, AT91C_US_ASYNC_MODE, AT91_BAUD_RATE_EXT, 0);
	AT91F_US_Configure(COM1, MCK, AT91C_US_ASYNC_MODE, AT91_BAUD_RATE_SERVO, 0);

	// Enable usart
	COM0->US_CR = AT91C_US_RXEN | AT91C_US_TXEN;
	COM1->US_CR = AT91C_US_RXEN | AT91C_US_TXEN;

	// Enable USART IT error and RXRDY
	AT91F_US_EnableIt(COM0, AT91C_US_TIMEOUT | AT91C_US_FRAME | AT91C_US_OVRE | AT91C_US_RXRDY);
	AT91F_US_EnableIt(COM1, AT91C_US_TIMEOUT | AT91C_US_FRAME | AT91C_US_OVRE | AT91C_US_RXRDY);

	// open Usart 0 interrupt
	AT91F_AIC_ConfigureIt(AT91C_BASE_AIC, AT91C_ID_US0, USART0_INTERRUPT_LEVEL,
			AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, Usart0IRQHandler);

	// open Usart 1 interrupt
	AT91F_AIC_ConfigureIt(AT91C_BASE_AIC, AT91C_ID_US1, USART1_INTERRUPT_LEVEL,
			AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, Usart1IRQHandler);
}

void Usart0IRQHandler(void) {
	AT91PS_USART USART_pt = AT91C_BASE_US0;
	unsigned int status;

	// get Usart status register
	status = USART_pt->US_CSR;
	if ((status & AT91C_US_RXRDY)  && (USART_pt->US_IMR & AT91C_US_RXRDY)) {
		if (uart0_CharReceivedCallbackFnct != 0) {
			char ch = USART_pt->US_RHR;
			uart0_CharReceivedCallbackFnct(ch);
		}
	}
	if ((status & AT91C_US_TXRDY) && (USART_pt->US_IMR & AT91C_US_TXRDY)) {
		if (uart0_TxBufferEmptyCallbackFnct != 0) {
			unsigned char ret = uart0_TxBufferEmptyCallbackFnct();
			if (ret == 0) {
				USART_pt->US_IDR = AT91C_US_TXRDY; // Disable interrupt
			}
		}
	}

	if (status & AT91C_US_OVRE) {
		// clear US_RXRDY
	}

	// Check error
	if (status & AT91C_US_PARE) {
		AT91F_US_PutChar(USART_pt, 'P');
	}

	if (status & AT91C_US_FRAME) {
		AT91F_US_PutChar(USART_pt, 'F');
	}

	if (status & AT91C_US_TIMEOUT) {
		USART_pt->US_CR = AT91C_US_STTTO;
		AT91F_US_PutChar(USART_pt, 'T');
	}

	// Reset the status bit
	USART_pt->US_CR = AT91C_US_RSTSTA;
}

void Usart1IRQHandler(void) {
	AT91PS_USART USART_pt = AT91C_BASE_US1;
	unsigned int status;

	// get Usart status register
	status = USART_pt->US_CSR;
	if (status & AT91C_US_RXRDY) {
		if (uart1_CharReceivedCallbackFnct != 0) {
			char ch = USART_pt->US_RHR;
			uart1_CharReceivedCallbackFnct(ch);
		}
	}

	if (status & AT91C_US_OVRE) {
		// clear US_RXRDY
		AT91F_US_GetChar(USART_pt);
		AT91F_US_PutChar(USART_pt, 'O');
	}

	// Check error
	if (status & AT91C_US_PARE) {
		AT91F_US_PutChar(USART_pt, 'P');
	}

	if (status & AT91C_US_FRAME) {
		AT91F_US_PutChar(USART_pt, 'F');
	}

	if (status & AT91C_US_TIMEOUT) {
		USART_pt->US_CR = AT91C_US_STTTO;
		AT91F_US_PutChar(USART_pt, 'T');
	}

	// Reset the status bit
	USART_pt->US_CR = AT91C_US_RSTSTA;
}

int uart0_getc(void) {
  while (!(COM0->US_CSR & AT91C_US_RXRDY));   // Wait for Full Rx Buffer
  return (COM0->US_RHR);                      // Read Character
}

void uart0_print_ascii(const char *buffer) {
    while (*buffer != '\0') {
        while (!(COM0->US_CSR & AT91C_US_TXRDY));
        COM0->US_THR = (*buffer++ & 0x1FF);
    }
}

int uart0_putc(int ch) {
	while (!(COM0->US_CSR & AT91C_US_TXRDY));
	AT91F_US_PutChar (COM0,ch);
	return ch;
}

int uart0_putchar (int ch) {
	if (ch == '\n')  {
		uart0_putc('\r');
	}
	return uart0_putc(ch);
}

void uart0_print_char_in_hex(unsigned char value) {
    unsigned char c1, c2;
    c1 = (value & 0x0F); // LSB
    c2 = (value & 0xF0) >> 4; //MSB
    uart0_print_ascii("0x"); // print the hex notation first
    uart0_print_4bit_in_hex(c2); // print LSB
    uart0_print_4bit_in_hex(c1); // print MSB
}

void uart0_print_4bit_in_hex(unsigned char value) {
    unsigned char c;
    c = (value & 0x0F); // LSB
    while (!(COM0->US_CSR & AT91C_US_TXRDY));
    COM0->US_THR = (c > 9 ?  0x137+c : 0x130+c);
}

void uart0_print_digit(unsigned char value) {
    char c = 0;

    while (!(COM0->US_CSR & AT91C_US_TXRDY));
    c = (value & 0xF);
	COM0->US_THR = (('0' + c) & 0x1FF);
}

void uart0_print_hex8(unsigned long value) {
    char c = 0;
    char shift = sizeof(unsigned long) * 8;

    uart0_print_ascii("0x");
    do {
        shift -= 4;
        while (!(COM0->US_CSR & AT91C_US_TXRDY));
        c = ((value >> shift) & 0xF);
        if (c > 9)
	  COM0->US_THR = (('A' + (c - 10)) & 0x1FF);
        else
	  COM0->US_THR = (('0' + c) & 0x1FF);
    } while (shift != 0);
}

void uart0_print_int(int zahl){
	if(zahl<=0){
		uart0_putchar('-');
		zahl = zahl * -1;
	}

	int mod = 10, durch = 1;
	long int temp;
	//Variablen der Längenerkenung
	int laenge = 0;

	//Ermittlung der anzahl der Stellen in der Variabl Bsp: 123 -> 3 Stellen
	do {
		laenge++;
		durch = durch * 10;
	} while (zahl / durch > 0);
	durch = 1;   //Reset der "durch" Variable

	//Ausgabe der einzelnen Stellenwerte, von links nach rechts (Für USART oder der Gleichen)
	int i, j, modtemp, durchtemp;
	for(i = laenge; i > 0; i--) {
		modtemp = mod;
		durchtemp = durch;
		for (j = 1; j < i; j++) { //Berechnen des Modulu und des Dividenden
			modtemp *= 10;
			durchtemp *= 10;
		}
		temp = (zahl % modtemp) / durchtemp;
		uart0_putchar(temp + 48); //Ausgabe
	}
}

CharReceivedCallback uart0_AssignCharReceivedCallback(CharReceivedCallback callback) {
	 CharReceivedCallback temp;

	 temp = uart0_CharReceivedCallbackFnct;
	 uart0_CharReceivedCallbackFnct = callback;
	 if (callback == 0) {
			AT91F_AIC_DisableIt(AT91C_BASE_AIC, AT91C_ID_US0);
	 } else {
			AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_US0);
	 }
	 return temp;
}

void uart0_SetBaudrate(int baudrate) {
	AT91F_US_Configure(COM0, MCK, AT91C_US_ASYNC_MODE, baudrate, 0);
	COM0->US_CR = AT91C_US_RXEN | AT91C_US_TXEN;
	AT91F_US_EnableIt(COM0, AT91C_US_TIMEOUT | AT91C_US_FRAME | AT91C_US_OVRE | AT91C_US_RXRDY);
}

int uart1_getc(void) {
	while (!(COM1->US_CSR & AT91C_US_RXRDY));   // Wait for Full Rx Buffer
	return (COM1->US_RHR);                      // Read Character
}


void uart1_print_ascii(const char *buffer) {
    while (*buffer != '\0') {
        while (!(COM1->US_CSR & AT91C_US_TXRDY));
        COM1->US_THR = (*buffer++ & 0x1FF);
    }
}

int uart1_putc(int ch) {
	while (!(COM1->US_CSR & AT91C_US_TXRDY));
	AT91F_US_PutChar (COM1,ch);
	return ch;
}

int uart1_putchar (int ch) {
  if (ch == '\n')  {
    uart1_putc('\r');
  }
  return uart1_putc(ch);
}

void uart1_waitforsent(void) {
	while (!(COM1->US_CSR & AT91C_US_TXEMPTY));
}

unsigned char uart1_char_available(void) {
	return (COM1->US_CSR & AT91C_US_RXRDY) > 0;
}

void uart1_SetBaudrate(int baudrate) {
	AT91F_US_Configure(COM1, MCK, AT91C_US_ASYNC_MODE, baudrate, 0);
	COM1->US_CR = AT91C_US_RXEN | AT91C_US_TXEN;
	AT91F_US_EnableIt(COM1, AT91C_US_TIMEOUT | AT91C_US_FRAME | AT91C_US_OVRE | AT91C_US_RXRDY);
}

CharReceivedCallback uart1_AssignCharReceivedCallback(CharReceivedCallback callback) {
	 CharReceivedCallback temp;

	 temp = uart1_CharReceivedCallbackFnct;
	 uart1_CharReceivedCallbackFnct = callback;
	 if (callback == 0) {
			AT91F_AIC_DisableIt(AT91C_BASE_AIC, AT91C_ID_US1);
	 } else {
			AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_US1);
	 }
	 return temp;
}

TxReadyCallback uart0_AssignTxReadyCallback(TxReadyCallback callback) {
	 TxReadyCallback temp;

	 temp = uart0_TxBufferEmptyCallbackFnct;
	 uart0_TxBufferEmptyCallbackFnct = callback;
	 if (callback == 0) {
			COM0->US_IDR = AT91C_US_TXRDY;
			if (uart0_CharReceivedCallbackFnct == 0) { // Only disable interrupt when no other callbacks are set
				AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_US0;
			}
	 } else {
			COM0->US_IER = AT91C_US_TXRDY;
			AT91C_BASE_AIC->AIC_IECR = 1 << AT91C_ID_US0;
	 }
	 return temp;
}

void uart0_enable_TxReadyCallback() {
	if (uart0_TxBufferEmptyCallbackFnct != 0) {
		COM0->US_IER = AT91C_US_TXRDY;
	}
}

// Nothing to do here for a char-based UART device
void uart1_hintStartOfTransfer(void) {
}
void uart1_hintEndOfTransfer(void) {
}

#endif
