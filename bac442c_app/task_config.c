
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "modbus.h"
#include "mb_var_def.h"
#include "common.h"
#include "vav.h" 
#include "hal_adc1_dma.h"
#include "hal_usart1_dma.h"
#include "hal_usart3_dma.h"
#include "hal_i2c1.h"
#include "hal_relay.h"
#include "hal_occ.h"
#include "hal_pwm_tim1_oc1.h"
#include "hal_actuator.h"
#include "variables.h"
#include "bacnetproc.h"
// #include "bootflag.h"
#include "task_config.h"
#include "actuator.h"
#include "flash.h"
#include "eeprom_offchip.h"
#include "hardware_def.h" 

// extern struct mstp_port_struct_t MSTP_Port;

float adc_power_compens;
uint16_t milli_second_cnt;
uint8_t one_millisec;

void SystemReset(void)
{
}

void GPIO_Def_Configuration(void)
{
}

//初始化看门狗
void IWDG_Configuare(void)
{
}

void IWDG_ReloadCounter(void)
{
}


//系统中断管理
void NVIC_Def_Configuration(void)
{ 
}

void SysTickInit(uint16_t ticks_hz)
{
}

void RCC_Disable(void)
{
}

extern void delay_ms(uint16_t time);

void BSP_SystemInit(void)
{
//    uint32_t vect, base, offset;
    
    IWDG_ReloadCounter();
    NVIC_Def_Configuration();
}

void BSP_Init(void)
{
    // _delay_ms(200);
    
    GPIO_Def_Configuration();
    // Unit EEP Restore 
    VAR_ValueInit();
    
    // HAL Init
    HAL_ADC1_DMA_Configuration(); 
    
#ifdef TYPE_SEL_SOFTWARE
    HAL_ADC1_InputTypeSet(ADC_IN_TYPE_NTC);
#endif
    
    HAL_USART1_ProtocolSet(HoldingReg[HR_USART_TYPE]);
    if(HoldingReg[HR_USART_TYPE]== 0) 
    {
        HAL_USART1_DMA_Configuration(BAUD_MUL_1200(HoldingReg[HR_USART_BAUD]), 
                                                            HoldingReg[HR_USART_PARITY], 
                                                            HoldingReg[HR_USART_PARITY]>0?0:1, 256);
    }
    else
    {
        BacnetInit(); 
    }
    
    HAL_Relay_Configuration();
    HAL_PWM_TIM1_OC1_Configuration();
    
    //
    Modbus_Sevice_Init();
    
    printf("[%s:%s:%d] VAV_Init  \n", 
                __FILE__, __FUNCTION__, __LINE__);
    VAV_Init();
    

#ifndef DEBUG_MODE        
    IWDG_Configuare();  
    IWDG_ReloadCounter();
#endif    
    
    HoldingReg[HR_SW_VERSION]= FIRMWARE_VERSION;
}

void Task_Control(void)
{
    HAL_ADC1_GetSampleValue();
    adc_power_compens= HAL_ADC1_GetPowerCompensation();
    VAV_ControlProc();
}

//   void Task_MSTP(void)
//   {
//       uint16_t len;
//       if(HAL_USART1_ProtocolGet()== TYPE_MODBUS)
//       {
//           len= HAL_USART1_DMA_Received();
//           if(len)    
//           {
//               len= Modbus_Slave_Analysis(MB_RTU, HoldingReg[HR_UNIT_MAC], HAL_USART1_GetBuff(), len);
//               if(len)
//               {
//                   HAL_USART1_DMA_Send(len);
//               }
//           } 
//       }
//       else
//       {
//           BacnetTask();
//       }
//   }
//
//   void Task_MSTP_Recieve(void)
//   {
//       if (MSTP_Port.ReceivedValidFrame == false
//           && MSTP_Port.ReceivedInvalidFrame == false)
//       {
//               MSTP_Receive_Frame_FSM(&MSTP_Port);
//       }
//   }

void Task_Modbus(int fd)
{
    uint16_t len;
    
    len= HAL_USART3_DMA_Received(fd);
    if(len)    
    {
        // printf("[%s:%s:%d] len = %d \n", 
        //        __FILE__, __FUNCTION__, __LINE__, len);
        len= Modbus_Slave_Analysis(MB_RTU, 1, HAL_USART3_GetBuff(), len);
        if(len)
        {
            // printf("[%s:%s:%d] HAL_USART3_DMA_Send len = %d \n", 
            //      __FILE__, __FUNCTION__, __LINE__, len);
            HAL_USART3_DMA_Send(fd, len);
            
            tmr_panel_comm= 0;
        }
    } 
}

void Task_Actuator(void)
{
    ACT_Control();
}

void App_OS_TimeTickHook (void)
{
    one_millisec++;
    
    tm_milli_second++;
    milli_second_cnt++;
    
    if(milli_second_cnt >= 1000)
    {
        milli_second_cnt= 0;
        tm_second++; 
    }
}
