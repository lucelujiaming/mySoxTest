#ifndef __USART_CONFIG_H__
#define __USART_CONFIG_H__

#include <stdint.h>

#define BAUD_MUL_1200(x)	(x*1200L)
#define BAUD_DIV_1200(x)	(x/1200L)

typedef enum 
{
	EVENT_UART_NONE= 0,
	EVENT_UART_IDLE= 1,
	EVENT_UART_TC= 2
}USART_EVENT;

void UART_Config(char* usart, uint32_t baud, uint16_t parity, uint16_t stop_bits);

#endif
