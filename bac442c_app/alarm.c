
#include <stdint.h>
#include "alarm.h"

void Alarm_Init(ALARM_SET *alm_set, ALARM_LEVEL lvl, uint16_t entry_dly, uint16_t exit_dly)
{
	if(exit_dly== 0)
		exit_dly= DEF_ALM_EXIT_HYST;
	
	alm_set->entry_delay_set= entry_dly;
	alm_set->exit_delay_set= exit_dly;
	
	alm_set->alarm_st= 0;
	alm_set->alm_level= lvl;

	alm_set->tmr_entry_delay= 0;
	alm_set->tmr_exit_delay= 0;
}

void Alarm_Clear(ALARM_SET *alm_set)
{
	alm_set->alarm_st= 0;
	alm_set->tmr_entry_delay= 0;
	alm_set->tmr_exit_delay= 0;
	alm_set->last_alarm= 0;
}

// Invoked Per Second
int8_t Alarm_Check(ALARM_SET *alm_set, int8_t alm)
{
	if(alm_set->tmr_entry_delay)
	{
		if(--alm_set->tmr_entry_delay== 0)
		{
			alm_set->alarm_st= 1;
		}
	}
	
	if(alm_set->tmr_exit_delay)
	{
		if(--alm_set->tmr_exit_delay== 0)
		{
			alm_set->alarm_st= 0;
		}
	}
	
	//
	if(alm != alm_set->last_alarm)
	{
		if(alm)
		{
			if(alm_set->entry_delay_set)
			{
				alm_set->tmr_entry_delay= alm_set->entry_delay_set;
			}
			else
			{
				alm_set->alarm_st= 1;
			}
			alm_set->tmr_exit_delay= 0;
		}
		else
		{
			if(alm_set->alm_level== LVL_URGENT
				|| alm_set->alm_level==LVL_NORMAL)
			{
				if(alm_set->exit_delay_set)
				{
					alm_set->tmr_exit_delay= alm_set->exit_delay_set;
				}
				else
				{
					alm_set->alarm_st= 0;
				}
				
				alm_set->tmr_entry_delay= 0;
			}
		}
	}
	
	alm_set->last_alarm= alm; 
	
	return alm_set->alarm_st;
}

