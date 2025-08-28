#include <stdint.h>
#include <string.h>
#include "common.h"
#include "filter_med_avg.h"
#include "mtimer.h"
#include "actuator.h"

#include "hal_adc1_dma.h"
#include "hal_pwm_tim2_oc2.h"

extern float power_scale;

/*
static void hardware_config(void)
{
}
*/

void HAL_PWM_TIM2_OC2_Configuration(void)
{
}

void HAL_PWM_TIM2_OC2_SetValue(uint16_t value)	
{
	value= _hi_lo_limit(value, 1000.0f, 0.0f);
}

