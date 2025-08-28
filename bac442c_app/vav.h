#ifndef __VAV_H__
#define __VAV_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mtimer.h"
#include "alarm.h"

void VAV_GetFlowAlarmState(void);
void VAV_ControlProc(void);
void VAV_OneSec(void);
void VAV_Init(void);

extern uint16_t tmr_panel_comm;
extern ALARM_SET Alm_SAF_Lo_70;
extern ALARM_SET Alm_SAF_Lo_50;
extern ALARM_SET Alm_DAT_Lo_17;
extern ALARM_SET Alm_DAT_Lo_8;
extern ALARM_SET Alm_DAT_Hi;

extern ALARM_SET Flt_Flw_Sensor;
extern ALARM_SET Flt_Dmp_Leak;
extern ALARM_SET Flt_Vlv_Leak;

extern uint16_t lc_ext_sec;

#endif

