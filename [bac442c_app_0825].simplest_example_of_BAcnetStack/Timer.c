
#include "Timer.h"
volatile MTIMER *TimerHead;

void Timer_SetParam(MTIMER * timer,bool repeat,uint32_t limit)
{
	timer->timer_repeat= repeat;
	timer->ticks_value= 0;
	timer->ticks_limit= limit;
}

void Timer_Start(MTIMER * timer)
{
	timer->ticks_en= 1;
}

void Timer_Stop(MTIMER * timer)
{
	timer->ticks_en= 0;
}

void Timer_Restart(MTIMER * timer)
{
	timer->ticks_value= 0;
	timer->ticks_en= 1;
}

bool Timer_Expires(MTIMER * timer)
{
	if(timer->ticks_en && (timer->ticks_value>= timer->ticks_limit))
	{
		if(timer->timer_repeat== 0)
		{
			timer->ticks_en= 0;
		}
		timer->ticks_value= 0;
		return true;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////
bool Timer_Init(MTIMER * timer)
{
	MTIMER *node;
	
	node= (MTIMER*)TimerHead;
	timer->next= NULL;
	
	
	if(TimerHead== NULL)
	{
		TimerHead= timer;
	}
	else
	{
		while(node->next)
		{
			node= node->next;
		}
		node->next= timer;
	}
	
	return false;
}

void Timer_TicksInc(void)
{
	MTIMER *node;
	
	node= (MTIMER*)TimerHead;
	
	while(node)
	{
		if(node->ticks_en)
		{
			if(node->ticks_value < node->ticks_limit)
			{
				node->ticks_value++;
			}
		}
		node= node->next;
	}
}


