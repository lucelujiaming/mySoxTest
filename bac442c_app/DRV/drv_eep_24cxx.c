
#include <stdint.h>
#include <stdbool.h>
#include "common.h"

#include "hal_i2c1.h"
#include "drv_eep_24cxx.h"  

static uint16_t retry_times;

void DRV_24Cxx_Wait(void)
{
}

void DRV_24Cxx_WriteData(uint8_t byte_addr, uint8_t* buff, uint16_t size)
{
}



void DRV_24Cxx_WriteBlock(uint8_t byte_addr, uint8_t* buff, uint16_t size)
{
}

void DRV_24Cxx_ReadBlock(uint8_t byte_addr, uint8_t* buff, uint16_t size)
{
}

void DRV_24Cxx_WriteByte(uint8_t byte_addr, uint8_t data)
{
}

uint8_t DRV_24Cxx_ReadByte(uint8_t byte_addr)
{
	uint8_t ret_value= 0;
	return ret_value;
}
