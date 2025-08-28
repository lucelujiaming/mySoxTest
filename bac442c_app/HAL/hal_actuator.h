
#ifndef __HAL_ACTUATOR_H__
#define __HAL_ACTUATOR_H__

#include <stdint.h>

void HAL_ACT_Configuration(void);
uint8_t HAL_ACT_ManualPinPress(void);	
uint8_t HAL_ACT_AdaptPinPress(void);	
uint8_t HAL_ACT_DirectPinPress(void);	
void HAL_ACT_SetIncDirect(uint8_t direct);
void HAL_ACT_SetSpeed(uint16_t sp);
#endif


