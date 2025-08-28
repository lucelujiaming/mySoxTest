
#ifndef __FLASH_UPDATE_H__
#define __FLASH_UPDATE_H__

#include <stdint.h>
#include <string.h>


uint32_t USR_Flash_FillBuffer(uint8_t* buff, 
													uint32_t ofs, 
													uint16_t len);

uint32_t USR_Flash_Write(uint16_t complete);
#endif



