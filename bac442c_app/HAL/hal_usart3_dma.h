#ifndef __HAL_USART3_DMA_H__
#define __HAL_USART3_DMA_H__

#include <stdint.h>
#include "usart_config.h"

void HAL_USART3_DMA_Configuration(uint32_t baud, uint16_t parity, uint16_t stop_bits, 
																uint16_t buff_size);

void HAL_USART3_DMA_Send(uint16_t len);
uint16_t HAL_USART3_DMA_Received(void);
USART_EVENT HAL_USART3_GetEvent(void);
uint8_t *HAL_USART3_GetBuff(void);

#endif
