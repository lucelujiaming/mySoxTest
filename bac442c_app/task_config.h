/****************************************Copyright (c)**************************************************
** Modified by:        ???
** Modified date:      2012-05-20 
** Version:            v3.0
** Descriptions:       ????STM32F10x   
**
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:            
** Descriptions:        
**
********************************************************************************************************/
#include <stdint.h>

#define SYSTEM_TICKS_HZ	1000

void Delay(volatile int nCount);
void IWDG_Init(void);
void SysTickInit(uint16_t ticks);
void BSP_SystemInit(void);
void BSP_Init(void);
void RCC_Disable(void);

void IWDG_Configuare(void);
void IWDG_ReloadCounter(void);


void Task_Control(void);
// void Task_MSTP(void);
// void Task_MSTP_Recieve(void);
void Task_Modbus(void);
void Task_Actuator(void);
void App_OS_TimeTickHook(void);

extern float adc_power_compens;
/********************************************************************************************************
**                            End Of File
********************************************************************************************************/
