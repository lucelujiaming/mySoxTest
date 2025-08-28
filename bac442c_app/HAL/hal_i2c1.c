
#include <stdlib.h>
#include "hal_i2c1.h"
#include "common.h"

// static uint16_t retry_times; 
/*
static void i2c_delay(uint16_t data)
{
}	
*/
static void i2c_unlock(void)
{
}

static void hardware_config(void)
{
}

void HAL_I2C1_Configuration(void)
{ 
	i2c_unlock();
	hardware_config();
}

uint8_t HAL_I2C1_WriteDataByte(uint8_t addr, uint8_t reg, uint8_t val)
{
	uint8_t error= 0;
	return error;  			 
}

uint8_t HAL_I2C1_WriteDataBlock(uint8_t addr, uint8_t reg,uint8_t *val,unsigned int len)
{
	uint8_t error= 0;
	return error; 
}

uint8_t HAL_I2C1_ReadDataByte(uint8_t addr, uint8_t reg, uint8_t *val)	
{
	uint8_t error= 0;
	return error;  
}

uint8_t HAL_I2C1_ReadDataBlock(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len)
{
	uint8_t error= 0;
	return error; 
}
