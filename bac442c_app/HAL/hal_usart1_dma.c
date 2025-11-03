//usart1.c

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mtimer.h"
#include "common.h"
#include "usart_config.h"
#include "mstp.h"
#include "task_config.h"
#include "hal_usart1_dma.h"

//MSTP

static uint8_t usart_Protolcol;

/* baud rate */
static uint32_t RS485_Baud;
/* data send and recieve counter */
static volatile uint16_t RS485_Send_Counter, RS485_Recieve_Counter;
/* data send and recieve length */
static volatile uint16_t RS485_Send_Length, RS485_Recieve_Length;
/* MSTP Port */
static struct mstp_port_struct_t* RS485_MSTP_Port;

static uint16_t rcv_len= 0;
// static uint16_t tm_Turnaround;
static uint16_t TturnaroundMax;
static USART_EVENT UsartEvent;
// static uint16_t Usart_DMA_SIZE;
static uint8_t *Usart_Buff= NULL;

//{To be Modified
#define USART1_SILENCE_MODBUS	(30L)
#define USART1_SILENCE_MSTP	(40L)


// static char* Usartx= "/dev/tty1";
static void hardware_config(void)
{
}
//}To be Modified

//-------------------------------------------------------------------------
static void usart_set_event(USART_EVENT ev)
{
	UsartEvent= ev;
}

USART_EVENT HAL_USART1_GetEvent(void)
{
	return UsartEvent;
}

/*
static void uart_dma_configuration()
{
    // 使能DMA串口发送和接受请求
}
 */
 
void HAL_USART1_ProtocolSet(uint8_t protol)
{
	usart_Protolcol= protol;
}

uint8_t HAL_USART1_ProtocolGet(void)
{
	return usart_Protolcol;
}

void HAL_USART1_DMA_Configuration(uint32_t baud, uint16_t parity, uint16_t stop_bits, uint16_t buff_size)
{
}

void HAL_USART1_DMA_Send(uint16_t size)
{
}

//Return Bytes Recieved, and Clear Recieved Byte Count
uint16_t HAL_USART1_DMA_Received(void)
{
	uint16_t ret_value;
	
	ret_value= 0;
	if(HAL_USART1_GetEvent()== EVENT_UART_IDLE)
	{
		usart_set_event(EVENT_UART_NONE);	
 
		ret_value= rcv_len;
		rcv_len= 0;
	}
	return ret_value;
}

uint8_t *HAL_USART1_GetBuff(void)
{
	return (uint8_t*)Usart_Buff; 
}
/////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
* DESCRIPTION: Sets the baud rate for the chip USART
* RETURN:      true if valid baud rate
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
bool RS485_Set_Baud_Rate(uint32_t baud)
{
	if(baud!= 9600 && baud!= 19200 && baud!= 38400 && baud!= 76800 )
		return false;
	
	if(RS485_Baud!= baud)
	{
		// USART1_Send_Disable();
		RS485_Baud= baud;
		UART_Config("/dev/tty1", RS485_Baud, 0, 1);
	}

  return true;
}

/****************************************************************************
* DESCRIPTION: Initializes the RS485 hardware and variables, and starts in
*              receive mode.
* RETURN:      none
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
void RS485_Initialize(uint32_t baud, struct mstp_port_struct_t* mstp_port)
{
	hardware_config();
	RS485_MSTP_Port= mstp_port;
  RS485_Set_Baud_Rate(baud);
	
	TturnaroundMax= 2 + ((USART1_SILENCE_MSTP * 1000UL) / baud);

  return;
}

/****************************************************************************
* DESCRIPTION: Send some data and wait until it is sent
* RETURN:      none
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
void RS485_Send_Frame(uint16_t nbytes)
{      
  if(nbytes) 
	{
	}
}

void USART1_IRQHandler(void)
{
}


