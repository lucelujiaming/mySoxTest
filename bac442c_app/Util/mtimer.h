
#ifndef __MTIMER_H__
#define __MTIMER_H__

#include <stdint.h>

typedef struct _MTIMER
{
	uint8_t start;
	uint8_t repeat;

	uint16_t msec;
	uint16_t limit;
}MTIMER;

uint16_t Timer_Eslaped(uint16_t cur, uint16_t last);
uint16_t Timer_Eslaped_MilliSec(uint16_t last); 
uint16_t Timer_Eslaped_Sec(uint16_t last); 
void Timer_SetLimit(MTIMER *tm, uint16_t lmt);

void Timer_Init(MTIMER *tm, uint8_t repeat, uint16_t limit);
void Timer_Start(MTIMER *tm);
void Timer_Stop(MTIMER *tm);
uint8_t Timer_Expires(MTIMER *tm);

uint16_t Timer_GetSeconds(void);
uint16_t Timer_GetMilliSeconds(void);

extern volatile uint16_t tm_milli_second;
extern volatile uint16_t tm_second;
#endif
