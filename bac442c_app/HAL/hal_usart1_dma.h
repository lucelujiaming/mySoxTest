#ifndef __HAL_USART1_DMA_H__
#define __HAL_USART1_DMA_H__

#include <stdint.h>
#include <stdbool.h>
#include "usart_config.h"
#include "mstp.h"

#define TYPE_MODBUS					0
#define TYPE_MSTP_MASTER		1
#define TYPE_MSTP_SLAVE			2

void HAL_USART1_DMA_Configuration(uint32_t baud, uint16_t parity, uint16_t stop_bits, uint16_t buff_size);

void HAL_USART1_DMA_Send(uint16_t len);
uint16_t HAL_USART1_DMA_Received(void);
USART_EVENT HAL_USART1_GetEvent(void);
uint8_t *HAL_USART1_GetBuff(void);

void HAL_USART1_ProtocolSet(uint8_t protol);
uint8_t HAL_USART1_ProtocolGet(void);

uint8_t RS485_Get_Protocol_Type(void);
bool RS485_Set_Baud_Rate(uint32_t baud);
void RS485_Initialize(uint32_t baud, struct mstp_port_struct_t* mstp_port);
void RS485_Send_Frame(uint16_t nbytes);
#endif
