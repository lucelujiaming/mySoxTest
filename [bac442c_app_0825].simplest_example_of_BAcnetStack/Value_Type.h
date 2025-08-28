#ifndef __VALUE_TYPE_H__
#define __VALUE_TYPE_H__

#include <stdint.h>
#include <stdbool.h>
#include "bacenum.h"
 
typedef enum 
{
	VT_UINT16= 0,
	VT_SINT16= 1,
	VT_UINT32= 2,
	VT_SINT32= 3,
	VT_REAL= 4,
	VT_MAXIM= 4
}VALUE_TYPE_T;

float GetRealValueFromInt16(uint16_t *buf, VALUE_TYPE_T type, bool be);
uint8_t SetRealValueToInt16(uint16_t *buf, float value, VALUE_TYPE_T type, bool be);

float GetRealValue_BACnet(uint16_t *buf, BACNET_OBJECT_TYPE type);
uint8_t SetRealValue_BACnet(uint16_t *buf, float value, BACNET_OBJECT_TYPE type);
#endif
