#ifndef __ACTUATOR_H__
#define __ACTUATOR_H__

#include <stdint.h>
#include <stdbool.h>

#define ACT_INIT_TYPE_RST		0		//Reset Init
#define ACT_INIT_TYPE_ADP		1		//Adaption Init
#define ACT_INIT_TYPE_USR		2		//User Init
#define ACT_INIT_TYPE_NORMAL		3		//No Init

#define ACT_INIT_CMD_STOP		0
#define ACT_INIT_CMD_ZERO		1
#define ACT_INIT_CMD_FULL		2

//
bool ACT_GetDirection(void); 

void ACT_SetPostion(uint16_t pos);
int16_t ACT_GetPosition(void);

void ACT_Init(void);
void ACT_Control(void);

extern volatile uint16_t motor_curr_set;
#endif
