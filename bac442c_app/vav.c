
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pressflow.h"
#include "actuator.h"
#include "mtimer.h"
#include "vav.h"
#include "actuator.h"
#include "hal_i2c1.h"
#include "drv_sdp800.h" 
#include "hal_relay.h"
#include "common.h"
#include "modbus.h"
#include "mb_var_def.h"
#include "pid.h"
#include "thermistor.h"

#include "hal_occ.h"
#include "hal_adc1_dma.h"
// #include "hal_switch.h"
#include "hal_pwm_tim1_oc1.h"

#include "alarm.h"
#include "variables.h"
#include "task_config.h"

#define DELAY_PID_OFF		30
#define DELAY_SAT				120
#define DELAY_FAN_OFF		120

extern float power_scale;

#define PID_TEMP_TS		(1.0f)
#define PID_DMP_TS		(0.2f)

PID_SET PID_Clg= {0};
PID_SET PID_Htg= {0};
PID_SET PID_Dmp_Flow= {0};
PID_SET PID_DAT_Valve= {0};
PID_SET PID_DAT_Flow= {0};

uint16_t lc_pid_clg_zat;
uint16_t lc_pid_htg_zat;
uint16_t lc_pid_dat_vlv;		//Dat Reset Heater
uint16_t lc_pid_dat_flw;		//Dat Reset Flow

MTIMER tm_PID_Damper;
MTIMER tm_OneSecond;

ALARM_SET Alm_SAF_Lo_70;
ALARM_SET Alm_SAF_Lo_50; 
ALARM_SET Alm_DAT_Lo_17;
ALARM_SET Alm_DAT_Lo_8;
//ALARM_SET Flt_Fan_Start;
//ALARM_SET Flt_Fan_Stop;;
ALARM_SET Flt_Flw_Sensor;
ALARM_SET Flt_Dmp_Leak;
ALARM_SET Flt_Vlv_Leak;
ALARM_SET Alm_DAT_Hi;

uint16_t sat_delay;
HVAC_MODE Mode_AHU= MODE_NONE;
HVAC_MODE Mode_SAT= MODE_NONE;

float lc_clg_flow_setpt_max;
float lc_clg_flow_setpt_min;
float lc_htg_flow_setpt_max;
float lc_htg_flow_setpt_min;

float lc_act_pos_setpt;
uint16_t lc_flow_req;

uint16_t lc_fan_cmd;
uint16_t lc_htg_stage;
uint16_t lc_ext_sec;
uint16_t lc_htg_valve;

uint8_t lc_htg_flow_enable;
uint16_t lc_dat_setpt;
float lc_local_temp;
uint16_t lc_flow_co2;

uint16_t tmr_panel_comm;
uint16_t tmr_fan_on_mins;
uint16_t tmr_fan_off_mins;
uint16_t tmr_htg_stage_on_mins;
uint16_t tmr_htg_stage_off_mins;

uint16_t delay_fan_off;

uint16_t tmr_htg_flow_ready;

int16_t VAV_GetLocalTempInput(void)
{
	int16_t ret_value;
	float kohm;
	uint16_t adc;
	
	adc= adc_value[ADC_SENSOR_INPUT];
	if(adc > 4000)
		return 9999;
	
	kohm= adc*10;
	kohm/= 4095 - adc;
	ret_value= GetThermTemp(kohm, (NTC_TYPE_T)HoldingReg[HR_LOCAL_TEMP_TYPE], 100.0f, -20.0f)*10;
	
	return ret_value;
}

// Invoked One Second
// Cool Water Temp is too High
// Heat Water Temp is too Low
void VAV_SAT_Reset(void)
{
	static uint16_t tmr_30= 0, tmr_20= 0;
	static uint16_t tmr_time_avg_suppress= 0;  
	uint16_t avg_suppress_max;	
	
	if(HoldingReg[HR_AHU_OCC_REQ]== OCC_MODE_OCC)
	{
		if(Mode_AHU== MODE_COOL)
		{
			if(HoldingReg[HR_CLG_OUTPUT] > 950)
			{
				HoldingReg[HR_SAT_RST_REQ]= 1;
			}
			else
			{
				if(HoldingReg[HR_CLG_OUTPUT] < 850)
				{
					HoldingReg[HR_SAT_RST_REQ]= 0;
				}
			}
				
			if(HoldingReg[HR_CLG_OUTPUT]== 1000 
				&& _in_range_limit(HoldingReg[HR_ZONE_TEMP], 9990, 1))
			{
				if(HoldingReg[HR_ZONE_TEMP] > HoldingReg[HR_CLG_SETPT] + 20)
				{
					avg_suppress_max= abs(HoldingReg[HR_ZONE_TEMP] - HoldingReg[HR_CLG_SETPT])*54;		//9 minutes per dgree
				
					if(++tmr_time_avg_suppress > avg_suppress_max)
					{
						tmr_time_avg_suppress= avg_suppress_max;
					}
				
					if(++tmr_20 > 120)
					{
						tmr_20= 120;
						if(tmr_time_avg_suppress >= avg_suppress_max)
						{
							HoldingReg[HR_SAT_RST_REQ]= 2;
						}
					}
					
					if(HoldingReg[HR_ZONE_TEMP] > HoldingReg[HR_CLG_SETPT]  + 30)
					{
						if(++tmr_30 > 120)
						{
							tmr_30= 120;
							if(tmr_time_avg_suppress >= avg_suppress_max)
							{
								HoldingReg[HR_SAT_RST_REQ]= 3;
							}
						}
					}
					else
					{
						tmr_30= 0;
					}
				}
				else
				{
					tmr_time_avg_suppress= 0;
					tmr_20= 0;
				}
			}
			else
			{
				tmr_30= 0;
				tmr_20= 0;
				tmr_time_avg_suppress= 0;
			}
		}
		else
		{
			if(Mode_AHU== MODE_HEAT)
			{
				if(HoldingReg[HR_HTG_OUTPUT] > 950)
				{
					HoldingReg[HR_SAT_RST_REQ]= 1;
				}
				else
				{
					if(HoldingReg[HR_HTG_OUTPUT] < 850)
					{
						HoldingReg[HR_SAT_RST_REQ]= 0;
					}
				}
					
				if(HoldingReg[HR_HTG_OUTPUT]== 1000)
				{
					if(HoldingReg[HR_ZONE_TEMP] + 20 < HoldingReg[HR_HTG_SETPT])
					{
						avg_suppress_max= abs(HoldingReg[HR_ZONE_TEMP] - HoldingReg[HR_HTG_SETPT])*54;		//9 minutes per dgree
					
						if(++tmr_time_avg_suppress > avg_suppress_max)
						{
							tmr_time_avg_suppress= avg_suppress_max;
						}
					
						if(++tmr_20 > 120)
						{
							tmr_20= 120;
							if(tmr_time_avg_suppress >= avg_suppress_max)
							{
								HoldingReg[HR_SAT_RST_REQ]= 2;
							}
						}
						
						if(HoldingReg[HR_ZONE_TEMP] + 30 < HoldingReg[HR_HTG_SETPT])
						{
							if(++tmr_30 > 120)
							{
								tmr_30= 120;
								if(tmr_time_avg_suppress >= avg_suppress_max)
								{
									HoldingReg[HR_SAT_RST_REQ]= 3;
								}
							}
						}
						else
						{
							tmr_30= 0;
						}
					}
					else
					{
						tmr_time_avg_suppress= 0;
						tmr_20= 0;
					}
				}
				else
				{
					tmr_30= 0;
					tmr_20= 0;
					tmr_time_avg_suppress= 0;
				}
			}
			else
			{
				tmr_30= 0;
				tmr_20= 0;
				tmr_time_avg_suppress= 0;
				HoldingReg[HR_SAT_RST_REQ]= 0;
			}
		}
	}
	else
	{
		tmr_30= 0;
		tmr_20= 0;
		tmr_time_avg_suppress= 0;
		HoldingReg[HR_SAT_RST_REQ]= 0;
	}
}

// Invoked One Second
// Supply Air Press is too Low
void VAV_SAP_Reset(void)		
{
	static uint16_t tmr_50= 0, tmr_70= 0;
	
	if(HoldingReg[HR_FLOW_REQUIRE])
	{
		if(HoldingReg[HR_DAMPER_POSITION] > 950)
		{
			HoldingReg[HR_SAP_RST_REQ]= 1;
			
			if(HoldingReg[HR_DUCT_AIR_FLOW] < HoldingReg[HR_FLOW_REQUIRE]*0.7f)
			{
				if(++tmr_70 > 60)
				{
					tmr_70= 60;
					HoldingReg[HR_SAP_RST_REQ]= 2;
				}
				
				if(HoldingReg[HR_DUCT_AIR_FLOW] < HoldingReg[HR_FLOW_REQUIRE]*0.5f)
				{
					if(++tmr_50 > 60)
					{
						tmr_50= 60;
						HoldingReg[HR_SAP_RST_REQ]= 3;
					}
				}
				else
				{
					tmr_50= 0;
				}
			}
			else
			{
				tmr_70= 0;
			}
		}
		else
		{
			if(HoldingReg[HR_DAMPER_POSITION] < 850)
			{
				tmr_70= 0;
				tmr_50= 0;
				HoldingReg[HR_SAP_RST_REQ]= 0;
			}
		}
	}
	else
	{
		tmr_70= 0;
		tmr_50= 0;
		HoldingReg[HR_SAP_RST_REQ]= 0;
	}
}

// Invoked One Second
// HW Temp is too LOW
void VAV_HWT_Reset(void)		
{
	static uint16_t tmr_170= 0, tmr_80= 0;
	
	if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD]!= OVRD_HTG_TO_ON)
	{
		if(HoldingReg[HR_HTG_VLV_OUTPUT] > 950)
		{
			HoldingReg[HR_HWT_RST_REQ]= 1;
		}
		else
		{
			if(HoldingReg[HR_HTG_VLV_OUTPUT] < 850)
			{
				HoldingReg[HR_HWT_RST_REQ]= 0;
			}
		}
			
		if(HoldingReg[HR_HTG_VLV_OUTPUT] == 1000 && HoldingReg[HR_VAV_DAT] + 80 < lc_dat_setpt)
		{
			if(++tmr_80 > 300)
			{
				tmr_80= 300;
				HoldingReg[HR_HWT_RST_REQ]= 2;
			}
			
			if(HoldingReg[HR_HTG_VLV_OUTPUT] == 1000 && HoldingReg[HR_VAV_DAT] + 170 < lc_dat_setpt)
			{
				if(++tmr_170 > 300)
				{
					tmr_170= 300;
					HoldingReg[HR_HWT_RST_REQ]= 3;
				}
			}
			else
			{
				tmr_170= 0;
			}
		}
		else
		{
			tmr_80= 0;
		}
	}
	else
	{
		tmr_170= 0;
		tmr_80= 0;
		HoldingReg[HR_HWT_RST_REQ]= 0;
	}
}

void VAV_Alarm_HighDAT(void)
{
	static uint16_t tmr_hstg= 0;
	static int8_t alm_gen_1= 0;
	int8_t alm_1;
	
	//
	if(HoldingReg[HR_HTG_TYPE]== HTG_STAGE)
	{
		if(HoldingReg[HR_VAV_DAT] > HoldingReg[HR_VAV_DAT_HI])
		{
			if(HoldingReg[HR_VAV_DAT] > HoldingReg[HR_VAV_DAT_HI] + 100)
			{
				if(++tmr_hstg > 5)
				{
					tmr_hstg= 5;
					if(alm_gen_1== 0)
					{
						alm_gen_1= 1;
						Alm_DAT_Hi.tmr_entry_delay= 1;
					}
				}
			}
			else
			{
				if(++tmr_hstg > 10)
				{
					tmr_hstg= 10;
					alm_gen_1= 1;
				}
			}
		}
		else
		{
			tmr_hstg= 0;
			alm_gen_1= 0;
		}
	}
	else
	{
		tmr_hstg= 0;
		alm_gen_1= 0;
	}
	
	alm_1= Alarm_Check(&Alm_DAT_Hi, alm_gen_1);
	
	if(alm_1)
	{
		HoldingReg[HR_UNIT_ALARM]|= 0x0001 << BIT_ALM_HIGH_DAT;
	}
	else
	{
		HoldingReg[HR_UNIT_ALARM]&= ~(0x0001 << BIT_ALM_HIGH_DAT);
	} 
}

// Invoked per second
void VAV_Alarm_LowDAT(void)
{
	static uint16_t tmr_170= 0, tmr_80= 0;
	int8_t alm_gen_1, alm_gen_2;
	int8_t alm_1, alm_2;
	
	alm_gen_1= 0;
	alm_gen_2= 0;
	if(HoldingReg[HR_HTG_TYPE]== HTG_MODULATE 
		&& _in_range_limit(HoldingReg[HR_VAV_DAT], 9990, 1)) 
	{
		if(HoldingReg[HR_VAV_DAT] + 80 < lc_dat_setpt)
		{
			if(++tmr_80 > 600)
			{
				tmr_80 = 600;
				alm_gen_2= 1;
			}
		}
		else
		{
			tmr_80 = 0;
		}
		
		if(HoldingReg[HR_VAV_DAT] + 170 < lc_dat_setpt)
		{
			if(++tmr_170 > 600)
			{
				tmr_170 = 600;
				alm_gen_1= 1;
			}
		}
		else
		{
			tmr_170= 0;
		}
	}
	
	alm_1= Alarm_Check(&Alm_DAT_Lo_17, alm_gen_1);
	alm_2= Alarm_Check(&Alm_DAT_Lo_8, alm_gen_2);
	
	if(alm_1|alm_2)
	{
		HoldingReg[HR_UNIT_ALARM]|= 0x0001 << BIT_ALM_LOW_DAT;
	}
	else
	{
		HoldingReg[HR_UNIT_ALARM]&= ~(0x0001 << BIT_ALM_LOW_DAT);
	}
}

// Invoked per second
void VAV_Alarm_LowSAF(void)
{
	static uint16_t tmr_70= 0, tmr_50= 0;
	int8_t alm_gen_1, alm_gen_2;
	int8_t alm_1, alm_2;
	
	alm_gen_1= 0;
	alm_gen_2= 0;
	
	if(HoldingReg[HR_FLOW_REQUIRE])
	{
		if(HoldingReg[HR_DUCT_AIR_FLOW] < HoldingReg[HR_FLOW_REQUIRE]*0.7f)
		{
			if(++tmr_70 > 300)
			{
				tmr_70= 300;
				alm_gen_2= 1;
			}
		}
		else
		{
			tmr_70= 0;
		}
		
		if(HoldingReg[HR_DUCT_AIR_FLOW] < HoldingReg[HR_FLOW_REQUIRE]*0.5f)
		{
			if(++tmr_50 > 300)
			{
				tmr_50= 300;
				alm_gen_1= 1;
			}
		}
		else
		{
			tmr_50= 0;
		}
	}
	
	alm_1= Alarm_Check(&Alm_SAF_Lo_70, alm_gen_1);
	alm_2= Alarm_Check(&Alm_SAF_Lo_50, alm_gen_2);
	
	if(alm_1|alm_2)
	{
		HoldingReg[HR_UNIT_ALARM]|= 0x0001 << BIT_ALM_LOW_SAF;
	}
	else
	{
		HoldingReg[HR_UNIT_ALARM]&= ~(0x0001 << BIT_ALM_LOW_SAF);
	}
}

// Invoked per second
//void VAV_GetFanFailFault(void)
//{
//	static uint16_t tmr_fan= 0;
//	int8_t alm_gen_1, alm_gen_2;
//	int8_t alm_1, alm_2;
//	
//	alm_gen_1= 0;
//	alm_gen_2= 0;
//	
//	if(HoldingReg[HR_AUX_FAN_EN])
//	{
//		if(HoldingReg[HR_AUX_FAN_CMD]!= HoldingReg[HR_AUX_FAN_ST])
//		{
//			if(++tmr_fan > 15)
//			{
//				tmr_fan= 15;
//				if(HoldingReg[HR_AUX_FAN_CMD])
//				{
//					alm_gen_1= 1;
//				}
//				else
//				{
//					alm_gen_2= 1;
//				}
//			}
//		}
//		else
//		{
//			tmr_fan= 0;
//		}
//	}
//	
//	alm_1= Alarm_Check(&Flt_Fan_Start, alm_gen_1);
//	alm_2= Alarm_Check(&Flt_Fan_Stop, alm_gen_2);
//	
//	if(alm_1|alm_2)
//	{
//		HoldingReg[HR_UNIT_FAULT]|= 0x0001 << BIT_FLT_FAN_FAIL;
//	}
//	else
//	{
//		HoldingReg[HR_UNIT_FAULT]&= ~(0x0001 << BIT_FLT_FAN_FAIL);
//	}
//}

// Invoked per second
void VAV_Fault_FlowSensor(void)
{
	static uint16_t tmr_sensor= 0;
	int8_t alm_gen;
	int8_t alm_1;
	
	alm_gen = 0;
	if(HoldingReg[HR_DAMPER_POSITION] < 10
		&& HoldingReg[HR_AHU_OCC_ST]== OCC_MODE_UNOCC 
		&& HoldingReg[HR_DUCT_AIR_FLOW] > HoldingReg[HR_CLG_FLOW_MAX]*0.1f)
	{
		if(++tmr_sensor > 600)
		{
			tmr_sensor= 600;
			alm_gen= 1;
		}
	}
	else
	{
		tmr_sensor= 0;
	}
	
	alm_1= Alarm_Check(&Flt_Flw_Sensor, alm_gen);
	
	if(alm_1)
	{
		HoldingReg[HR_UNIT_FAULT]|= 0x0001 << BIT_FLT_FLOW_SENSOR;
	}
	else
	{
		HoldingReg[HR_UNIT_FAULT]&= ~(0x0001 << BIT_FLT_FLOW_SENSOR);
	}
}

// Invoked per second
void VAV_Fault_LeakDamper(void)
{
	static uint16_t tmr_dmp= 0;
	int8_t alm_gen;
	int8_t alm_1;
	
	alm_gen= 0;
	if(HoldingReg[HR_DAMPER_POSITION] < 10
		&& HoldingReg[HR_DUCT_AIR_FLOW] > HoldingReg[HR_CLG_FLOW_MAX]*0.1f)	//10%
	{
		if(++tmr_dmp > 600)
		{
			tmr_dmp= 600;
			alm_gen= 1;
		}
	}
	else
	{
		tmr_dmp= 0;
	}
	
	alm_1= Alarm_Check(&Flt_Dmp_Leak, alm_gen);
	
	if(alm_1)
	{
		HoldingReg[HR_UNIT_FAULT]|= 0x0001 << BIT_FLT_LEAK_DMP;
	}
	else
	{
		HoldingReg[HR_UNIT_FAULT]&= ~(0x0001 << BIT_FLT_LEAK_DMP);
	}
}

// Invoked per second
void VAV_Fault_LeakValve(void)
{
	static uint16_t tmr_vlv= 0;
	int8_t alm_gen;
	int8_t alm_1;
	
	alm_gen= 0;
	if(HoldingReg[HR_HTG_TYPE]== HTG_MODULATE )
	{
		if(HoldingReg[HR_HTG_VLV_OUTPUT] == 0
			&& _in_range_limit(HoldingReg[HR_AHU_SAT], 9990, 1)
			&& _in_range_limit(HoldingReg[HR_VAV_DAT], 9990, 1)
			&& HoldingReg[HR_VAV_DAT] > HoldingReg[HR_AHU_SAT] + 30)	//1%
		{
			if(++tmr_vlv > 900)
			{
				tmr_vlv = 900;
				alm_gen= 1;
			}
		}
		else
		{
			tmr_vlv= 0;
			alm_1= 0;
		}
	}
	else
	{
		tmr_vlv= 0;
		alm_1= 0;
	}
	
	alm_1= Alarm_Check(&Flt_Vlv_Leak, alm_gen);
	
	if(alm_1)
	{
		HoldingReg[HR_UNIT_FAULT]|= 0x0001 << BIT_FLT_LEAK_VLV;
	}
	else
	{
		HoldingReg[HR_UNIT_FAULT]&= ~(0x0001 << BIT_FLT_LEAK_VLV);
	}
}

void VAV_Fault_ZoneTemp(void)
{
	if(++tmr_panel_comm > 30)
	{
		HoldingReg[HR_STAT_TEMP]= 9999;
	}
	
	if(HoldingReg[HR_ZONE_TEMP]>= 9990)
	{
		HoldingReg[HR_UNIT_FAULT]|= 0x0001 << BIT_FLT_TEMP_SENSOR;
	}
	else
	{
		HoldingReg[HR_UNIT_FAULT]&= ~(0x0001 << BIT_FLT_TEMP_SENSOR);
	}
}

void VAV_GetFlowSetpoint(void)
{
	// Normal Control
	if(HoldingReg[HR_AHU_OCC_REQ]== OCC_MODE_OCC)
	{
		if(Mode_AHU== MODE_COOL)	//Cooling Air 
		{
			if(PID_Clg.enable)
			{
				lc_flow_req= (uint32_t)HoldingReg[HR_CLG_OUTPUT]*(lc_clg_flow_setpt_max - lc_clg_flow_setpt_min)/1000 
																				+ lc_clg_flow_setpt_min;
			}
			else
			{
				if(PID_Htg.enable)
				{
					if(PID_DAT_Flow.enable)
					{
						lc_flow_req= (uint32_t)lc_pid_dat_flw*(lc_htg_flow_setpt_max - lc_htg_flow_setpt_min)/1000 
																				+ lc_htg_flow_setpt_min;
					}
					else
					{
						lc_flow_req= lc_htg_flow_setpt_min;
					}
				}
				else
				{
					lc_flow_req= lc_clg_flow_setpt_min;
				}
			}
		}
		else	//Warm Air 
		{
			if(HoldingReg[HR_HTG_OUTPUT])
			{
				lc_flow_req= (uint32_t)HoldingReg[HR_HTG_OUTPUT]*(lc_htg_flow_setpt_max - lc_htg_flow_setpt_min)/1000 
																				+ lc_htg_flow_setpt_min;
			}
			else
			{
				lc_flow_req= lc_clg_flow_setpt_min;
			}
		}
		
		lc_flow_req+= lc_flow_co2;
		
		if(lc_flow_req > HoldingReg[HR_CLG_FLOW_MAX])
			lc_flow_req = HoldingReg[HR_CLG_FLOW_MAX];
	}
	else
	{
		lc_flow_req= 0;
		
		if(delay_fan_off)	//Fan off delay, keep minimum flow for 2 minutes 
		{
			lc_flow_req= HoldingReg[HR_FLOW_MIN];
		}
	}
	
	if(HoldingReg[HR_UNOCC_FLW_ZERO]== 0)
	{
		if(lc_flow_req < HoldingReg[HR_FLOW_MIN])
			lc_flow_req= HoldingReg[HR_FLOW_MIN];
	}
	
	//
	if((Mode_AHU== MODE_COOL && Mode_SAT== MODE_HEAT)
		|| (Mode_AHU== MODE_HEAT && Mode_SAT== MODE_COOL))
	{ 
		lc_flow_req = HoldingReg[HR_FLOW_MIN];
	}
	
	if(HoldingReg[HR_ZONE_TEMP] > 9990)
	{
		if(lc_flow_req > HoldingReg[HR_FLOW_MIN])
			lc_flow_req = HoldingReg[HR_FLOW_MIN];
	}
	
	//Commissioning/Testing
	if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD] == OVRD_FLW_SET_TO_ZERO)		// Flow Override
	{
		lc_flow_req= 0; 
	}
	else
	{
		if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD] == OVRD_FLW_SET_TO_VCOOL_MAX)
		{
			lc_flow_req= HoldingReg[HR_CLG_FLOW_MAX];
		}
		else
		{
			if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD] == OVRD_FLW_SET_TO_VCOOL_MIN)
			{
				lc_flow_req= HoldingReg[HR_FLOW_MIN];
			}
			else
			{
				if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD] == OVRD_FORCE_FLOW)
				{
					lc_flow_req= HoldingReg[HR_FORCE_FLW_VALUE];
				}
				else
				{
					if(HoldingReg[HR_EFFECT_HVAC_MODE]== MODE_PURGE)
					{
						lc_flow_req= HoldingReg[HR_CLG_FLOW_MAX];
					}
				}
			}
		}
	}
	
	if(lc_htg_flow_enable)
	{
		if(lc_flow_req < HoldingReg[HR_FLOW_MIN])
		{
			lc_flow_req = HoldingReg[HR_FLOW_MIN];
		}
	}
}

void VAV_ValveControl(void)
{
	if(HoldingReg[HR_HTG_TYPE]== HTG_MODULATE)
	{
		//Commissioning/Testing
		if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD] == OVRD_HTG_TO_OFF)		
		{
			lc_htg_valve= 0; 
		}
		else
		{
			if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD] == OVRD_HTG_TO_ON)
			{
				lc_htg_valve= 1000;
			}
			else
			{
				if(Mode_AHU== MODE_COOL)
				{
					if(PID_DAT_Valve.enable)
					{
						lc_htg_valve= _min(lc_pid_htg_zat, lc_pid_dat_vlv);
					}
					else
					{
						lc_htg_valve= lc_pid_htg_zat;
					}
				}
			}
		}
	}
	else
	{
		lc_htg_valve= 0; 
	}
	
	if(tmr_htg_flow_ready < 30)
		lc_htg_valve= 0;
}	

//Return:lc_htg_stage
void VAV_HtgStageControl(void)
{
	static uint8_t last_htg_stage= 0;
	
	if(HoldingReg[HR_HTG_TYPE]== HTG_STAGE 
		&& Mode_AHU== MODE_COOL
		&& (HoldingReg[HR_UNIT_ALARM] & (0x0001 << BIT_ALM_HIGH_DAT))== 0)
	{
		if(HoldingReg[HR_HTG_OUTPUT] > 400)
		{
			lc_htg_stage= 1;
			lc_htg_flow_enable= 1;
		}
		else
		{
			if(HoldingReg[HR_HTG_OUTPUT] == 0)
			{
				lc_htg_stage= 0;
			}
		}
	}
	else
	{
		lc_htg_stage= 0;
	}
	
	//Minimum On/OFF Minutes
	if(last_htg_stage!= lc_htg_stage)
	{
		if(lc_htg_stage)
		{
			if(tmr_htg_stage_off_mins >= HoldingReg[HR_HTG_STAGE_OFF_MINUTES]*60)
			{
				tmr_htg_stage_on_mins= 0;
			}
			else
			{
				lc_htg_stage= 0;
			}
		}
		else
		{
			if(HoldingReg[HR_UNIT_ALARM] & (0x0001 << BIT_ALM_HIGH_DAT))
			{
				tmr_htg_stage_on_mins = HoldingReg[HR_HTG_STAGE_ON_MINUTES]*60;
			}
			
			if(tmr_htg_stage_on_mins >= HoldingReg[HR_HTG_STAGE_ON_MINUTES]*60
				|| tmr_htg_flow_ready < 30)
			{
				tmr_htg_stage_off_mins= 0;
				lc_htg_flow_enable= 0;
			}
			else
			{
				lc_htg_stage= 1;
			}
		}
		
		last_htg_stage= lc_htg_stage;	
	}
		
	//Minimum On/OFF Minutes
	if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD]== OVRD_HTG_TO_OFF)		// Valve Override
	{
		lc_htg_stage= 0;
		tmr_htg_stage_on_mins= 0;		
	}
	else
	{
		if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD]== OVRD_HTG_TO_ON)
		{
			lc_htg_stage= 1;
			tmr_htg_stage_off_mins= 0;	
		}
	}
	
	if(tmr_htg_flow_ready < 30)
		lc_htg_stage= 0;
}

// return: lc_fan_cmd
void VAV_AuxFanCotrol(void)
{
	static uint8_t last_fan_cmd= 0;
	
	if(HoldingReg[HR_EFFECT_HVAC_MODE]!= MODE_PURGE)
	{
		if(HoldingReg[HR_AUX_FAN_TYPE]== FAN_SERIAL)
		{
			if(HoldingReg[HR_AHU_OCC_REQ]== OCC_MODE_OCC)
			{
				lc_fan_cmd= 1;
			}
			else
			{
				lc_fan_cmd= 0;
			}
		}
		else
		{
			if(HoldingReg[HR_AUX_FAN_TYPE]== FAN_PAALLEL)
			{
				if(HoldingReg[HR_AHU_OCC_REQ]== OCC_MODE_OCC)
				{
					if(Mode_AHU== MODE_COOL)
					{
						if(HoldingReg[HR_ZONE_TEMP] < HoldingReg[HR_HTG_SETPT]
							&& PID_Clg.enable== 0)	
						{
							lc_fan_cmd= 1;
						}
						else
						{
							if(HoldingReg[HR_CLG_OUTPUT] > 50 
								&& PID_Htg.enable== 0)
							{
								lc_fan_cmd= 0;
							}
						}
					}
					else
					{
						if(HoldingReg[HR_ZONE_TEMP] < HoldingReg[HR_HTG_SETPT]
							&& HoldingReg[HR_HTG_OUTPUT] > 50)
						{
							lc_fan_cmd= 1;
						}
						else
						{
							if(HoldingReg[HR_HTG_OUTPUT] == 0
								&& PID_Htg.enable== 0)
							{
								lc_fan_cmd= 0;
							}
						}
					}
				}
				else
				{
					if(PID_Htg.enable== 0)
					{
						lc_fan_cmd= 0;
					}
				}
			}
			else
			{
				lc_fan_cmd= 0;
			}
		}
	}
	else
	{
		lc_fan_cmd= 0;
	}
	
	//Minimum On/OFF Minutes
	if(last_fan_cmd!= lc_fan_cmd)
	{
		if(lc_fan_cmd)
		{
			if(tmr_fan_off_mins>= HoldingReg[HR_AUX_FAN_OFF_MINUTES]*60)
			{
				tmr_fan_off_mins= 0;
				tmr_fan_on_mins= 0;
			}
			else
			{
				lc_fan_cmd= 0;
			}
		}
		else
		{
			if(tmr_fan_on_mins>= HoldingReg[HR_AUX_FAN_ON_MINUTES]*60 && delay_fan_off== 0)
			{
				tmr_fan_off_mins= 0;
				tmr_fan_on_mins= 0;
			}
			else
			{
				lc_fan_cmd= 1;
			}
		}
		
		last_fan_cmd= lc_fan_cmd;
	}
	
	//Commissioning/Testing
	if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD]== OVRD_FAN_TO_OFF)		// Fan Override
	{
		lc_fan_cmd= 0; 
		tmr_fan_on_mins= 0;
	}
	else
	{
		if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD]== OVRD_FAN_TO_ON)
		{
			lc_fan_cmd= 1;
			tmr_fan_off_mins= 0;
		}
	}
}

// Return: lc_act_pos_setpt
void VAV_DamperControl(uint16_t lc_flow_req)
{
	float duct_dp;
	float pito_coeff;
	float dp_offset;
	float duct_flow;
	float duct_flow_setpt;
	float duct_area;
	float temp, hi, lo, rate;
	
	// Get Delta Pressure Raw Value
	dp_offset= VAR_GetRealValue(HR_DUCT_DP_OFFSET); 
	
	duct_dp= Press_GetAvgValue() + dp_offset;
	VAR_SetRealValue(HR_DUCT_DP, duct_dp, 1);
	
	//Get Duct Area
	duct_area= VAR_GetRealValue(HR_DUCT_AREA);
	
	// Get Duct Velo Setpoint
	duct_flow_setpt= lc_flow_req;
	
	//Press->Duct Velo->Duct Flow->Act Setpt
	pito_coeff= VAR_GetRealValue(HR_DUCT_DP_COEFF);
	duct_flow= Press_GetDuctVelocity(duct_dp, pito_coeff);
	duct_flow*= duct_area*3600;
	
	//
	if(lc_flow_req > 0.0f)
	{
		if(Timer_Expires(&tm_PID_Damper))
		{
			rate= fabs(duct_flow - duct_flow_setpt)/ duct_flow_setpt;
			
			if(rate > 0.03f && fabs(duct_flow - duct_flow_setpt) > HoldingReg[HR_FLOW_DB]/2)
			{
				//
				if(rate > 0.1)
				{
					PID_SetBandCoeff(&PID_Dmp_Flow, 
												HoldingReg[HR_FLOW_PB], HoldingReg[HR_FLOW_I], HoldingReg[HR_FLOW_D], 
												PID_DMP_TS);
				}
				else
				{
					PID_SetBandCoeff(&PID_Dmp_Flow, 
												HoldingReg[HR_FLOW_PB]*3, HoldingReg[HR_FLOW_I], HoldingReg[HR_FLOW_D], 
												PID_DMP_TS);
				}
				
				//Set PID Output Limit
				temp= HoldingReg[HR_DAMPER_POSITION];
				temp/= 1000.0f;
				if(rate < 0.2f)
				{
					hi= temp + 0.01f;
					lo= temp - 0.01f;
				}
				else
				{
					hi= temp + 0.03f;
					lo= temp - 0.03f;
				}
				
				if(hi > 1.0f)
				{
					hi= 1.0f;
				}
				
				if(lo < 0.0f)
				{
					lo= 0.0f;
				}
				
				PID_LimitSet(&PID_Dmp_Flow, hi, lo);
				
				lc_act_pos_setpt= (PID_Gain_Loop(&PID_Dmp_Flow, duct_flow, duct_flow_setpt)*1000 + lc_act_pos_setpt)/2.0f; 
			}
		}
	}
	else
	{
		lc_act_pos_setpt= 0.0;
		PID_Init(&PID_Dmp_Flow);
	}
	
	// Get Duct Flow
	HoldingReg[HR_DUCT_AIR_FLOW]= duct_flow;
	
	//Minimum On/OFF Minutes
	if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD] == OVRD_DMP_TO_CLOSE)		// Damper Override
	{
		lc_act_pos_setpt= 0; 
	}
	else
	{
		if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD] == OVRD_DMP_TO_OPEN)
		{
			lc_act_pos_setpt= 1000;
		}
		else
		{
			if((COMMISSION_TSET)HoldingReg[HR_COMMISSION_OVRD] == OVRD_FORCE_DAMPER)
			{
				lc_act_pos_setpt= HoldingReg[HR_FORCE_DMP_VALUE];
			}
		}
	}
}

void VAV_GetEffectSetpoint(void)
{
	int16_t clg_temp_setpt, htg_temp_setpt;
	
	if(HoldingReg[HR_EFFECT_OCC]== OCC_MODE_OCC)
	{
		clg_temp_setpt= HoldingReg[HR_OCC_CLG_SETPT] + HoldingReg[HR_STAT_ADJUST];
		htg_temp_setpt= HoldingReg[HR_OCC_HTG_SETPT] + HoldingReg[HR_STAT_ADJUST];
	}
	else
	{
		if(HoldingReg[HR_SCHEDUULE_OCC]== OCC_MODE_OCC
			&& HoldingReg[HR_STAT_OCC]== OCC_MODE_UNOCC)
		{
			if(Mode_AHU== MODE_COOL)
			{
				clg_temp_setpt= HoldingReg[HR_OCC_CLG_SETPT] + HoldingReg[HR_STAT_ADJUST] + HoldingReg[HR_STDBY_TEMP_OFFSET];
				htg_temp_setpt= HoldingReg[HR_OCC_HTG_SETPT] + HoldingReg[HR_STAT_ADJUST] + HoldingReg[HR_STDBY_TEMP_OFFSET];
			}
			else
			{
				clg_temp_setpt= HoldingReg[HR_OCC_CLG_SETPT] + HoldingReg[HR_STAT_ADJUST] - HoldingReg[HR_STDBY_TEMP_OFFSET];
				htg_temp_setpt= HoldingReg[HR_OCC_HTG_SETPT] + HoldingReg[HR_STAT_ADJUST] - HoldingReg[HR_STDBY_TEMP_OFFSET];
			}
		}
		else
		{
			switch((HVAC_MODE)HoldingReg[HR_EFFECT_HVAC_MODE])
			{
				case MODE_WARMUP:
				case MODE_COOLDOWN:
					clg_temp_setpt= HoldingReg[HR_OCC_CLG_SETPT];
					htg_temp_setpt= HoldingReg[HR_OCC_HTG_SETPT];
					break;
				case MODE_SETUP:			//In Cooling Mode
				case MODE_SETBACK:		//In Heating Mode
					clg_temp_setpt= HoldingReg[HR_UNOCC_CLG_SETPT] - 10;
					htg_temp_setpt= HoldingReg[HR_UNOCC_HTG_SETPT] + 10;
					break;
				default:
					clg_temp_setpt= HoldingReg[HR_UNOCC_CLG_SETPT];
					htg_temp_setpt= HoldingReg[HR_UNOCC_HTG_SETPT];
					break;
			}
		}
	}
	
	HoldingReg[HR_CLG_SETPT]= clg_temp_setpt;
	HoldingReg[HR_HTG_SETPT]= htg_temp_setpt;
}

void VAV_GetSATMode(void)
{
	if(HoldingReg[HR_LOCAL_TEMP_FUNCTION]== TFN_SAT)
	{
		if(HoldingReg[HR_DUCT_AIR_FLOW] > HoldingReg[HR_FLOW_MIN]*0.9f)
		{
			if(++sat_delay > DELAY_SAT)
			{
				sat_delay= DELAY_SAT;
				HoldingReg[HR_AHU_SAT]= lc_local_temp;
			}
			else
			{
				HoldingReg[HR_AHU_SAT]= 0;
			}
		}
		else
		{
			HoldingReg[HR_AHU_SAT]= 0;
			sat_delay= 0;
		}
	}

	if(_in_range_limit(HoldingReg[HR_AHU_SAT], 9990, 1)
		&& _in_range_limit(HoldingReg[HR_ZONE_TEMP], 9990, 1))
	{
		if(HoldingReg[HR_AHU_SAT] > HoldingReg[HR_SAT_HTG_LOCK]
			|| (HoldingReg[HR_ZONE_TEMP] < HoldingReg[HR_HTG_SETPT] 
					&& HoldingReg[HR_HTG_SETPT] + 30 < HoldingReg[HR_AHU_SAT]))	//Accoding to Addenum Z
		{
			Mode_SAT = MODE_HEAT;
		}
		else
		{
			if(HoldingReg[HR_AHU_SAT] < HoldingReg[HR_SAT_CLG_LOCK] 
					|| (HoldingReg[HR_ZONE_TEMP] > HoldingReg[HR_CLG_SETPT]
							&&HoldingReg[HR_CLG_SETPT] > HoldingReg[HR_AHU_SAT] + 30))	//Accoding to Addenum Z
			{
				Mode_SAT= MODE_COOL;
			}
		}
	}
	else
	{
		Mode_SAT= MODE_NONE;
	}
	
	
}

void VAV_GetFlowSetpointRange(void)
{
	if(HoldingReg[HR_HTG_FLOW_MAX]== 0)
	{
		HoldingReg[HR_HTG_FLOW_MAX]= HoldingReg[HR_CLG_FLOW_MAX];
	}
	
	switch((HVAC_MODE)HoldingReg[HR_EFFECT_HVAC_MODE])
	{
		case MODE_COOLDOWN:		//In Cooling Mode
			lc_clg_flow_setpt_max= HoldingReg[HR_CLG_FLOW_MAX];
			lc_clg_flow_setpt_min= 0;
			lc_htg_flow_setpt_max= HoldingReg[HR_HTG_FLOW_MAX];
			lc_htg_flow_setpt_min= HoldingReg[HR_FLOW_MIN];
			break;	
		case MODE_WARMUP:			//In Heating Mode
		case MODE_SETBACK:		//In Heating Mode
			lc_clg_flow_setpt_max= 0;
			lc_clg_flow_setpt_min= 0;
			lc_htg_flow_setpt_max= HoldingReg[HR_CLG_FLOW_MAX];
			lc_htg_flow_setpt_min= HoldingReg[HR_HTG_FLOW_MAX];
			break;
		case MODE_SETUP:	//In Cooling Mode
			lc_clg_flow_setpt_max= HoldingReg[HR_CLG_FLOW_MAX];
			lc_clg_flow_setpt_min= 0;
			lc_htg_flow_setpt_max= 0;
			lc_htg_flow_setpt_min= 0;
			break;
		case MODE_PURGE:
			lc_clg_flow_setpt_max= HoldingReg[HR_CLG_FLOW_MAX];
			lc_clg_flow_setpt_min= 0;
			lc_htg_flow_setpt_max= 0;
			lc_htg_flow_setpt_min= 0;
			break;
		default:
			lc_clg_flow_setpt_max= HoldingReg[HR_CLG_FLOW_MAX];
			lc_clg_flow_setpt_min= HoldingReg[HR_FLOW_MIN];
			lc_htg_flow_setpt_max= HoldingReg[HR_HTG_FLOW_MAX];
			lc_htg_flow_setpt_min= HoldingReg[HR_FLOW_MIN];
			break;
	}
}

//Invoked every second
void VAV_GetEffectOcc(void)
{
	if(HoldingReg[HR_STAT_OCC]!= OCC_MODE_NONE)
	{
		HoldingReg[HR_EFFECT_OCC]= HoldingReg[HR_STAT_OCC];
	}
	else
	{
		if(HoldingReg[HR_SCHEDUULE_OCC]== OCC_MODE_OCC)
		{
			HoldingReg[HR_EFFECT_OCC]= OCC_MODE_OCC;
		}
		else
		{
			HoldingReg[HR_EFFECT_OCC]= OCC_MODE_UNOCC;
		}
	}
	
	//
	if(HoldingReg[HR_STAT_OCC]== OCC_MODE_UNOCC)
	{
		lc_ext_sec= 0;
		
		if(HoldingReg[HR_SCHEDUULE_OCC]== OCC_MODE_UNOCC)
		{
			HoldingReg[HR_STAT_OCC]= OCC_MODE_NONE;
		}
	}
	
	if(lc_ext_sec)
	{
		HoldingReg[HR_STAT_OCC]= OCC_MODE_OCC;
		
		if(--lc_ext_sec== 0)
		{
			HoldingReg[HR_STAT_OCC]= OCC_MODE_NONE;
		}
	}
	
	HoldingReg[HR_STAT_EXT_MIN]= (lc_ext_sec + 59)/60;
	
	// AHU Occ Req
	if(HoldingReg[HR_EFFECT_OCC]== OCC_MODE_OCC
		|| HoldingReg[HR_SCHEDUULE_OCC]== OCC_MODE_OCC)
	{
		HoldingReg[HR_AHU_OCC_REQ]= OCC_MODE_OCC;
	}
	else
	{
		if(HoldingReg[HR_EFFECT_HVAC_MODE]== MODE_COOL
			|| HoldingReg[HR_EFFECT_HVAC_MODE]== MODE_HEAT)
		{
			HoldingReg[HR_AHU_OCC_REQ]= OCC_MODE_UNOCC;
		}
		else
		{
			HoldingReg[HR_AHU_OCC_REQ]= OCC_MODE_OCC;
		}
	}
}

//Invoked every second
void VAV_GetEffectHVACMode(void)
{
	VAV_GetSATMode();	
	
	if((HVAC_MODE)HoldingReg[HR_HVAC_MODE_OVRD]== MODE_NONE)
	{
		if(Mode_SAT!= MODE_NONE)
		{
			HoldingReg[HR_EFFECT_HVAC_MODE]= Mode_SAT;
		}
		else
		{
			HoldingReg[HR_EFFECT_HVAC_MODE]= HoldingReg[HR_STAT_MODE_SET];
		}
	}
	else
	{
		HoldingReg[HR_EFFECT_HVAC_MODE]= HoldingReg[HR_HVAC_MODE_OVRD];
	}
		
	switch((HVAC_MODE)HoldingReg[HR_EFFECT_HVAC_MODE])
	{
		case MODE_HEAT:
		case MODE_SETBACK:
		case MODE_WARMUP:
			Mode_AHU= MODE_HEAT;
			break;
		case MODE_COOL:
		case MODE_SETUP:
		case MODE_COOLDOWN:
			Mode_AHU= MODE_COOL;
			break;
		case MODE_PURGE:
			Mode_AHU= MODE_COOL;
			break;
		default:
			Mode_AHU= MODE_COOL;
			break;
	}
}

void VAV_PID_Control(void)
{
	//PID Enable 
	if(HoldingReg[HR_EFFECT_HVAC_MODE]!= MODE_PURGE
		&& _in_range_limit(HoldingReg[HR_ZONE_TEMP], 9990, 1)
		&& HoldingReg[HR_AHU_OCC_REQ]== OCC_MODE_OCC)
	{
		// Cooling PID Enable
		if(HoldingReg[HR_ZONE_TEMP] > HoldingReg[HR_CLG_SETPT])
		{
			if(PID_Htg.enable== 0
				&& Mode_AHU== MODE_COOL)
			{
				PID_Clg.enable= 1;
				PID_Clg.delay_disable= 0;
			}
			else
			{
				PID_Clg.enable= 0;
			}
		}
		else
		{
			if(HoldingReg[HR_CLG_OUTPUT]== 0 && PID_Clg.enable)
			{
				if(++PID_Clg.delay_disable > DELAY_PID_OFF)
				{
					PID_Clg.delay_disable= DELAY_PID_OFF;
					PID_Clg.enable= 0;
				}
			}
		}
		
		// Heating PID Enable		
		if(HoldingReg[HR_ZONE_TEMP] < HoldingReg[HR_HTG_SETPT])
		{
			if(PID_Clg.enable== 0 
				&& (Mode_AHU== MODE_HEAT || HoldingReg[HR_HTG_TYPE]))
			{
				PID_Htg.enable= 1;
				PID_Htg.delay_disable= 0;
			}
			else
			{
				PID_Htg.enable= 0;
			}
		}
		else
		{
			if(HoldingReg[HR_HTG_OUTPUT]== 0  && PID_Htg.enable)
			{
				if(++PID_Htg.delay_disable > DELAY_PID_OFF)
				{
					PID_Htg.delay_disable= DELAY_PID_OFF;
					PID_Htg.enable= 0;
				}
			}
		}
		
		// DAT Valve Control Enable
		if(PID_Htg.enable
			&& HoldingReg[HR_HTG_TYPE]== HTG_MODULATE
			&& HoldingReg[HR_EFFECT_OCC]== OCC_MODE_OCC						//If No body in room, DAT Control is disabled.
			&& _in_range_limit(HoldingReg[HR_VAV_DAT], 9990, 1))
		{
			if(HoldingReg[HR_HTG_OUTPUT] <= 500)
			{
				lc_dat_setpt= HoldingReg[HR_HTG_SETPT] + 110;
				PID_DAT_Valve.enable= 1;
			}
			else
			{
				if(HoldingReg[HR_HTG_OUTPUT] >= 510)
				{
					PID_DAT_Valve.enable= 0;
				}
			}
		}
		else
		{
			PID_DAT_Valve.enable= 0;
			PID_DAT_Valve.delay_disable= 0;
		}
		
		// DAT Flow Control Enable
		if(PID_Htg.enable
			&& HoldingReg[HR_HTG_TYPE]
			&& HoldingReg[HR_EFFECT_OCC]== OCC_MODE_OCC						//If No body in room, DAT Control is disabled.
			&& _in_range_limit(HoldingReg[HR_VAV_DAT], 9990, 1))
		{
			if(HoldingReg[HR_HTG_OUTPUT] <= 500)
			{
				PID_DAT_Flow.enable= 0;
			}
			else
			{
				if(HoldingReg[HR_HTG_OUTPUT] >= 510)
				{
					lc_dat_setpt= HoldingReg[HR_ZONE_TEMP] + 30;
					
					PID_DAT_Flow.enable= 1;
				}
			}
		}
		else
		{
			PID_DAT_Flow.enable= 0;
			PID_DAT_Flow.delay_disable= 0;
		}
	}
	else
	{
		PID_Clg.enable= 0;
		PID_Clg.delay_disable= 0;
		
		PID_Htg.enable= 0;
		PID_Htg.delay_disable= 0;
		
		PID_DAT_Flow.enable= 0;
		PID_DAT_Flow.delay_disable= 0;
		
		PID_DAT_Valve.enable= 0;
		PID_DAT_Valve.delay_disable= 0;
	}
	/////////////////////////////////////////////////////////////////////
	// Cooling PID
	if(PID_Clg.enable)
	{
		PID_SetBandCoeff(&PID_Clg, 
										HoldingReg[HR_CLG_PB], HoldingReg[HR_CLG_I], 0, 
										PID_TEMP_TS);
		
		if(abs(HoldingReg[HR_ZONE_TEMP] - HoldingReg[HR_CLG_SETPT]) > HoldingReg[HR_CLG_DB]/2)
		{
			lc_pid_clg_zat= PID_Band_Loop(&PID_Clg, HoldingReg[HR_ZONE_TEMP], HoldingReg[HR_CLG_SETPT], 0)*1000;
		}
	}
	else
	{
		lc_pid_clg_zat= 0;
		PID_Init(&PID_Clg);
	}
	
	// Heating PID
	if(PID_Htg.enable) 
	{
		PID_SetBandCoeff(&PID_Htg, 
											HoldingReg[HR_HTG_PB], HoldingReg[HR_HTG_I], 0, 
											PID_TEMP_TS);	
		
		if(abs(HoldingReg[HR_ZONE_TEMP] - HoldingReg[HR_HTG_SETPT]) > HoldingReg[HR_HTG_DB]/2)
		{
			lc_pid_htg_zat= PID_Band_Loop(&PID_Htg, HoldingReg[HR_ZONE_TEMP], HoldingReg[HR_HTG_SETPT], 0)*1000;
		}
	}
	else
	{
		lc_pid_htg_zat= 0;
		PID_Init(&PID_Htg);
	}
	
	// DAT Valve PID
	if(PID_DAT_Valve.enable)
	{
		PID_SetBandCoeff(&PID_DAT_Valve, 
											HoldingReg[HR_DAT_PB], HoldingReg[HR_DAT_I], 0, 
											PID_TEMP_TS);
		
		if(abs(HoldingReg[HR_VAV_DAT] - lc_dat_setpt) > HoldingReg[HR_DAT_DB]/2)
		{
			lc_pid_dat_vlv= PID_Band_Loop(&PID_DAT_Valve, HoldingReg[HR_VAV_DAT], lc_dat_setpt, 0)*1000;
		}
	}
	else
	{
		lc_pid_dat_vlv= 0;
		PID_Init(&PID_DAT_Valve);
	}
	
	// 
	if(PID_DAT_Flow.enable)
	{
		PID_SetBandCoeff(&PID_DAT_Flow, 
											HoldingReg[HR_DAT_PB], HoldingReg[HR_DAT_I], 0, 
											PID_TEMP_TS);
		
		if(abs(HoldingReg[HR_VAV_DAT] - lc_dat_setpt) > HoldingReg[HR_DAT_DB]/2)
		{
			lc_pid_dat_flw= PID_Band_Loop(&PID_DAT_Flow, HoldingReg[HR_VAV_DAT], lc_dat_setpt, 0)*1000;
		}
	}
	else
	{
		lc_pid_dat_flw= 0;
		PID_Init(&PID_DAT_Flow);
	}
}

void VAV_ControlProc(void)
{
	uint16_t ao_output;
	float temp;
	
	//Get Local Temp
	temp= VAV_GetLocalTempInput();
	if(temp== 9999)
	{
		lc_local_temp= 9999;
	}
	else
	{
		if(lc_local_temp < 9999)
		{
			lc_local_temp= (temp + lc_local_temp*9.0f)/10.0f;
			lc_local_temp+= VAR_GetRealValue(HR_LOCAL_TEMP_OFFSET);
		}
		else
		{
			lc_local_temp= temp;
		}
	}

	//Get Zone Temp
	if(HoldingReg[HR_LOCAL_TEMP_FUNCTION]== TFN_ZONE)
	{
		HoldingReg[HR_LOCAL_ZONE_TEMP]= lc_local_temp;
		HoldingReg[HR_ZONE_TEMP]= HoldingReg[HR_LOCAL_ZONE_TEMP];
	}
	else
	{
		HoldingReg[HR_LOCAL_ZONE_TEMP]= 0;
		HoldingReg[HR_ZONE_TEMP]= HoldingReg[HR_STAT_TEMP];
	}
	
	//Get Discharge Air Temp
	if(HoldingReg[HR_LOCAL_TEMP_FUNCTION]== TFN_DAT)
	{
		HoldingReg[HR_VAV_DAT]= lc_local_temp;
	}

	// One Second Proc
	if(Timer_Expires(&tm_OneSecond))
	{
		//Get Effect Occ
		VAV_GetEffectOcc();
		
		//Get Effect HVAC Mode
		VAV_GetEffectHVACMode();
		
		//Get Effect Temperature Setpoint
		VAV_GetEffectSetpoint();
	
		//Get Flow Min Max
		VAV_GetFlowSetpointRange();
		
		//
		VAV_PID_Control();
		
		HoldingReg[HR_CLG_OUTPUT]= lc_pid_clg_zat;
		HoldingReg[HR_HTG_OUTPUT]= lc_pid_htg_zat;
		
		////////////////////////////////
		VAV_Alarm_LowSAF();
		VAV_Alarm_LowDAT();
		VAV_Alarm_HighDAT();
		
		VAV_Fault_FlowSensor();
		VAV_Fault_LeakDamper();
		VAV_Fault_LeakValve();
		VAV_Fault_ZoneTemp();
		
		VAV_SAT_Reset();
		VAV_SAP_Reset();
		VAV_HWT_Reset();
		
		// Heating Stage Start/Stop Minutes
		if(HoldingReg[HR_HTG_STAGE_CMD])
		{
			if(++tmr_htg_stage_on_mins >= HoldingReg[HR_HTG_STAGE_ON_MINUTES]*60)
			{
				tmr_htg_stage_on_mins= HoldingReg[HR_HTG_STAGE_ON_MINUTES]*60;
			}
		}
		else
		{
			if(++tmr_htg_stage_off_mins >= HoldingReg[HR_HTG_STAGE_OFF_MINUTES]*60)
			{
				tmr_htg_stage_off_mins= HoldingReg[HR_HTG_STAGE_OFF_MINUTES]*60;
			}
		}
		
		// Fan Start/Stop Minutes
		if(HoldingReg[HR_AUX_FAN_CMD])
		{
			if(++tmr_fan_on_mins >= HoldingReg[HR_AUX_FAN_ON_MINUTES]*60)
			{
				tmr_fan_on_mins= HoldingReg[HR_AUX_FAN_ON_MINUTES]*60;
			}
		}
		else
		{
			if(++tmr_fan_off_mins >= HoldingReg[HR_AUX_FAN_OFF_MINUTES]*60)
			{
				tmr_fan_off_mins= HoldingReg[HR_AUX_FAN_OFF_MINUTES]*60;
			}
		}
		
		//When heater switched off, Fan Delay Off
		if(HoldingReg[HR_HTG_STAGE_CMD]== 0)
		{
			if(delay_fan_off)
			{
				delay_fan_off--;
			}
		}
		else
		{
			delay_fan_off= DELAY_FAN_OFF;	//120s
		}
		
		//Heat Flow
		if(HoldingReg[HR_DUCT_AIR_FLOW] > HoldingReg[HR_RHTG_FLOW_MIN])
		{
			if(++tmr_htg_flow_ready > 30)
			{
				tmr_htg_flow_ready= 30;
			}
		}
		else
		{
			tmr_htg_flow_ready= 0;
		}
	}
	
	//IAQ Flow
	if(HoldingReg[HR_ZONE_IAQ_SETPT] && HoldingReg[HR_ZONE_IAQ] + 200 > HoldingReg[HR_ZONE_IAQ_SETPT] 
		&& HoldingReg[HR_EFFECT_OCC]== OCC_MODE_OCC)
	{
		temp= HoldingReg[HR_ZONE_IAQ] + 200 - HoldingReg[HR_ZONE_IAQ_SETPT];
		temp/= 200;
	}
	else
	{
		temp= 0;
	}
	temp= _hi_lo_limit(temp, 1.0f, 0.0f);
	lc_flow_co2= temp*HoldingReg[HR_CLG_FLOW_MAX];
	HoldingReg[HR_IAQ_OUTPUT]= temp*1000;
	
	//Get Flow Setpt
	VAV_GetFlowSetpoint();
	HoldingReg[HR_FLOW_REQUIRE]= lc_flow_req;
	
	//Damper Control
	HoldingReg[HR_DAMPER_POSITION]= ACT_GetPosition(); // Get Actator Position
	VAV_DamperControl(HoldingReg[HR_FLOW_REQUIRE]);
	
	ACT_SetPostion(lc_act_pos_setpt);
	
	//Fan Control 
	VAV_AuxFanCotrol();
	HoldingReg[HR_AUX_FAN_CMD]= lc_fan_cmd;
	HAL_Relay_SetValue(0,HoldingReg[HR_AUX_FAN_CMD]);
	
	//Heat Stage Control
	VAV_HtgStageControl();
	if(lc_htg_stage)
	{
		if(tmr_htg_flow_ready >= 30)
		{
			HoldingReg[HR_HTG_STAGE_CMD]= 1;
		}
		else
		{
			HoldingReg[HR_HTG_STAGE_CMD]= 0;
		}
	}
	else
	{
		HoldingReg[HR_HTG_STAGE_CMD]= 0;
	}
	
	HAL_Relay_SetValue(1,HoldingReg[HR_HTG_STAGE_CMD]);
	
	//Valeve Cotrol
	VAV_ValveControl();
	
	HoldingReg[HR_HTG_VLV_OUTPUT]= lc_htg_valve;
	
	if(HoldingReg[HR_HTG_TYPE]== HTG_MODULATE) 
	{
		ao_output= lc_htg_valve;
	}
	else
	{
		//Damper Position
		ao_output= HoldingReg[HR_DAMPER_POSITION];
	}
	
	if(HoldingReg[HR_AO_MAN_VALUE])
	{
		ao_output= HoldingReg[HR_AO_MAN_VALUE];
	}
	
	if(HoldingReg[HR_HTG_VAVE_TYPE]== AT_2_10V)
	{
		ao_output= ao_output*0.8f + 200.0f;
	}
	 
	ao_output= (uint32_t)ao_output*adc_power_compens; 
	HAL_PWM_TIM1_OC1_SetValue(ao_output);
	
	//Debug
	HoldingReg[DBG_SAT_STB_DELAY]= sat_delay;
	HoldingReg[DBG_TMR_CLG_DIS_DLY]= PID_Clg.delay_disable;
	HoldingReg[DBG_TMR_HTG_DIS_DLY]= PID_Htg.delay_disable;
	HoldingReg[DBG_TMR_FAN_OFF_DLY]= delay_fan_off;
	HoldingReg[DBG_TMR_FAN_ON_MINS]= tmr_fan_on_mins;
	HoldingReg[DBG_TMR_FAN_OFF_MINS]= tmr_fan_off_mins;
	HoldingReg[DBG_TMR_HTG_STG_ON_MINS]= tmr_htg_stage_on_mins;
	HoldingReg[DBG_TMR_HTG_STG_OFF_MINS]= tmr_htg_stage_off_mins;
	HoldingReg[DBG_TMR_HTG_FLOW_READY]= tmr_htg_flow_ready;
}

void VAV_Init(void)
{
	DRV_SDP800_Init(0x25);
	DRV_SDP800_SoftReset();
	DRV_SDP800_StartMeasurement(SDP800_TEMPCOMP_DIFFERNTIAL_PRESSURE,
                                   SDP800_AVERAGING_TILL_READ);	

	ACT_Init();	 
	//////////////
	PID_Init(&PID_Clg);
	PID_Init(&PID_Htg);
	PID_Init(&PID_Dmp_Flow);
	PID_Init(&PID_DAT_Valve);
	PID_Init(&PID_DAT_Flow);
	
	PID_SetDirect(&PID_Clg, 1);
	PID_SetDirect(&PID_Htg, 0);
	PID_SetDirect(&PID_Dmp_Flow, 0);
	PID_SetDirect(&PID_DAT_Valve, 0);
	PID_SetDirect(&PID_DAT_Flow, 1);
	
	Timer_Init(&tm_PID_Damper, 1, 1000*PID_DMP_TS);
	Timer_Start(&tm_PID_Damper);
	
	Timer_Init(&tm_OneSecond, 1, 1000);
	Timer_Start(&tm_OneSecond);
	
	//
	Alarm_Init(&Alm_SAF_Lo_70, LVL_URGENT, 300, 5);
	Alarm_Init(&Alm_SAF_Lo_50, LVL_URGENT, 300, 5);
//	Alarm_Init(&Alm_SAF_Lo_50, LVL_CRITICAL_EQUIPMENT, 300, 5);
	Alarm_Init(&Alm_DAT_Lo_17, LVL_URGENT, 600, 5);
	Alarm_Init(&Alm_DAT_Lo_8, LVL_URGENT, 600, 5);
//	Alarm_Init(&Alm_DAT_Lo_8, LVL_CRITICAL_EQUIPMENT, 600, 5);
	Alarm_Init(&Alm_DAT_Hi, LVL_URGENT, 30, 5);
//	Alarm_Init(&Alm_DAT_Hi, LVL_CRITICAL_EQUIPMENT, 30, 5);
	
//	Alarm_Init(&Flt_Fan_Start, LVL_CRITICAL_EQUIPMENT, 15, 5);
//	Alarm_Init(&Flt_Fan_Stop, LVL_NORMAL, 15, 5);
	Alarm_Init(&Flt_Flw_Sensor, LVL_URGENT, 10, 5);
	Alarm_Init(&Flt_Dmp_Leak, LVL_NORMAL, 600, 5);
	Alarm_Init(&Flt_Vlv_Leak,LVL_NORMAL,  900, 5);
	
	tmr_htg_stage_off_mins= HoldingReg[HR_HTG_STAGE_OFF_MINUTES]*60;
	tmr_fan_off_mins= HoldingReg[HR_AUX_FAN_OFF_MINUTES]*60;
}
