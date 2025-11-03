//usart1.c

#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include "mtimer.h"
#include "common.h"
#include "usart_config.h"
#include "hal_usart3_dma.h"
#include "usart.h"

// static uint16_t rcv_len= 0;
static uint16_t Usart_DMA_SIZE = 0;
static uint8_t *Usart_Buff= NULL;

//{To be Modified
#define USART3_SILENCE	(30L)

// static char* Usartx= "/dev/tty3";

/*
static void hardware_config(void)
{
}
//}To be Modified

//-------------------------------------------------------------------------
static void uart_dma_configuration()
{
}
 */

void HAL_USART3_DMA_Configuration(int fd, uint32_t baud, uint16_t parity, uint16_t stop_bits, uint16_t buff_size)
{
	uint16_t parity_bit, word_len;
	if(parity== 1)
	{
		parity_bit= 'O';
		word_len= 9;
		printf("Set Parity Error\n");
        return ;
	}
	else
	{
		if(parity== 2)
		{
			parity_bit= 'E';
			word_len= 9;
		    printf("Set Parity Error\n");
            return ;
		}
		else
		{
			parity_bit= 'N';
			word_len= 8;
		}
	}
    if(stop_bits)
	{
		stop_bits= 2;
	}
	else
	{
		stop_bits= 1;
	}
    set_speed(fd, baud);
	if (set_parity(fd, word_len, stop_bits, parity_bit) == SERIAL_FALSE)  {
		printf("Set Parity Error\n");
		exit (0);
	}
    Usart_Buff = (uint8_t*)malloc(buff_size);
    Usart_DMA_SIZE = buff_size;
}

void HAL_USART3_DMA_Send(int fd, uint16_t size)
{
    int send_ret = write(fd, Usart_Buff, size);
    if(send_ret != size)
    {
        printf("[%s:%s:%d] send_ret = %d and size = %d \n", 
                __FILE__, __FUNCTION__, __LINE__, send_ret, size);
    }
	// 等待数据发送完成
	tcdrain(fd);
}

void USART3_IRQHandler(void)
{
}

//Return Bytes Recieved, and Clear Recieved Byte Count
uint16_t HAL_USART3_DMA_Received(int fd)
{
	return read(fd, Usart_Buff, Usart_DMA_SIZE);
}

uint8_t *HAL_USART3_GetBuff(void)
{
	return (uint8_t*)Usart_Buff; 
}

