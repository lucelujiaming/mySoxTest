#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct _TIMER
{
	uint32_t ticks_value;
	uint32_t ticks_limit;
	bool timer_repeat;
	bool ticks_en;
	struct _TIMER *next;
}MTIMER;

#define TIMER_MAX	16
 
/////////////////////////////////////////////////////////////////
void Timer_SetParam(MTIMER *timer,bool repeat,uint32_t limit);
void Timer_Start(MTIMER *timer);
void Timer_Stop(MTIMER *timer);
void Timer_Restart(MTIMER *timer);
bool Timer_Expires(MTIMER *timer);

/////////////////////////////////////////////////////////
bool Timer_Init(MTIMER *timer);
void Timer_TicksInc(void);

#endif
