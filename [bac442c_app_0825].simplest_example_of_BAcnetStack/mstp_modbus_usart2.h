#ifndef _MSTP_MODBUS_USART2_H_
#define _MSTP_MODBUS_USART2_H_

#include <stdbool.h>
#include <stdint.h> 
#include "mstp.h"
#include "Timer.h"
	
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void USART2_MSTP_SetPort(struct mstp_port_struct_t* port);
struct mstp_port_struct_t* USART2_MSTP_GetPort(void);

uint32_t USART2_GetBaudRate(void);
void USART2_ModbusInitialize(uint32_t baud, uint16_t parity, uint16_t stop_bits, uint16_t ack_to);
void USART2_MstpInitialize(uint32_t baud);
void USART2_SetSendCompleteSem(sem_t* send_sem);
void USART2_SendFrame(uint16_t nbytes);

bool USART2_MODBUS_FrameReceived(void);
bool USART2_Ack_Timeout(void);
uint8_t* USART2_MODBUS_GetBuf(void);

uint8_t USART2_MODBUS_GetFrameLength(void);
void USART2_MODBUS_ClearFrameLength(void);

void USART2_SetModbusMode(bool en);
bool USART2_GetModbusMode(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
