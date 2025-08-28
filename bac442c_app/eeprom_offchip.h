
#ifndef __EEPROM_OFFCHIP_H__
#define __EEPROM_OFFCHIP_H__

#include <stdint.h>

#define SIGNATURE	0xAA55

#define EEP_ADDR_CONFIG_SIG 0
#define EEP_ADDR_DEVICE_SIG	1
#define EEP_ADDR_BACNET_SIG	2
#define EEP_ADDR_UPDATE_SIG	3

uint16_t EEP_Offchip_ReadWord(uint16_t ofs);
void EEP_Offchip_WriteWord(uint16_t ofs, uint16_t value);

void EEP_Offchip_Motor_Reload(uint16_t addr, uint16_t* dest, uint16_t len);
void EEP_Offchip_Motor_Save(uint16_t addr, uint16_t* src, uint16_t len);

void EEP_Offchip_Reload(uint16_t addr, uint16_t* dest, uint16_t len);
void EEP_Offchip_Save(uint16_t addr, uint16_t* src, uint16_t len);
void EEP_Offchip_Def_Save(uint16_t sig_addr);
void EEP_Offchip_VarTab_Init(void);

extern uint16_t Config_VarCnt;
extern uint16_t Device_VarCnt;
extern uint16_t BACnet_VarCnt;
extern uint16_t Update_VarCnt;
#endif


