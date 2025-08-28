#ifndef __PRESS_FLOW_H__
#define __PRESS_FLOW_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

float Press_GetAvgValue(void);

float Press_GetDuctVelocity(float press, float coeff);
float Press_GetSAT(void);
#endif
