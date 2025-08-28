#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "pid.h"
#include "modbus.h"
#include "mb_var_def.h"

void PID_Init(PID_SET *pid)
{
	pid->err_0= 0.0f;
	pid->err_1= 0.0f;
	pid->err_2= 0.0f;
	pid->err_sum= 0.0f;
	
	pid->output= 0.0f;
	
	if(pid->limit_hi== 0.0f)
	{
		pid->limit_hi= 1.0f;
	}
}

void PID_SetGainCoeff(PID_SET *pid, float kp, float ki, float kd)
{
	pid->Kp= kp;
	pid->Ki= ki;
	pid->Kd= kd;
}


float PID_Gain_Loop(PID_SET *pid, float pv, float setpt)
{
	float ret_value;
	
	if(pid->direct)
	{
		pid->err_0= pv - setpt;
	}
	else
	{
		pid->err_0= setpt - pv;
	}
	
	//P
	pid->vp= pid->Kp*(pid->err_0 - pid->err_1);
	
	//I
	if(!((pid->output >= 1.0f && pid->err_0 > 0.0f) || (pid->output <= 0.0f && pid->err_0 < 00.0f)))
	{
		pid->vi= pid->Ki*pid->err_0;
	}
		
	//D
	pid->vd= pid->Kd*(pid->err_0 - 2.0f*pid->err_1 + pid->err_2);
	pid->err_2= pid->err_1;
	pid->err_1= pid->err_0;
	
	//
	ret_value=  pid->output + pid->vp + pid->vi + pid->vd; 
	ret_value= _hi_lo_limit(ret_value, pid->limit_hi, pid->limit_lo);
	
	if(pid->ramp > 0.0f)
	{
		ret_value= _ramp(pid->output, ret_value, pid->ramp);
	}
	
	pid->output= ret_value;
	
	return ret_value;
}

//
void PID_SetBandCoeff(PID_SET *pid, float pb, float ki, float kd, float ts)
{
	float p, i, d;
	
	p= ts;
	p/= pb;
	
	i= 0.0f;
	if(ki > 0.0f)
	{
		i= 1.0f/(ki*pb)*ts;
	}
	
	d= kd*ts/pb;
	
	PID_SetGainCoeff(pid, p, i, d);
}

float PID_Band_Loop(PID_SET *pid, float pv, float setpt, float offset)
{
	float out_plus, ftemp;
	
	if(pid->direct)
	{
		pid->err_0= pv - setpt;
	}
	else
	{
		pid->err_0= setpt - pv;
	}
	
	//P
	pid->vp= pid->err_0*pid->Kp;
	
	//I
	if(pid->Ki > 0.0f)
	{
		if(!((pid->output >= 1.0f && pid->err_0 > 0.0f) || (pid->output <= 0.0f && pid->err_0 < 0.0f)))
		{
				//Error Sum
			ftemp= (pid->err_0 + pid->err_1)*pid->Ki;
			ftemp/= 2.0f;
			pid->err_sum+= ftemp;
		}
		
		pid->err_sum= _hi_lo_limit(pid->err_sum, 1.0f, -1.0f);
	}
	else
	{
		pid->err_sum= 0;
	}
	pid->vi= pid->err_sum;
	
	//D
	ftemp= pid->err_0 - pid->err_1;
	ftemp*= pid->Kd;
	pid->vd= ftemp;
	pid->err_1= pid->err_0;
	
	//
	out_plus= pid->vp + pid->vi + pid->vd + offset; 
	
	if(pid->ramp > 0.0f)
	{
		out_plus= _ramp(pid->output, out_plus, pid->ramp);
	}
	
	pid->output= _hi_lo_limit(out_plus, 1.0f, 0.0f);
	
	return pid->output;
}

void PID_SetDirect(PID_SET *pid, uint8_t direct)
{
	pid->direct= direct;
}

void PID_LimitSet(PID_SET *pid, float limit_hi, float limit_lo)
{
	pid->limit_hi= limit_hi;
	pid->limit_lo= limit_lo;
}

void PID_RampSet(PID_SET *pid, float ramp)
{
	pid->ramp= ramp;
}

void PID_SetOutput(PID_SET *pid, float out)
{
	pid->output= out;
}

