
#include <stdbool.h>
#include "hal_actuator.h"
#include "common.h"

#include "hardware_def.h"

extern void ACT_PulseCnt(void);

static void hardware_config(void)
{
}

void HAL_PWM_TIM1_OC3_Configuration(void)
{
}

void HAL_ACT_Exti9_Config(void)
{
}

void HAL_ACT_Configuration(void)
{
	hardware_config();
	HAL_PWM_TIM1_OC3_Configuration();
	HAL_ACT_Exti9_Config();
}

uint8_t HAL_ACT_ManualPinPress(void)	
{
	return 1;
}

uint8_t HAL_ACT_AdaptPinPress(void)	
{
	return 1;
}

uint8_t HAL_ACT_DirectPinPress(void)	
{
	return 1;
}

void HAL_ACT_SetIncDirect(uint8_t direct)
{
}

void HAL_ACT_SetSpeed(uint16_t pwm)
{
}

void EXTI9_5_IRQHandler(void)
{
}
