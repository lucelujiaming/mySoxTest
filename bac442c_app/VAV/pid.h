#ifndef __PID_H__
#define __PID_H__

#include <stdint.h>

typedef struct
{
	float output;
	float limit_hi;
	float limit_lo;
	float ramp;
	
	float err_0;
	float err_1;
	float err_2;
	float err_sum;
	
	float Kp;
	float Ki;
	float Kd;
	
	float vp;
	float vi;
	float vd;
	uint8_t direct;
	uint8_t enable;
	uint16_t delay_disable;

}PID_SET;

void PID_Init(PID_SET *pid);
void PID_SetDirect(PID_SET *pid, uint8_t direct);

void PID_SetGainCoeff(PID_SET *pid, 
									float kp, float ki, float kd);

void PID_SetBandCoeff(PID_SET *pid, 
											float pb, float ki, float kd, 
											float loop_s);
	
float PID_Gain_Loop(PID_SET *pid, float pv, float setpt);
float PID_Band_Loop(PID_SET *pid, float pv, float setpt, float offset);
void PID_LimitSet(PID_SET *pid, float limit_hi, float limit_lo);
void PID_RampSet(PID_SET *pid, float ramp);
void PID_SetOutput(PID_SET *pid, float out);

#endif
