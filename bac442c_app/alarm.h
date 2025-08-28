#ifndef __ALARM_H__
#define __ALARM_H__

#include <stdint.h>

#define DEF_ALM_EXIT_HYST		5

#define DEF_ALM_ENTRY_DLY_LVL_1		1
#define DEF_ALM_ENTRY_DLY_LVL_2		10
#define DEF_ALM_ENTRY_DLY_LVL_3		60
#define DEF_ALM_ENTRY_DLY_LVL_4		300

typedef enum
{
	LVL_NONE= 0,
	LVL_LIFE_SAFETY= 1,
	LVL_CRITICAL_EQUIPMENT= 2,
	LVL_URGENT= 3,
	LVL_NORMAL= 4,
}ALARM_LEVEL;


typedef struct
{
	ALARM_LEVEL alm_level;
	
	uint8_t alarm_st;
	uint8_t last_alarm;
	
	uint16_t entry_delay_set;
	uint16_t exit_delay_set;
	
	uint16_t tmr_entry_delay;
	uint16_t tmr_exit_delay;
}ALARM_SET;	

void Alarm_Init(ALARM_SET *alm_set, 
								ALARM_LEVEL lvl, 
								uint16_t entry_dly, uint16_t exit_dly);

void Alarm_Clear(ALARM_SET *alm_set);
int8_t Alarm_Check(ALARM_SET *alm_set, int8_t alm);

void Alarm_Generate_Init(ALARM_SET *alm_set, 
												uint16_t entry_dly, uint16_t exit_dly);
int8_t Alarm_Generate_Check(ALARM_SET *alm_set, int8_t alm);
#endif
