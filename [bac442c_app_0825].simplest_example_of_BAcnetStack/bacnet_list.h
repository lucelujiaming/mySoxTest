#ifndef __BACNET_LIST_H__
#define __BACNET_LIST_H__

#include <stdint.h>
#include <stdbool.h>

#include "pattern.h"
#include "Modbus.h"

typedef struct
{
	uint8_t pattern;
	REG_ADDRESS address;
}BACNET_LIST_ADDRESS;

bool BACnetListReload(uint16_t idx, BACNET_LIST_ADDRESS *list);
bool BACnetListSave(uint8_t idx, BACNET_LIST_ADDRESS *list);
bool BACnetListReloadAll(BACNET_LIST_ADDRESS *list, uint16_t qty);

#endif
