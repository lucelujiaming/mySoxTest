#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mtimer.h"
#include <math.h>
#include "common.h"

#include "hal_adc1_dma.h"

#include "hardware_def.h"

volatile uint32_t adc_buf[ADC_SPL][ADC_CH];
volatile uint16_t adc_value[ADC_CH];
volatile uint8_t adc_ready= 0;

static void hardware_config(void)
{
}

static void adc1_dma_config(void)
{
}

static void adc1_config(void)
{
}

void HAL_ADC1_DMA_Configuration(void)
{
	hardware_config();
	adc1_dma_config();
	adc1_config();
}

void HAL_ADC1_GetSampleValue(void)
{
}

float HAL_ADC1_GetPowerCompensation(void)
{
	float ret= 1.0f;
	
	ret= 2.5;
	ret*=4095;
	if(adc_value[ADC_REF_2V50]!= 0.0f)
	{	
		ret/= adc_value[ADC_REF_2V50];
		ret= 3.3f/ret;
	}
	
	if(ret > 1.2f || ret < 0.8f)
	{
		ret= 1.0f;
	}

	return ret;
}

#ifdef TYPE_SEL_SOFTWARE
void HAL_ADC1_InputTypeSet(uint8_t type)
{
}
#endif

void DMA1_Channel1_IRQHandler()
{
}




