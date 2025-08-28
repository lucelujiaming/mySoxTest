#ifndef __PATTERN_H__
#define __PATTERN_H__

#include <stdint.h>
#include <stdbool.h>

#define PATTERN_MAX	10
#define PATTERN_ITEM_MAX	50

// Use Pattern for Valid Variables of Address Table
typedef struct
{
	uint8_t var_type;
	uint32_t var_addr;
	char descrption[64];
}PATTERN_LIST_ADDR;

#endif


