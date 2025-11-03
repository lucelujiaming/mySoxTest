#include <stdint.h>
#include <stdbool.h>

#include <unistd.h>
#include "at24cxx.h"
#include "_24cxx_dev.h"    

#include "common.h"

#include "hal_i2c1.h"
#include "drv_eep_24cxx.h"

/**
 * @brief  delay function
 * @param  none
 * @retval none
 */
static void page_write_delay(void) 
{
#if 1
      usleep(4000);    
#else
    uint16_t i;
    
    i = 0xFFFF;
    while(i--);
#endif
}


/**
 * @brief  device init
 */
const _24cxx_dev_t at24cxx_dev =
{
    "/dev/i2c-1",                /*i2c device name*/
    0x50,                        /*eeprom address*/
    _24C16_E,                    /*eeprom model,eg AT24C02*/
    0,                            /*no write protect*/
    page_write_delay,            /*delay function*/
    
};

    
/*

void DRV_24Cxx_Wait(void)
{
}

void DRV_24Cxx_WriteData(uint8_t byte_addr, uint8_t* buff, uint16_t size)
{
}
*/
    
#include <string.h>
#include <stdio.h>

void data_printf(uint8_t *buf,int addr,int size)
{    
    int i;    
    
    for(i = 0; i < size; i++)    
    {    
        if( (i % 16) == 0 )         
        {
            printf("\n %.4x|  ", addr);    
        }
        else if( (i % 8) == 0 )             
        {
            printf("  ");    
        }
        printf("%.2x ", *(buf+i));    
        addr++;        
   }    
    printf("\r\n");
}


void DRV_24Cxx_WriteBlock(uint8_t byte_addr, uint8_t* buff, uint16_t size)
{
    uint32_t error = 0;
    _24cxx_dev_t *p = (_24cxx_dev_t*)&at24cxx_dev;
    error = _24cxx_write(p,byte_addr, buff, size);
    if(error)
    {
        printf("[%s:%s:%d] error = %d  \n", 
                __FILE__, __FUNCTION__, __LINE__, error);
    }
    data_printf(buff, 0, size);
}

void DRV_24Cxx_ReadBlock(uint8_t byte_addr, uint8_t* buff, uint16_t size)
{
    uint32_t error = 0;
    _24cxx_dev_t *p = (_24cxx_dev_t*)&at24cxx_dev;    
    error = _24cxx_read(p,byte_addr,buff,size); 
    if(error)
    {
        printf("[%s:%s:%d] error = %d  \n", 
                __FILE__, __FUNCTION__, __LINE__, error);
    }
    data_printf(buff, 0, size);
}

