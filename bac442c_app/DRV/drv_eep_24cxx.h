
#ifndef __DRV_24Cxx_H__
#define __DRV_24Cxx_H__

#include <stdint.h>
#include <stdbool.h>

#define I2C_M24C02_ADDR		0xA0

#define EEP_PAGE_SIZE		16
#define EEP_PAGE_MAX		16

void DRV_24Cxx_WriteData(uint8_t byte_addr, uint8_t* buff, uint16_t size);
void DRV_24Cxx_WriteBlock(uint8_t byte_addr, uint8_t* buff, uint16_t size);
void DRV_24Cxx_ReadBlock(uint8_t byte_addr, uint8_t* buff, uint16_t size);

void DRV_24Cxx_WriteByte(uint8_t byte_addr, uint8_t data);
uint8_t DRV_24Cxx_ReadByte(uint8_t byte_addr);

#endif
