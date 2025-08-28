#ifndef __HAL_RELAY_H__
#define __HAL_RELAY_H__

#include <stdint.h>

void HAL_Relay_Configuration(void);
void HAL_Relay_SetValue(int8_t ch, uint8_t on);
#endif
