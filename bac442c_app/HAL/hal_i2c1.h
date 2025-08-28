#ifndef _HAL_I2C1_H_
#define _HAL_I2C1_H_

#include <stdint.h>
#include <stdbool.h>

#define I2C1_RETY_TIMES 10000

void HAL_I2C1_Configuration(void);
uint8_t HAL_I2C1_WriteDataByte(uint8_t addr, uint8_t reg, uint8_t val);
uint8_t HAL_I2C1_WriteDataBlock(uint8_t addr, uint8_t reg,uint8_t *val,unsigned int len);
uint8_t HAL_I2C1_ReadDataByte(uint8_t addr, uint8_t reg, uint8_t *val);
uint8_t HAL_I2C1_ReadDataBlock(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);

#endif
