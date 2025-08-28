
#include <stdio.h>
#include <semaphore.h>
#include "mstp_modbus_usart3.h"
#include "Timer.h"
#include "mstp.h"

#define USART3_RX_ON()	(GPIOB->BRR|=0x0200)
#define USART3_RX_OFF()	(GPIOB->BSRR|=0x0200)
#define USART3_TX_ON()	(GPIOB->BRR|=0x0100)
#define USART3_TX_OFF()	(GPIOB->BSRR|=0x0100)

/* define variables for MODBUS */
static bool usart_modbus_en= false;
static volatile uint8_t usart_modbus_buff[256];
static volatile uint16_t usart_send_length= 0;
static volatile uint16_t usart_send_counter= 0;
static volatile uint16_t usart_recieve_counter= 0;

static volatile MTIMER tm_byte_to= {0};
static volatile MTIMER tm_frame_rcv= {0};
static volatile MTIMER tm_ack_to= {0};
static volatile sem_t *usart_send_complete_sem;

////////////////////////////////////////////////////////////////////////////////////
/* define variables for MS/TP */
/* The minimum time after the end of the stop bit of the final octet of a */
/* received frame before a node may enable its EIA-485 driver: 40 bit times. */
/* At 9600 baud, 40 bit times would be about 4.166 milliseconds */
/* At 19200 baud, 40 bit times would be about 2.083 milliseconds */
/* At 38400 baud, 40 bit times would be about 1.041 milliseconds */
/* At 57600 baud, 40 bit times would be about 0.694 milliseconds */
/* At 76800 baud, 40 bit times would be about 0.520 milliseconds */
/* At 115200 baud, 40 bit times would be about 0.347 milliseconds */
/* 40 bits is 4 octets including a start and stop bit with each octet */
#define USART3_Tturnaround  (40UL)

/* baud rate */
static uint32_t usart_baud;
static volatile struct mstp_port_struct_t* mstp_port;

void USART3_MSTP_SetPort(struct mstp_port_struct_t* port)
{
	mstp_port= port;
}

struct mstp_port_struct_t* USART3_MSTP_GetPort(void)
{
	return (struct mstp_port_struct_t*)mstp_port;
}

uint32_t USART3_GetBaudRate(void)
{
    return usart_baud;
}

void USART3_ModbusInitialize(uint32_t baud, uint16_t parity, uint16_t stop_bits, uint16_t ack_to)
{
  return;
}

void USART3_MstpInitialize(uint32_t baud)
{
  return;
}

void USART3_SetSendCompleteSem(sem_t* send_sem)
{
	usart_send_complete_sem= send_sem;
}

/****************************************************************************
* DESCRIPTION: Send some data and wait until it is sent
* RETURN:      none
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
void USART3_SendFrame(uint16_t nbytes)
{      
}

///////////////////////////////////////////////////////////////////////////////////
void USART3_IRQHandler(void)
{
}

bool USART3_MODBUS_FrameReceived(void)
{
	return Timer_Expires((MTIMER*)&tm_frame_rcv);
}

bool USART3_Ack_Timeout(void)
{
	return Timer_Expires((MTIMER*)&tm_ack_to);
}

uint8_t *USART3_MODBUS_GetBuf(void)
{
	return (uint8_t*)usart_modbus_buff;
}

uint8_t USART3_MODBUS_GetFrameLength(void)
{
	return usart_recieve_counter;
}

void USART3_MODBUS_ClearFrameLength(void)
{
	usart_recieve_counter= 0;
}

void USART3_SetModbusMode(bool en)
{
	usart_modbus_en= en;
}

bool USART3_GetModbusMode(void)
{
	return usart_modbus_en;
}
