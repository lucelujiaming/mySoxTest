#ifndef __HAL_USART3_DMA_H__
#define __HAL_USART3_DMA_H__

#include <stdint.h>
#include "usart_config.h"

void HAL_USART3_DMA_Configuration(int fd, uint32_t baud, uint16_t parity, uint16_t stop_bits, 
																uint16_t buff_size);

void HAL_USART3_DMA_Send(int fd,uint16_t len);
uint16_t HAL_USART3_DMA_Received(int fd);
uint8_t *HAL_USART3_GetBuff();

#endif
