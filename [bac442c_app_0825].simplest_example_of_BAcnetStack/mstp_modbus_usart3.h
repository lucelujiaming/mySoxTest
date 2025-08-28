#ifndef _MSTP_MODBUS_USART3_H_
#define _MSTP_MODBUS_USART3_H_

#include <stdbool.h>
#include <stdint.h>
#include "mstp.h"
#include "Timer.h"
	
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void USART3_MSTP_SetPort(struct mstp_port_struct_t* port);
struct mstp_port_struct_t* USART3_MSTP_GetPort(void);

uint32_t USART3_GetBaudRate(void);
void USART3_ModbusInitialize(uint32_t baud, uint16_t parity, uint16_t stop_bits, uint16_t ack_to);
void USART3_MstpInitialize(uint32_t baud);
void USART3_SetSendCompleteSem(sem_t* send_sem);
void USART3_SendFrame(uint16_t nbytes);

bool USART3_MODBUS_FrameReceived(void);
bool USART3_Ack_Timeout(void);
uint8_t *USART3_MODBUS_GetBuf(void);

uint8_t USART3_MODBUS_GetFrameLength(void);
void USART3_MODBUS_ClearFrameLength(void);

void USART3_SetModbusMode(bool en);
bool USART3_GetModbusMode(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
