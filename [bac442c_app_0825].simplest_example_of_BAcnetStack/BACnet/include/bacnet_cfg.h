#ifndef __BACNET_CONFIG_H__
#define __BACNET_CONFIG_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint32_t instance;
	uint8_t mac_addr;
	uint8_t max_master;
	uint8_t max_info_frame;
}BACNET_CONFIG;

bool BACnetConfigReload(BACNET_CONFIG *cfg);
bool BACnetConfigSave(BACNET_CONFIG *cfg);

extern BACNET_CONFIG BACnetConfig;
#endif
