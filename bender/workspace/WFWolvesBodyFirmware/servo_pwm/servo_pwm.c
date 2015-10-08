/*
 * servo_pwm.c
 *
 *  Created on: 16.04.2010
 *      Author: Stefan
 */

#include "servo_pwm.h"
#include "include/Board.h"

// MIN ~ 0.01 / (Prescaler / MCK)
#define MIN_DUTY_CYCLE	1500
// MAX ~ 0.01 / (Prescaler / MCK)
#define MAX_DUTY_CYCLE	3000

void ServoPWM_Init(void) {
#ifndef DEBUG_X86
	// Enable clock for interface
	AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PWMC;

	// Set second functionality of pin
	AT91C_BASE_PIOB->PIO_PDR = AT91C_PIO_PB20;
	AT91C_BASE_PIOB->PIO_ASR = AT91C_PIO_PB20;
	AT91C_BASE_PIOB->PIO_BSR = 0;

	// Set second functionality of pin
	AT91C_BASE_PIOB->PIO_PDR = AT91C_PIO_PB21;
	AT91C_BASE_PIOB->PIO_ASR = AT91C_PIO_PB21;
	AT91C_BASE_PIOB->PIO_BSR = 0;

	// Enable clock for PWM
	AT91C_BASE_PWMC->PWMC_MR = 0x00010001;

	// Period for PWM
	// Period ~ 20ms
	// CPRD ~ (Period * MCK) / Prescaler
	AT91C_BASE_PWMC_CH1->PWMC_CPRDR = 30000;
	AT91C_BASE_PWMC_CH2->PWMC_CPRDR = 30000;

	// Duty for PWM
	AT91C_BASE_PWMC_CH1->PWMC_CDTYR = MIN_DUTY_CYCLE;
	AT91C_BASE_PWMC_CH2->PWMC_CDTYR = MIN_DUTY_CYCLE;

	// Prescaler = 32
	// Waveform starts at high level
	// Modify the duty cycle at the next period start event
	AT91C_BASE_PWMC_CH1->PWMC_CMR = 0x0205;
	AT91C_BASE_PWMC_CH2->PWMC_CMR = 0x0205;

	// Enable PWM channel
	AT91C_BASE_PWMC->PWMC_ENA = (1 << 1) | (1 << 2);
#endif
}

void ServoPWM_SendPosition(unsigned char id, int pos) {
#ifndef DEBUG_X86
	int val = MIN_DUTY_CYCLE + ((float)(MAX_DUTY_CYCLE - MIN_DUTY_CYCLE) / 255.0f) * (float)pos;

	if (id == 0) {
		AT91C_BASE_PWMC_CH1->PWMC_CUPDR = val;
	} else if (id == 1) {
		AT91C_BASE_PWMC_CH2->PWMC_CUPDR = val;
	}
#endif
}
