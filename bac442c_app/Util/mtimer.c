
#include <stdint.h>
#include <stdlib.h>
#include "mtimer.h"

volatile uint16_t tm_milli_second;
volatile uint16_t tm_second;

uint16_t Timer_Eslaped(uint16_t cur, uint16_t last) 
{
	return (uint16_t)((cur + 65536 - last)%65536);
}

uint16_t Timer_Eslaped_MilliSec(uint16_t last) 
{
	return (uint16_t)((tm_milli_second + 65536 - last)%65536);
}

uint16_t Timer_Eslaped_Sec(uint16_t last) 
{
	return (uint16_t)((tm_second + 65536 - last)%65536);
}

void Timer_Init(MTIMER *tm, uint8_t repeat, uint16_t limit)
{
	tm->repeat= repeat;
	tm->limit= limit;
} 

void Timer_Start(MTIMER *tm)
{
	tm->msec= Timer_GetMilliSeconds();
	tm->start= 1;
}

void Timer_Stop(MTIMER *tm)
{
	tm->start= 0;
}

void Timer_SetLimit(MTIMER *tm, uint16_t lmt)
{
	tm->limit= lmt;
}

uint8_t Timer_Expires(MTIMER *tm)
{
	if(tm->start && (Timer_Eslaped_MilliSec(tm->msec) >= tm->limit))
	{
		if(tm->repeat)
		{
			tm->msec= Timer_GetMilliSeconds();
		}
		else
		{
			tm->start= 0;
		}
		return 1;
	}

	return 0;
}

uint16_t Timer_GetSeconds(void)
{
	return tm_second;
}

uint16_t Timer_GetMilliSeconds(void)
{
	return tm_milli_second;
}

