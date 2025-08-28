#ifndef __CRC16_MODBUS_H__
#define __CRC16_MODBUS_H__

#include <stdint.h>

uint16_t modbus_crc16(uint8_t *pdata, uint8_t size);

#endif
