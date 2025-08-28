#include <stdint.h>
#include <stdbool.h>

#include "bacnet_list.h"
#include "fs.h"

bool BACnetListReload(uint16_t idx, BACNET_LIST_ADDRESS *list)
{
	bool ret_value; 
	
	ret_value= FILE_ReloadParams(_FN_MSTP_LISRT, (uint32_t)idx*sizeof(BACNET_LIST_ADDRESS), list, sizeof(BACNET_LIST_ADDRESS));
	
	return ret_value;
}

bool BACnetListSave(uint8_t idx, BACNET_LIST_ADDRESS *list)
{
	bool ret_value; 
	
	ret_value= FILE_SaveParams(_FN_MSTP_LISRT, (uint32_t)idx*sizeof(BACNET_LIST_ADDRESS), list, sizeof(BACNET_LIST_ADDRESS));
	
	return ret_value;
}

bool BACnetListReloadAll(BACNET_LIST_ADDRESS *list, uint16_t qty)
{
	uint16_t i;
	bool ret_value= false; 
	
	for(i= 0;i< qty;i++)
	{
		ret_value= FILE_ReloadParams(_FN_MSTP_LISRT, (uint32_t)i*sizeof(BACNET_LIST_ADDRESS), list + i, sizeof(BACNET_LIST_ADDRESS));
		
		if(!ret_value)
		{
			return false;
		}
	}
	
	return true;
}

