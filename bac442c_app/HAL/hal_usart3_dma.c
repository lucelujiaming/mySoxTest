//usart1.c

#include <string.h>
#include <stdlib.h>
#include "mtimer.h"
#include "common.h"
#include "usart_config.h"
#include "hal_usart3_dma.h"

static uint16_t rcv_len= 0;
static MTIMER tm_Silence;
static USART_EVENT UsartEvent;
static uint16_t Usart_DMA_SIZE;
static uint8_t *Usart_Buff= NULL;

//{To be Modified
#define USART3_SILENCE	(30L)

static char* Usartx= "/dev/tty3";

static void hardware_config(void)
{
}
//}To be Modified

//-------------------------------------------------------------------------
static void usart_set_event(USART_EVENT ev)
{
	UsartEvent= ev;
}

USART_EVENT HAL_USART3_GetEvent(void)
{
	return UsartEvent;
}

static void uart_dma_configuration()
{
}

void HAL_USART3_DMA_Configuration(uint32_t baud, uint16_t parity, uint16_t stop_bits, uint16_t buff_size)
{
}

void HAL_USART3_DMA_Send(uint16_t size)
{
}

void USART3_IRQHandler(void)
{
}

//Return Bytes Recieved, and Clear Recieved Byte Count
uint16_t HAL_USART3_DMA_Received(void)
{
	uint16_t ret_value;
	
	ret_value= 0;
	if(HAL_USART3_GetEvent()== EVENT_UART_IDLE)
	{
		usart_set_event(EVENT_UART_NONE);	
 
		ret_value= rcv_len;
		rcv_len= 0;
	}
	return ret_value;
}

uint8_t *HAL_USART3_GetBuff(void)
{
	return (uint8_t*)Usart_Buff; 
}

