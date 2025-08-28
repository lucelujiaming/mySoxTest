#ifndef __TCP_LIST_H__
#define __TCP_LIST_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint8_t ip[4];
	uint16_t port[2];
}TCP_LIST_ADDRESS;

bool TcpListReload(uint16_t idx, TCP_LIST_ADDRESS *list);
bool TcpbusListSave(uint8_t idx, TCP_LIST_ADDRESS *list);
bool TcpListReloadAll(TCP_LIST_ADDRESS *list, uint16_t qty);

#endif


