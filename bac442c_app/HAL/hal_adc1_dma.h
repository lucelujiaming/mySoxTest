#ifndef __HAL_ADC1_DMA_H__
#define __HAL_ADC1_DMA_H__

#include <stdint.h>
#include "hardware_def.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC_CH	4
#define ADC_SPL	80

#define ADC_REF_2V50				0
#define ADC_SENSOR_INPUT		1
#define ADC_ACT_POSITION		2
#define ADC_MT_CURRENT			3

#define ADC_IN_TYPE_VOL			0
#define ADC_IN_TYPE_NTC			1

extern volatile uint16_t adc_value[ADC_CH];
extern volatile uint8_t adc_ready;
	 
void HAL_ADC1_DMA_Configuration(void);
void HAL_ADC1_GetSampleValue(void);

float HAL_ADC1_GetPowerCompensation(void);

#ifdef TYPE_SEL_SOFTWARE
void HAL_ADC1_InputTypeSet(uint8_t type);
#endif

#ifdef __cplusplus
}
#endif

#endif




