#include "tc.h"
#include "../include/Board.h"

#ifdef DEBUG_X86
#include <sys/time.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#endif

static volatile unsigned int tc_delayRemaining = 0;
volatile unsigned long tc_systemTicks;
TimerCallback tc_timerCallbackFnct = 0;
TimerCallback tc_timerCallbackFnct2 = 0;

//Prototypes
void Timer0ISR(void);
void Timer1ISR(void);
void Timer2ISR(void);

void TC_Init(void) {
#ifndef DEBUG_X86
	// Enable timer clock
	AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TC0;
	AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TC1;
	AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TC2;

	// Disable the interrupt on the interrupt controller
	AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_TC0;
	AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_TC1;
	AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_TC2;

	// Save the interrupt handler routine pointer
	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_TC0] = (unsigned int)Timer0ISR;
	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_TC1] = (unsigned int)Timer1ISR;
	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_TC2] = (unsigned int)Timer2ISR;

	// Store the Source Mode Register
	// 4 PRIORITY is random
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_TC0] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 4;
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_TC1] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 3;
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_TC2] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 3;

	// Clear the interrupt on the interrupt controller
	AT91C_BASE_AIC->AIC_ICCR = 1 << AT91C_ID_TC0;
	AT91C_BASE_AIC->AIC_ICCR = 1 << AT91C_ID_TC1;
	AT91C_BASE_AIC->AIC_ICCR = 1 << AT91C_ID_TC2;

	// Set up the timer. We want just the basics, except when the timer compares
	// with RC, retrigger
	//
	// MCK is 48054500 (47923200)
	// DIV5: A tick MCK/1024 times a second
	// This makes every tick every 21.309us => appr. 47 ticks per ms
	// CPCTRG makes the RC event reset the counter and trigger it to restart
	AT91C_BASE_TC0->TC_CMR = AT91C_TC_CLKS_TIMER_DIV5_CLOCK | AT91C_TC_CPCTRG;
	AT91C_BASE_TC1->TC_CMR = AT91C_TC_CLKS_TIMER_DIV5_CLOCK | AT91C_TC_CPCTRG;
	AT91C_BASE_TC2->TC_CMR = AT91C_TC_CLKS_TIMER_DIV5_CLOCK | AT91C_TC_CPCTRG;

	// Only really interested in interrupts when the RC happens
	AT91C_BASE_TC0->TC_IDR = 0xFF; 				// Disable all interrupts
	AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS; 	// Enable compare interrupt
	AT91C_BASE_TC1->TC_IDR = 0xFF; 				// Disable all interrupts
	AT91C_BASE_TC1->TC_IER = AT91C_TC_CPCS; 	// Enable compare interrupt
	AT91C_BASE_TC2->TC_IDR = 0xFF; 				// Disable all interrupts
	AT91C_BASE_TC2->TC_IER = AT91C_TC_CPCS; 	// Enable compare interrupt

	// load the RC value
	AT91C_BASE_TC0->TC_RC = TIMER0_RATE;
	AT91C_BASE_TC1->TC_RC = TIMER1_RATE;
	AT91C_BASE_TC2->TC_RC = TIMER2_RATE;

	// Enable the interrupt
	AT91C_BASE_AIC->AIC_IECR = 1 << AT91C_ID_TC0;
	AT91C_BASE_AIC->AIC_IECR = 1 << AT91C_ID_TC1;
	AT91C_BASE_AIC->AIC_IECR = 1 << AT91C_ID_TC2;

	// Enable the device
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
	AT91C_BASE_TC1->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
	AT91C_BASE_TC2->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
#endif
}

void Timer0ISR(void) {
#ifndef DEBUG_X86
	if (AT91C_BASE_TC0->TC_SR & AT91C_TC_CPCS) {
		if (tc_timerCallbackFnct != 0) {
			tc_timerCallbackFnct();
		}
	}
#endif
}

void Timer1ISR(void) {
#ifndef DEBUG_X86
	if (AT91C_BASE_TC1->TC_SR & AT91C_TC_CPCS) {
		tc_systemTicks++;				// Will overflow after 49.x days - possible problem, but not for our use case.

		unsigned int n = tc_delayRemaining;						//1000Hz decrement timer
		if (n) tc_delayRemaining = --n;
	}
#endif
}

void Timer2ISR(void) {
#ifndef DEBUG_X86
	if (AT91C_BASE_TC2->TC_SR & AT91C_TC_CPCS) {
		if (tc_timerCallbackFnct2 != 0) {
			tc_timerCallbackFnct2();
		}
	}
#endif
}

void TC_DelayMs(unsigned int ms) {
#ifndef DEBUG_X86
	tc_delayRemaining = ms;
	while (tc_delayRemaining);
#else
#ifdef WIN32
	Sleep(ms);
#else
	usleep(1000 * ms);
#endif
#endif
}

unsigned long TC_GetSystemTicks(void) {
#ifndef DEBUG_X86
	return tc_systemTicks;
#else
	struct timeval tme;
	gettimeofday(&tme, 0);

	return (tme.tv_sec * 1000) + (tme.tv_usec / 1000);
#endif
}

unsigned int TC_GetMsSinceTick(unsigned long sinceTick) {
#define MAX_UNSIGNEDLONG 0xFFFFFFFF
	if (sinceTick == 0) {
		return 0;
	}
	unsigned long curTick = TC_GetSystemTicks();
	if (curTick < sinceTick) {
		return (MAX_UNSIGNEDLONG - sinceTick) + curTick;
	}
	return curTick - sinceTick;
}

void TC_SetTimer1Interval(unsigned int intervalMs) {
#ifndef DEBUG_X86
	AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_TC0; // Disable interrupt
	AT91C_BASE_TC0->TC_RC = TIMER0_RATE * intervalMs;
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_SWTRG; // Reset counter value
	AT91C_BASE_AIC->AIC_ICCR = 1 << AT91C_ID_TC0; // Clear interrupt
	AT91C_BASE_AIC->AIC_IECR = 1 << AT91C_ID_TC0; // Enable interrupt
#endif
}

void TC_SetTimer2Interval(unsigned int intervalMs) {
#ifndef DEBUG_X86
	AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_TC2; // Disable interrupt
	AT91C_BASE_TC2->TC_RC = TIMER2_RATE * intervalMs;
	AT91C_BASE_TC2->TC_CCR = AT91C_TC_SWTRG; // Reset counter value
	AT91C_BASE_AIC->AIC_ICCR = 1 << AT91C_ID_TC2; // Clear interrupt
	AT91C_BASE_AIC->AIC_IECR = 1 << AT91C_ID_TC2; // Enable interrupt
#endif
}

TimerCallback TC_AssignTimer1Callback(TimerCallback callback) {
	 TimerCallback temp;

	 temp = tc_timerCallbackFnct;
	 tc_timerCallbackFnct = callback;
	 return temp;
}

TimerCallback TC_AssignTimer2Callback(TimerCallback callback) {
	 TimerCallback temp;

	 temp = tc_timerCallbackFnct2;
	 tc_timerCallbackFnct2 = callback;
	 return temp;
}

void TC_SetInterruptState(unsigned char state) {
#ifndef DEBUG_X86
	if (state == 0) {
		// Disable the interrupts
		AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_TC0;
		AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_TC2;
	} else {
		// Enable the interrupts - pending interrupts will be executed
		AT91C_BASE_AIC->AIC_IECR = 1 << AT91C_ID_TC0;
		AT91C_BASE_AIC->AIC_IECR = 1 << AT91C_ID_TC2;
	}
#endif
}
