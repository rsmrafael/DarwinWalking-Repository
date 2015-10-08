#ifndef TC_H_
#define TC_H_

#define TIMER_CYCLES_PER_MS 47
#define TIMER0_RATE 		 1 * TIMER_CYCLES_PER_MS
#define TIMER1_RATE 		 1 * TIMER_CYCLES_PER_MS
#define TIMER2_RATE 		 1 * TIMER_CYCLES_PER_MS

typedef void (*TimerCallback)(void);

void TC_Init(void);

void TC_DelayMs(unsigned int ms);
unsigned long TC_GetSystemTicks(void);
unsigned int TC_GetMsSinceTick(unsigned long sinceTick);
void TC_SetTimer1Interval(unsigned int intervalMs);
void TC_SetTimer2Interval(unsigned int intervalMs);
TimerCallback TC_AssignTimer1Callback(TimerCallback callback);
TimerCallback TC_AssignTimer2Callback(TimerCallback callback);
void TC_SetInterruptState(unsigned char state);

#endif /*TC_H_*/
