#ifndef __VARIABLES_H__
#define __VARIABLES_H__

#include <stdint.h>
#include "bacdef.h"

#define BBO_MAX_QTY		2

typedef enum
{
	PR_RD= 0,
	PR_WR= 1,
	PR_SV= 2
}PERMISSON;

typedef struct
{
	uint16_t addr;
	int16_t min_value;
	int16_t max_value;
	int16_t def_value;
	float multipler;
	PERMISSON prm;
}MB_VAR_SET;

typedef struct
{
	uint16_t addr;
	uint16_t obj_instance;
	char name[24];
	char description[32];
	BACNET_ENGINEERING_UNITS unit;
}BAC_VAR_SET;

typedef struct
{
	uint8_t state_index;
	uint16_t max_state;
}BAC_MSV_STATE;

uint8_t VAR_GetTableLen(void);
MB_VAR_SET* VAR_GetTablePtr(void);
float VAR_GetRealValue(uint16_t addr);

void VAR_SetRealValue(uint16_t addr, float value, int8_t force);
void VAR_SetIntValue(uint16_t addr, int16_t value, int8_t force);
void VAR_ValueInit(void);

#endif



