
#include <stdint.h>
#include "hal_relay.h"

static void hardware_config(void)
{
}

void HAL_Relay_Configuration(void)
{
	hardware_config();
}

void HAL_Relay_SetValue(int8_t ch, uint8_t on)
{
}

