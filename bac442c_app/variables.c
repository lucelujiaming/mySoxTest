#include <stdio.h>
#include <stdint.h>
#include "variables.h"
#include "modbus.h" 
#include "mb_var_def.h"
#include "common.h"
#include "eeprom_offchip.h"
#include "thermistor.h"
#include "bacenum.h"

#include "variables.h"

const MB_VAR_SET VarValueRangeTable[]=
{
	{HR_UNIT_FAULT, 0, 31, 0, 0.0f, PR_WR},
	{HR_UNIT_ALARM, 0, 32767,  0, 0.0f, PR_WR},
	{HR_COMMISSION_OVRD, OVRD_NONE, OVRD_FORCE_FLOW, OVRD_NONE, 0.0f, PR_SV},
	{HR_SAT_RST_REQ, 0, 3, 0, 0.0f, PR_RD},
	{HR_SAP_RST_REQ, 0, 3, 0, 0.0f, PR_RD},
	
	{HR_HWT_RST_REQ, 0, 3, 0, 0.0f, PR_RD},
	{HR_EFFECT_OCC, OCC_MODE_UNOCC, OCC_MODE_OCC, OCC_MODE_UNOCC, 0.0f, PR_RD},
	{HR_EFFECT_HVAC_MODE, MODE_NONE, MODE_COOL, MODE_COOL, 0.0f, PR_RD}, 
	{HR_CLG_SETPT, 0, 350, 0, 0.1f, PR_RD},
	{HR_HTG_SETPT, 0, 350, 0, 0.1f, PR_RD},
	//11
	{HR_AHU_OCC_REQ, OCC_MODE_UNOCC, OCC_MODE_OCC, OCC_MODE_UNOCC, 0.0f, PR_RD},
	{HR_AHU_OCC_ST, OCC_MODE_UNOCC, OCC_MODE_OCC, OCC_MODE_NONE, 0.0f, PR_WR},
	{HR_SCHEDUULE_OCC, OCC_MODE_UNOCC, OCC_MODE_OCC, OCC_MODE_NONE, 0.0f, PR_WR}, 
	{HR_HVAC_MODE_OVRD, MODE_NONE, MODE_PURGE, MODE_NONE, 0.0f, PR_WR},	
	{HR_LOCAL_ZONE_TEMP, 0, 500, 0, 0.1f, PR_RD},
	
	{HR_VAV_DAT, 0, 1000, 0, 0.1f, PR_RD},
	{HR_DUCT_DP, 0, 10000, 0, 0.1f, PR_RD},
	{HR_AHU_SAT, 0, 500, 0, 0.1f, PR_WR},
	{HR_DAMPER_POSITION, 0, 1000, 0, 0.1f, PR_RD},
	{HR_HTG_VLV_OUTPUT, 0, 1000, 0, 0.1f, PR_RD},
	//21
	{HR_HTG_STAGE_CMD, 0, 1, 0, 0.0f, PR_RD},
	{HR_AUX_FAN_CMD, 0, 1, 0, 0.0f, PR_RD},
	{HR_STAT_OCC, OCC_MODE_NONE, OCC_MODE_OCC, OCC_MODE_NONE, 0.0f, PR_WR},
	{HR_STAT_ADJUST, -100, 100, 0, 0.1, PR_SV},
	{HR_STAT_TEMP, 0, 500, 220, 0.1f, PR_WR},

	{HR_STAT_ADJUST_MAX, 0, 100, 30, 0.1f, PR_SV},
	{HR_STAT_MODE_SET, MODE_HEAT, MODE_COOL, MODE_COOL, 0.0f, PR_SV},
	{HR_STAT_TEMP_OFFSET, -100, 100, 0, 0.1f, PR_SV},
	{HR_STAT_IAQ_OFFSET, -200, 200, 0, 0.0f, PR_SV},
	{HR_STAT_KEYPAD_LOCK, 0, 7, 0, 0.0f, PR_SV},
	//31
	{HR_STAT_EXT_MIN, 0, 90, 0, 0.0f, PR_WR}, 
	{HR_ZONE_TEMP, 0, 500, 0, 0.1f, PR_RD},
	{HR_ZONE_IAQ, 0, 2000, 0, 0.0f, PR_WR}, 
	{HR_CLG_OUTPUT, 0, 1000, 0, 0.1f, PR_RD},
	{HR_HTG_OUTPUT, 0, 1000, 0, 0.1f, PR_RD},

	{HR_IAQ_OUTPUT, 0, 1000, 0, 0.1, PR_RD},
	{HR_FLOW_REQUIRE, 0, 32767, 0, 0.0f, PR_RD},
	{HR_DUCT_AIR_FLOW, 0, 32767, 0, 0.0f, PR_RD},
	{HR_EXT_MIN_SET, 0, 120, 30, 0.0f, PR_SV},
	{HR_OCC_CLG_SETPT, 180, 300, 240, 0.1f, PR_SV},
	//41
	{HR_OCC_HTG_SETPT, 160, 280, 210, 0.1f, PR_SV},
	{HR_UNOCC_CLG_SETPT, 180, 350, 320, 0.1f, PR_SV},
	{HR_UNOCC_HTG_SETPT, 100, 350, 180, 0.1f, PR_SV},
	{HR_LOCAL_TEMP_TYPE, NTC_10K_B3950, NTC_10K_III, NTC_10K_II, 0.0f, PR_SV},
	{HR_LOCAL_TEMP_OFFSET, -100, 100, 0, 0.1f, PR_SV},	

	{HR_LOCAL_TEMP_FUNCTION, TFN_NONE, TFN_DAT, TFN_NONE, 0.0f, PR_SV},
	{HR_VAV_DAT_HI, 300, 800, 500, 0.1f, PR_SV},
	{HR_HTG_TYPE, HTG_NONE, HTG_MODULATE, HTG_NONE, 0.0f, PR_SV},
	{HR_HTG_STAGE_ON_MINUTES, 0, 20, 3, 0.0f, PR_SV},
	{HR_HTG_STAGE_OFF_MINUTES, 0, 20, 5, 0.0f, PR_SV},
	//51
	{HR_HTG_VAVE_TYPE, AT_NONE, AT_2_10V, AT_0_10V, 0.0f, PR_SV},
	{HR_AUX_FAN_TYPE, FAN_NONE, FAN_PAALLEL, FAN_NONE, 0.0f, PR_SV},
	{HR_AUX_FAN_ON_MINUTES, 0, 20, 3, 0.0f, PR_SV},
	{HR_AUX_FAN_OFF_MINUTES, 0, 20, 5, 0.0f, PR_SV},
	{HR_CLG_FLOW_MAX, 0, 30000,	1440, 0.0f, PR_SV},

	{HR_FLOW_MIN, 0, 30000, 432, 0.0f, PR_SV},
	{HR_HTG_FLOW_MAX, 0, 30000, 1152, 0.0f, PR_SV},
	{HR_RHTG_FLOW_MIN, 0, 30000, 216, 0.0f, PR_SV},
	{HR_ZONE_IAQ_SETPT, 0, 2000,	894, 0.0f, PR_SV},
	{HR_SAT_CLG_LOCK, 180, 280, 220, 0.1f, PR_SV},
	//61
	{HR_SAT_HTG_LOCK, 240, 340, 260, 0.1f, PR_SV},
	{HR_CLG_DB, 0, 20, 4, 0.1f, PR_SV},
	{HR_CLG_PB, 0, 200, 30, 0.1f, PR_SV},
	{HR_CLG_I, 0, 600, 120, 0.0f, PR_SV},

	{HR_HTG_DB, 0, 20, 4, 0.1f, PR_SV},
	{HR_HTG_PB, 0, 200, 30, 0.1f, PR_SV},
	{HR_HTG_I, 0, 600, 120, 0.0f, PR_SV},

	{HR_DAT_DB, 0, 50, 10, 0.1f, PR_SV},
	{HR_DAT_PB, 0, 200, 100, 0.1f, PR_SV},
	{HR_DAT_I, 0, 600, 30, 0.0f, PR_SV},
	
	//71
	{HR_FLOW_DB, 0, 10000, 10, 0.0f, PR_SV},
	{HR_FLOW_PB, 0, 10000, 300, 0.0f, PR_SV},
	{HR_FLOW_I, 0, 1000, 20, 0.0f, PR_SV},
	{HR_FLOW_D, 0, 1000, 3, 0.0f, PR_SV},
	
	{HR_FORCE_DMP_VALUE, 0, 1000, 0, 0.1f, PR_SV},
	{HR_FORCE_FLW_VALUE, 0, 30000, 0, 0.0f, PR_SV}, 
	{HR_UNOCC_FLW_ZERO, 0, 1, 0, 0.0f, PR_SV},
	{HR_AO_MAN_VALUE, 0, 1000, 0, 0.1f, PR_SV},
	
#if HW_VERSION < 23 
	{HR_ACT_10_NM, 0, 1, 0, 0.0f, PR_SV},
#endif

	{HR_STDBY_TEMP_OFFSET, 10, 50, 20, 0.1f, PR_SV},
	{HR_STAT_OCC, OCC_MODE_NONE, OCC_MODE_OCC, OCC_MODE_NONE, 0.0f, PR_WR},
	
	{DBG_ADPTION,  0, 32767, 0, 0.0f, PR_RD}, 
	
	{HR_ACT_ADAPT, 0, 1, 0, 0.0f, PR_WR},
	{HR_UNIT_RESET, 0, 1, 0,	0.0f, PR_WR},
	
	//Unit
	{HR_UNIT_MAC, 1, 247, 127, 0.0f, PR_SV},
	{HR_USART_BAUD, 1, 96, 16, 0.0f, PR_SV},
	{HR_USART_PARITY, 0, 2, 0, 0.0f, PR_SV},
	{HR_USART_TYPE, 0, 2, 0,	0.0f, PR_SV},
	{HR_DUCT_DP_COEFF, 500, 32767, 1000, 0.001f, PR_SV},
	{HR_DUCT_DP_OFFSET, -5000, 5000, 0, 0.1f, PR_SV},
	{HR_DUCT_AREA, 0, 10000, 400,	0.0001f, PR_SV},
	
#if	HW_VERSION > 23
	{HR_ACT_CONFIG, -32768, 32767, 0,	0.0f, PR_SV},
#endif
	
	//BACnet
	{HR_INSTANCE_L, -32768, 32767, 1001,	0.0f, PR_SV},
	{HR_INSTANCE_H, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_MAX_INFO_FRAME, 1, 20, 5,	0.0f, PR_SV},
	{HR_MAX_MASTER, 1, 127, 127,	0.0f, PR_SV},
	{HR_LOCATION, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+1, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+2, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+3, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+4, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+5, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+6, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+7, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+8, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+9, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+10, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+11, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+12, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+13, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+14, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_LOCATION+15, -32768, 32767, 0,	0.0f, PR_SV},
	
	//Update
	{HR_UPD_STATUS, 0, 3, 0,	0.0f, PR_WR},
	{HR_UPD_CMD, 0, 5, 0, 0.0f,PR_WR},
	{HR_UPD_FILE_SIZE_L, -32768, 32767, 0,	0.0f, PR_SV}, 
	{HR_UPD_FILE_SIZE_H, -32768, 32767, 0,	0.0f, PR_SV},
	{HR_UPD_FILE_CRC, -32768, 32767, 0, 0.0f, PR_SV},
	{HR_UPD_TIME_STATMP_L, -32768, 32767, 0, 0.0f,PR_SV},
	{HR_UPD_TIME_STATMP_H, -32768, 32767, 0, 0.0f,PR_SV},
};

uint8_t VAR_GetTableLen(void)
{
	uint8_t len;
	
	len= sizeof(VarValueRangeTable)/sizeof(MB_VAR_SET);
	
	return len;
}

MB_VAR_SET* VAR_GetTablePtr(void)
{
	return (MB_VAR_SET*)(&VarValueRangeTable[0]);
}

float VAR_GetRealValue(uint16_t addr)
{
	uint8_t len;
	uint8_t i;
	float ret_value;
	
	len= VAR_GetTableLen();
	
	ret_value= -9990;
	for(i= 0; i< len;i++)
	{
		if(addr== VarValueRangeTable[i].addr)
		{
			ret_value= HoldingReg[addr];
			if(VarValueRangeTable[i].multipler > 0.0f)
			{
				ret_value*= VarValueRangeTable[i].multipler;
			}
			break;
		}
	}
	
	return ret_value;
}

void VAR_SetRealValue(uint16_t addr, float value, int8_t force)
{
	uint8_t len;
	uint8_t i;
	
	len= VAR_GetTableLen();

	for(i= 0; i< len;i++)
	{
		if(addr== VarValueRangeTable[i].addr 
			&& (VarValueRangeTable[i].prm >= PR_WR || force))
		{
			if(VarValueRangeTable[i].multipler > 0.0f)
			{
				value/= VarValueRangeTable[i].multipler;
			}
			
			value= _hi_lo_limit(value, VarValueRangeTable[i].max_value, VarValueRangeTable[i].min_value);
			
			if(addr== HR_OCC_CLG_SETPT)
			{
				if(value < HoldingReg[HR_OCC_HTG_SETPT] + 20)
				{
					return;
				}
			}
			
			if(addr== HR_OCC_HTG_SETPT)
			{
				if(value + 20 > HoldingReg[HR_OCC_CLG_SETPT])
				{
					return;
				}
			}
			
			if(addr== HR_UNOCC_CLG_SETPT)
			{
				if(value < HoldingReg[HR_OCC_CLG_SETPT] + 50)
				{
					return;
				}
			}
			
			if(addr== HR_UNOCC_HTG_SETPT)
			{
				if(value + 20 > HoldingReg[HR_OCC_HTG_SETPT])
				{
					return;
				}
			}
	
			if(HoldingReg[addr]!= value)
			{
				HoldingReg[addr]= value;
				
				if(VarValueRangeTable[i].prm == PR_SV)
				{
					EEP_Offchip_Save(addr, (uint16_t*)&HoldingReg[addr], 1);
					EEP_Offchip_Reload(addr, (uint16_t*)&HoldingReg[addr], 1);
				}
			}
			break;
		}
	}
}

void VAR_SetIntValue(uint16_t addr, int16_t value, int8_t force)
{
	uint8_t len;
	uint8_t i;
	
	len= VAR_GetTableLen();
	
	for(i= 0; i< len;i++)
	{
		if(addr== VarValueRangeTable[i].addr  && 
			(VarValueRangeTable[i].prm >= PR_WR || force))
		{
			value= _hi_lo_limit(value, VarValueRangeTable[i].max_value, VarValueRangeTable[i].min_value);
			
			if(addr== HR_OCC_CLG_SETPT)
			{
				if(value < HoldingReg[HR_OCC_HTG_SETPT] + 20)
				{
					return;
				}
			}
			
			if(addr== HR_OCC_HTG_SETPT)
			{
				if(value + 20 > HoldingReg[HR_OCC_CLG_SETPT])
				{
					return;
				}
			}
			
			if(addr== HR_UNOCC_CLG_SETPT)
			{
				if(value < HoldingReg[HR_OCC_CLG_SETPT] + 50)
				{
					return;
				}
			}
			
			if(addr== HR_UNOCC_HTG_SETPT)
			{
				if(value + 20 > HoldingReg[HR_OCC_HTG_SETPT])
				{
					return;
				}
			}
			
			if(HoldingReg[addr]!= value)
			{
				HoldingReg[addr]= value;
				
				if(VarValueRangeTable[i].prm == PR_SV
					&& addr!= HR_UPD_FILE_SIZE_L
					&& addr!= HR_UPD_FILE_SIZE_H)
				{
					EEP_Offchip_Save(addr, (uint16_t*)&HoldingReg[addr], 1);
					EEP_Offchip_Reload(addr, (uint16_t*)&HoldingReg[addr], 1);
				}
			}
			break;
		}
	}
}

void VAR_ValueInit(void)
{
	uint8_t len;
	uint8_t i;
	uint16_t sig;
	
	len= VAR_GetTableLen();
	
	// Get Default Value
	for(i= 0; i< len; i++)
	{
		HoldingReg[VarValueRangeTable[i].addr]= VarValueRangeTable[i].def_value;
	}
    // Use Bacnet for test
	printf("[%s:%s:%d] Use Bacnet for test \n",
			__FILE__, __FUNCTION__, __LINE__);
    HoldingReg[HR_USART_TYPE] = 1;
	
}

