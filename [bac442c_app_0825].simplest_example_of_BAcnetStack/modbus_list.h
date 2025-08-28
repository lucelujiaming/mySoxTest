#ifndef __MODBUS_LIST_H__
#define __MODBUS_LIST_H__

#include <stdint.h>
#include <stdbool.h>
#include "Value_Type.h"
#include "pattern.h"
#include "Modbus.h"

typedef struct
{
	uint8_t tcp_list;
	uint8_t pattern;
	uint8_t com_port;
	REG_ADDRESS address;
	VALUE_TYPE_T value_type;
	uint8_t value32_be;	//Big Endian
}MODBUS_LIST_ADDRESS;

bool ModbusListItemReload(uint16_t idx, MODBUS_LIST_ADDRESS *list);
bool ModbusListItemSave(uint8_t idx, MODBUS_LIST_ADDRESS *list);
bool ModbusListReload(MODBUS_LIST_ADDRESS *list, uint16_t idx, uint16_t qty);
bool ModbusListSave(MODBUS_LIST_ADDRESS *list, uint16_t idx, uint16_t qty);
#endif

