#include <stdint.h>
#include <stdbool.h>

#include "bacnet_cfg.h" 
#include "fs.h"

BACNET_CONFIG BACnetConfig=
{
	240001, 127, 127, 1
};

bool BACnetConfigReload(BACNET_CONFIG *cfg)
{
	bool ret_value; 
	
	ret_value= FILE_ReloadParams(_FN_BACNET, 0, cfg, sizeof(BACNET_CONFIG));
	
	return ret_value;
}

bool BACnetConfigSave(BACNET_CONFIG *cfg)
{
	bool ret_value; 
	
	ret_value= FILE_SaveParams(_FN_BACNET, 0, cfg, sizeof(BACNET_CONFIG));
	
	return ret_value;
}

