#include <stdint.h>
#include <stdbool.h>

#include "Modbus.h"
#include "modbus_list.h" 
#include "fs.h"

bool ModbusListItemReload(uint16_t idx, MODBUS_LIST_ADDRESS *list)
{
	bool ret_value; 
	
	ret_value= FILE_ReloadParams(_FN_MB_LISRT, (uint32_t)idx*sizeof(MODBUS_LIST_ADDRESS), list, sizeof(MODBUS_LIST_ADDRESS));
	
	return ret_value;
}

bool ModbusListItemSave(uint8_t idx, MODBUS_LIST_ADDRESS *list)
{
	bool ret_value; 
	
	ret_value= FILE_SaveParams(_FN_MB_LISRT, (uint32_t)idx*sizeof(MODBUS_LIST_ADDRESS), list, sizeof(MODBUS_LIST_ADDRESS));
	
	return ret_value;
}

bool ModbusListReload(MODBUS_LIST_ADDRESS *list, uint16_t idx, uint16_t qty)
{
	bool ret_value= false; 
	
	ret_value= FILE_ReloadParams(_FN_MB_LISRT, (uint32_t)idx*sizeof(MODBUS_LIST_ADDRESS), list, (uint32_t)qty*sizeof(MODBUS_LIST_ADDRESS));
		
	return ret_value;
}

bool ModbusListSave(MODBUS_LIST_ADDRESS *list, uint16_t idx, uint16_t qty)
{
	bool ret_value; 
	
	ret_value= FILE_SaveParams(_FN_MB_LISRT, (uint32_t)idx*sizeof(MODBUS_LIST_ADDRESS), list, (uint32_t)qty*sizeof(MODBUS_LIST_ADDRESS));
	
	return ret_value;
}

