
#include <stdint.h>
#include "modbus.h"
#include "mb_handler.h"

#include "mb_var_def.h"
#include "hal_usart1_dma.h"
#include "mb_function.h"
#include "drv_eep_24cxx.h"
#include "hal_usart1_dma.h"

#include "variables.h"
#include "alarm.h"
#include "vav.h"

service_function Modbus_Service_Function[SERVICE_MAX]= {0};
uint8_t mb_apdu[256];

#if (HREG_MAX > 0)
int16_t HoldingReg[HREG_MAX];

void mb_preset_reg_callback(uint16_t reg_addr, int16_t value)
{
	VAR_SetIntValue(reg_addr, value, 0);
	
	if(reg_addr== HR_STAT_EXT_MIN
		&& (HVAC_MODE)HoldingReg[HR_HVAC_MODE_OVRD]!= MODE_NONE)
	{
		HoldingReg[HR_STAT_EXT_MIN]= HoldingReg[HR_EXT_MIN_SET];
		lc_ext_sec= HoldingReg[HR_EXT_MIN_SET]*60; 
	}
	
	if(reg_addr== HR_UNIT_ALARM)
	{
		Alarm_Clear(&Alm_SAF_Lo_50);
		Alarm_Clear(&Alm_DAT_Lo_8);
		Alarm_Clear(&Alm_DAT_Hi);
	}
}
#endif

#if (IREG_MAX > 0)
int16_t InputReg[IREG_MAX];
#endif

#if (OCOIL_MAX > 0)
uint8_t Coil[OCOIL_MAX];

void mb_force_coil_callback(uint16_t reg_addr, uint8_t value)
{
	Coil[reg_addr]= value;
}
#endif

#if (ICOIL_MAX > 0)
uint8_t InputCoil[ICOIL_MAX];
#endif

///////////////////////////////////////////////////////////////////////////////////
void mb_set_service_handler(MB_SERVICE service_choice, service_function pFunction)
{
	if(service_choice < SERVICE_MAX)
		Modbus_Service_Function[service_choice]= pFunction;
}

//
void Modbus_Sevice_Init(void)
{
#if (OCOIL_MAX > 0)
	mb_set_service_handler(SERVICE_READ_OUTPUT_COIL, mb_read_coil_handler);
	mb_set_service_handler(SERVICE_FORCE_SINGLE_COIL, mb_force_single_coil_handler);
	mb_set_service_handler(SERVICE_FORCE_MULTIPLE_COILS, mb_force_multi_coil_handler);
#endif
	
#if (ICOIL_MAX > 0)
	mb_set_service_handler(SERVICE_READ_INPUT_COIL, mb_read_input_coil_handler);
#endif
	
#if (HREG_MAX > 0)
	mb_set_service_handler(SERVICE_READ_HOLDING_REG, mb_read_holding_reg_handler);
	mb_set_service_handler(SERVICE_PRESET_SINGLE_REG, mb_preset_single_reg_handler);
	mb_set_service_handler(SERVICE_PRESET_MULTIPLE_REG, mb_preset_multi_reg_handler);
#endif
	
#if (IREG_MAX > 0)
	mb_set_service_handler(SERVICE_READ_INPUT_REG, mb_read_input_reg_handler);
#endif
}

//
uint16_t Modbus_Slave_Analysis(MB_TYPE bus, uint8_t unit_addr, uint8_t *pdu, uint8_t pdu_len)
{
	uint8_t id;
	uint16_t len, apdu_len;
	uint16_t ivoke_id;
	MB_SERVICE service;
	
	len= 0;
	apdu_len= mb_frame_npdu_decode(bus, mb_apdu, pdu, pdu_len, &id, &ivoke_id);
	
	if(apdu_len && id== unit_addr)
	{
		service= (MB_SERVICE)mb_apdu[0];
		
		if(service < SERVICE_MAX && Modbus_Service_Function[service])
		{
			len= Modbus_Service_Function[service](mb_apdu, apdu_len);
		}
	}
	
	if(len)
	{
		len= mb_frame_npdu_encode(bus, pdu, mb_apdu, len, id, ivoke_id);
	}
	
	return len;
}


