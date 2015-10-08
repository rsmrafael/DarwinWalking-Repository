#include "adc.h"
#include "../include/Board.h"

void ADC_Init(void) {
#ifndef DEBUG_X86
	// Enable clock for interface
	AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_ADC;

	// Reset
	AT91C_BASE_ADC->ADC_CR = 0x1;
	AT91C_BASE_ADC->ADC_CR = 0x0;

	// Setup ADC
	AT91C_BASE_ADC->ADC_MR = AT91C_ADC_TRGEN_DIS | AT91C_ADC_LOWRES_10_BIT | AT91C_ADC_SLEEP_NORMAL_MODE | AT91C_ADC_PRESCAL | AT91C_ADC_STARTUP | AT91C_ADC_SHTIM; // software trigger, 10 bits, normal mode, max time
	AT91C_BASE_ADC->ADC_CHER = AT91C_ADC_CH0;
	AT91C_BASE_ADC->ADC_CHER = AT91C_ADC_CH1;
	AT91C_BASE_ADC->ADC_CHER = AT91C_ADC_CH2;
	AT91C_BASE_ADC->ADC_CHER = AT91C_ADC_CH3;
	AT91C_BASE_ADC->ADC_CHER = AT91C_ADC_CH4;
	AT91C_BASE_ADC->ADC_CHER = AT91C_ADC_CH5;
	AT91C_BASE_ADC->ADC_CHER = AT91C_ADC_CH6;
	AT91C_BASE_ADC->ADC_CHER = AT91C_ADC_CH7;
#endif
}


unsigned int ADC_GetChannel(unsigned char channel) {
#ifndef DEBUG_X86
	// Start conversion
	AT91C_BASE_ADC->ADC_CR = 0x2;

	// wait for end of conversion (of last channel)
	while(!(AT91C_BASE_ADC->ADC_SR & AT91C_ADC_EOC7));

	switch (channel) {
		case ADC_CHN_1: return AT91C_BASE_ADC->ADC_CDR0;
		case ADC_CHN_2: return AT91C_BASE_ADC->ADC_CDR1;
		case ADC_CHN_3: return AT91C_BASE_ADC->ADC_CDR2;
		case ADC_CHN_4: return AT91C_BASE_ADC->ADC_CDR3;
		case ADC_CHN_5: return AT91C_BASE_ADC->ADC_CDR4;
		case ADC_CHN_6: return AT91C_BASE_ADC->ADC_CDR5;
		case ADC_CHN_7: return AT91C_BASE_ADC->ADC_CDR6;
		case ADC_CHN_8: return AT91C_BASE_ADC->ADC_CDR7;
	}
#endif
	return 0;
}
