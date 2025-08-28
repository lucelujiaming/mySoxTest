
#ifndef __MB_CONFIG_H__
#define __MB_CONFIG_H__

#include <stdint.h>
#include "modbus_def.h"

typedef uint16_t (*service_function) (uint8_t * apdu, uint16_t apdu_len);
typedef void (*mb_callback_function) (uint16_t reg_addr, uint16_t value);

#define HREG_MAX	200
#define IREG_MAX	0
#define OCOIL_MAX	0
#define ICOIL_MAX	0

#if (HREG_MAX > 0)
	extern int16_t HoldingReg[HREG_MAX];
#endif

#if (IREG_MAX > 0)
	extern int16_t InputReg[IREG_MAX];
#endif

#if (OCOIL_MAX > 0)
	extern uint8_t Coil[OCOIL_MAX];
#endif

#if (ICOIL_MAX > 0)
	extern uint8_t InputCoil[ICOIL_MAX];
#endif

void mb_force_coil_callback(uint16_t reg_addr, uint8_t value);
void mb_preset_reg_callback(uint16_t reg_addr, int16_t value);

void mb_set_service_handler(MB_SERVICE service_choice, 
														service_function pFunction);
void Modbus_Sevice_Init(void);

uint16_t Modbus_Slave_Analysis(MB_TYPE bus, uint8_t unit_addr, uint8_t *pdu, uint8_t pdu_len);
#endif
