#include <stdint.h>
#include <stdbool.h>

#include "tcp_list.h"
#include "fs.h"

bool TcpListReload(uint16_t idx, TCP_LIST_ADDRESS *list)
{
	bool ret_value; 
	
	ret_value= FILE_ReloadParams(_FN_TCP_LISRT, (uint32_t)idx*sizeof(TCP_LIST_ADDRESS), list, sizeof(TCP_LIST_ADDRESS));
	
	return ret_value;
}

bool TcpListSave(uint8_t idx, TCP_LIST_ADDRESS *list)
{
	bool ret_value; 
	
	ret_value= FILE_SaveParams(_FN_TCP_LISRT, (uint32_t)idx*sizeof(TCP_LIST_ADDRESS), list, sizeof(TCP_LIST_ADDRESS));
	
	return ret_value;
}

bool TcpListReloadAll(TCP_LIST_ADDRESS *list, uint16_t qty)
{
	uint16_t i;
	bool ret_value= false; 
	
	for(i= 0;i< qty;i++)
	{
		ret_value= FILE_ReloadParams(_FN_TCP_LISRT, (uint32_t)i*sizeof(TCP_LIST_ADDRESS), list + i, sizeof(TCP_LIST_ADDRESS));
		
		if(!ret_value)
		{
			return false;
		}
	}
	
	return true;
}

