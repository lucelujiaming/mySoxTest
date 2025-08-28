#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "mtimer.h"
#include "common.h"
#include "modbus.h"
#include "mb_var_def.h"

#include "actuator.h"
#include "filter_med_avg.h"

#include "hal_adc1_dma.h"
#include "hal_actuator.h"
#include "eeprom_offchip.h"
#include "pid.h"

#include "hardware_def.h"

#define ACT_STOP	0
#define ACT_RUN		1

#define MT_ALARM_MS		100
//
//#define PWM_MAX	1320
#define PWM_MAX	1000
#define PWM_MIN	100

#define PULSE_SAMPLE_TIME		50
#define PULSE_SPEED_5_NM		15		//60s
#define PULSE_SPEED_10_NM		32		//60s

#define PULSE_STARTUP			10

#define PID_LOOP_TIME			50

#define SPEED_KP	0.014f
#define SPEED_KI	0.004f
#define SPEED_KD	0.001f

#define CURR_KP	0.0014f
#define CURR_KI	0.0004f
#define CURR_KD	0.0001f

#define DIST_KP	0.2f
#define DIST_KI	0.1f
#define DIST_KD	0.5f

////////////////////////////
#define CUR_300_MA		372
#define CUR_250_MA		310
#define CUR_200_MA		248
#define CUR_150_MA		186
#define CUR_120_MA		149
#define CUR_100_MA		124
#define CUR_90_MA			111
#define CUR_80_MA			99
#define CUR_70_MA			87	
#define CUR_60_MA			75
#define CUR_50_MA			62
#define CUR_30_MA			38

struct _ACT_POS
{
	uint16_t pos_zero;
	uint16_t pos_full;
}ACT_POS;

PID_SET Motor_SpeedPid;
PID_SET Motor_CurrPid;

volatile uint8_t act_run_cmd;		//run command
volatile uint8_t act_run_st;		//run state
volatile uint8_t last_act_run_cmd;

volatile uint8_t act_block_st;
volatile uint8_t last_act_block_st;

volatile uint16_t motor_pwm_curr;
volatile uint16_t motor_pulse_cnt;				//motor pulse counter
volatile uint16_t motor_start_pulse;

volatile uint8_t motor_pulse_to_times;		//motor pulse timeout times
volatile float motor_pulse_speed;			//motor speed pulse
volatile float motor_pulse_speed_set;

volatile float motor_curr_input;
volatile uint16_t motor_curr_set;

volatile float pid_speed_value;
volatile float pid_curr_value;

MTIMER tm_pulse_sample;						//motor pulse sampler timer
MTIMER tm_motor_pid;
uint16_t tm_block_delay;
uint16_t tm_act_stop_delay;

uint8_t act_stop_block;

volatile uint8_t act_init_cmd;
volatile uint8_t act_init_type;

//Define act_percent
volatile float act_percent_input= 0;
volatile float act_percent_set;
volatile float act_delta_percent= 0;

//
bool direct_inc_cmd= false;
bool direct_inc_state= false;
bool direct_ccw= false;

//
volatile bool manual_state= false;

int8_t bldc_hyst_compens;
uint16_t run_current_max;

uint16_t eepAct[4];

uint16_t PulseSpeed= PULSE_SPEED_5_NM;

uint16_t CurrentMax= (CUR_100_MA);
uint16_t PosBlock_Lo, PosBlock_Hi;
//////////////////////////////////////////////////////////////
void ACT_SetPostion(uint16_t pos)
{
	pos= _hi_lo_limit(pos, 1000.0f, 0.0f); 
	
	act_percent_set= pos;
	
	HoldingReg[DBG_ACT_POS_SETPT]= act_percent_set;
	
	if(direct_ccw)
	{
		act_percent_set= 1000- act_percent_set;
	}
	
	act_percent_set= _hi_lo_limit(act_percent_set, 1000.0f, 0.0f); 
}

bool ACT_GetDirection(void)
{
	return direct_ccw;
}

int16_t ACT_GetPosition(void)
{
	int16_t ret_value;
	
	if(direct_ccw)
	{
		ret_value= (1000- act_percent_input);
	}
	else
	{
		ret_value= act_percent_input;
	}
	
	ret_value= _hi_lo_limit(ret_value, 1000, 0);
	
	return ret_value;
}

uint8_t ACT_MotorStart(void)
{
	if(direct_inc_cmd!= direct_inc_state
		|| act_block_st
	  || manual_state
		|| act_stop_block)
	{
		return ACT_STOP;
	}
	
	if(act_run_st== ACT_STOP)
	{
		act_run_cmd= ACT_RUN;
	}
	
	return ACT_RUN;
}

void ACT_MotorStop(void)
{
	act_run_cmd= ACT_STOP;
}

float ACT_CurrentControl(float pv, float set)
{
	static float ret_value= 0.0f;
	
	if(act_run_cmd== ACT_STOP && act_run_st== ACT_STOP)
	{
		PID_Init(&Motor_CurrPid);
		ret_value= 0;
	}
	else
	{
		PID_SetGainCoeff(&Motor_CurrPid, CURR_KP, CURR_KI, CURR_KD);
		ret_value = PID_Gain_Loop(&Motor_CurrPid, pv, set); 
	}
	
	return ret_value;
}

float ACT_SpeedControl(float pv, float set)
{
	static float ret_value= 0.0f;
	
	if(act_run_cmd== ACT_STOP && act_run_st== ACT_STOP)
	{
		PID_Init(&Motor_SpeedPid);
		ret_value= 0;
	}
	else
	{
		PID_SetGainCoeff(&Motor_SpeedPid, SPEED_KP, SPEED_KI, SPEED_KD);
		ret_value = PID_Gain_Loop(&Motor_SpeedPid, pv, set);
	}
	
	return ret_value;
}

void ACT_AdaptionProc(void)
{
	static uint8_t run_once_st= ACT_STOP;
	
  switch(act_init_cmd) 
  {
	case ACT_INIT_CMD_STOP:
		if(act_run_st== ACT_STOP)
		{
			if(act_init_type== ACT_INIT_TYPE_RST)   //ACT Init Full
			{
	#ifdef HR_ACT_CONFIG	
				if(HoldingReg[HR_ACT_CONFIG] & BIT_ACT_DIRECT)				
	#else
				if(HAL_ACT_DirectPinPress())
	#endif
				{
					act_init_cmd= ACT_INIT_CMD_FULL;
				}
				else
				{
					act_init_cmd= ACT_INIT_CMD_ZERO;
				}
			}
			else
			{
				act_init_cmd= ACT_INIT_CMD_ZERO;
			}				
		
			run_once_st= ACT_STOP;
		}
		else
		{
			// If Running, Stop it
			ACT_MotorStop();
		}
		break;
  case ACT_INIT_CMD_ZERO:
		if(run_once_st== ACT_STOP)
		{
			if(act_run_st == ACT_STOP
				&& act_block_st== 0 )
			{
				direct_inc_cmd= false;
				
				if(ACT_MotorStart()== ACT_RUN)
					run_once_st= ACT_RUN;
			}
		}
		else
		{
			if(last_act_block_st== ACT_RUN 
				&& act_block_st== ACT_STOP)
			{
				run_once_st= ACT_STOP;
				
				eepAct[0]= adc_value[ADC_ACT_POSITION];
				act_init_cmd= ACT_INIT_CMD_FULL;
			}
		}
    break;
  case ACT_INIT_CMD_FULL:
		if(run_once_st== ACT_STOP)
		{
			if(act_run_st == ACT_STOP
				&& act_block_st== 0 )
			{
				direct_inc_cmd= true;

				if(ACT_MotorStart()== ACT_RUN)
					run_once_st= ACT_RUN;
			}
		}
		else
		{
			if(last_act_block_st== ACT_RUN 
				&& act_block_st== ACT_STOP)
			{
				run_once_st= ACT_STOP;
				
				eepAct[1]= adc_value[ADC_ACT_POSITION];
				EEP_Offchip_Motor_Save(0,  &eepAct[0], 2);
				
				ACT_POS.pos_full= eepAct[1] *0.995f;
				ACT_POS.pos_zero= eepAct[0] + (eepAct[1] - eepAct[0]) *0.005f;

				PosBlock_Hi= 0;
				PosBlock_Lo= 0;
				HoldingReg[HR_ACT_ADAPT]= 0;
				act_init_type= ACT_INIT_TYPE_NORMAL;
				
				HoldingReg[DBG_ADPTION]++;
			}
		}
    break;
  default:
    break;
  }
}

int16_t ACT_GetPhysicalPosition(void)
{
	static int16_t position= 0.0;
	int16_t value= 0;
	
	if(ACT_POS.pos_full > ACT_POS.pos_zero + 1000)
	{
		value= adc_value[ADC_ACT_POSITION];
	
		if(value <= 3800)
		{
			if(value > ACT_POS.pos_full + 30)
			{
				HoldingReg[HR_UNIT_FAULT]|= (0x0001 << BIT_FLT_POT_HI);
			}
			else
			{
				if(value + 30 < ACT_POS.pos_full)
				{
					HoldingReg[HR_UNIT_FAULT]&= ~(0x0001 << BIT_FLT_POT_HI);
				}
			}
				
			if(value + 30 < ACT_POS.pos_zero && value)
			{
				HoldingReg[HR_UNIT_FAULT]|= (0x0001 << BIT_FLT_POT_LO);
			}
			else
			{
				if(value > ACT_POS.pos_zero + 30 && value)
				{
					HoldingReg[HR_UNIT_FAULT]&= ~(0x0001 << BIT_FLT_POT_LO);
				}
			}
			
			value= _hi_lo_limit(value, ACT_POS.pos_full, ACT_POS.pos_zero);
			
			position= (int32_t)1000*(value- ACT_POS.pos_zero)
								/(ACT_POS.pos_full - ACT_POS.pos_zero);
		}
	}
	
	return position;
}

void ACT_PulseCnt(void)
{
	motor_pulse_cnt++;
	
	if(act_run_st== ACT_STOP)
	{
		motor_start_pulse++;
	}
}

void ACT_Speed_Set(float speed)	//us
{
	motor_pulse_speed_set= PulseSpeed*speed;
}

uint16_t ACT_PID_Control(void)
{
	uint16_t ret_value;
	float pid_value;
	
	motor_curr_set= CurrentMax;
	
	motor_curr_input= (adc_value[ADC_MT_CURRENT] + motor_curr_input)/2;
	
	pid_speed_value= (ACT_SpeedControl(motor_pulse_speed, motor_pulse_speed_set) + pid_speed_value)/2.0f;
	
	pid_value= (ACT_CurrentControl(motor_curr_input, pid_speed_value*motor_curr_set) + pid_curr_value)/2.0f; 
	
	pid_curr_value= _ramp(pid_curr_value, pid_value, 0.02f); 
	
	ret_value= pid_curr_value*PWM_MAX;
	
	if(act_run_cmd== ACT_RUN)
	{
		if(ret_value < PWM_MIN)
			ret_value= PWM_MIN; 
	}
	else
	{
		pid_speed_value= 0.0f;
		pid_curr_value= 0.0f;
	}
		
	return ret_value;
}

//////////////////////////////
void ACT_Init(void)
{
	EEP_Offchip_Motor_Reload(0, &eepAct[0], 2);
		
	if(eepAct[1] > eepAct[0] + 1000
		&& eepAct[1] < eepAct[0] + 4000)
	{
		ACT_POS.pos_full= eepAct[1] *0.995f;
		ACT_POS.pos_zero= eepAct[0] + (eepAct[1] - eepAct[0]) *0.005f;
		act_init_type= ACT_INIT_TYPE_NORMAL;
	}
  else
  {
		act_init_type= ACT_INIT_TYPE_ADP;
    act_init_cmd= ACT_INIT_CMD_STOP;
  }
	
	HAL_ACT_Configuration();
	
	PID_Init(&Motor_SpeedPid);
	PID_Init(&Motor_CurrPid);

	PID_SetDirect(&Motor_SpeedPid, 0);
	PID_SetDirect(&Motor_CurrPid, 0);
	
	Timer_Init(&tm_pulse_sample, 1, PULSE_SAMPLE_TIME);
	Timer_Init(&tm_motor_pid, 1, PID_LOOP_TIME);
	
	Timer_Start(&tm_pulse_sample);
	Timer_Start(&tm_motor_pid);

#ifdef HR_ACT_CONFIG
	if(HoldingReg[HR_ACT_CONFIG] & BIT_ACT_10NM)
#else
	if(HoldingReg[HR_ACT_10_NM])
#endif
	{
		PulseSpeed= PULSE_SPEED_10_NM;
		
		CurrentMax= CUR_100_MA*1.5f;
	}
	
	HAL_ACT_SetIncDirect(1);
	_delay_ms(1);
	HAL_ACT_SetIncDirect(0);
	_delay_ms(1);
}

void ACT_RunStatus(void)
{
	if(Timer_Expires(&tm_pulse_sample))
	{
		if(act_run_st== ACT_RUN)
		{
			if(motor_pulse_cnt < 2)
			{
				if(++motor_pulse_to_times >= 1)
				{
					motor_pulse_to_times= 1;
					act_run_st= ACT_STOP;
					
					if(act_stop_block== 0)
					{
						tm_act_stop_delay= Timer_GetMilliSeconds();
						act_stop_block= 1;
					}
				}
			}
			else
			{
				motor_pulse_to_times= 0;
			}
		
			motor_start_pulse= 0;
		}
		else
		{
			if(motor_start_pulse >= PULSE_STARTUP 
				&& act_run_cmd== ACT_RUN)
			{
				motor_start_pulse= PULSE_STARTUP;
				act_run_st= ACT_RUN;
			}
		}

		motor_pulse_speed= (motor_pulse_cnt + motor_pulse_speed)/2;
		motor_pulse_cnt= 0;
	}
	
	if(act_stop_block
		&& Timer_Eslaped_MilliSec(tm_act_stop_delay) > 100)
	{
		act_stop_block= 0;
	}
}

void ACT_Block_State(void)
{
	if(act_run_cmd== ACT_RUN 
		&& motor_pwm_curr== 1000 
		&& act_run_st== ACT_STOP)
	{ 
		act_block_st= 1;
		
		tm_block_delay= Timer_GetMilliSeconds();
	}
	else
	{
		if(Timer_Eslaped_MilliSec(tm_block_delay) > 500
			&& act_block_st)
		{
			act_block_st= 0;
			
			// Can Not Blocked at the End50
			if(act_init_type== ACT_INIT_TYPE_NORMAL)
			{
				if(direct_inc_cmd)
				{
					PosBlock_Hi= act_percent_input;
				}
				else
				{
					PosBlock_Lo= act_percent_input;
				}
			}
		}
	}
		
	//Bolck Alarm
	if(direct_ccw)
	{
		if(PosBlock_Hi)
			HoldingReg[HR_UNIT_FAULT]|= (0x0001 << BIT_FLT_MOTOR_LO);
		else
			HoldingReg[HR_UNIT_FAULT]&= ~(0x0001 << BIT_FLT_MOTOR_LO);
		
		if(PosBlock_Lo)
			HoldingReg[HR_UNIT_FAULT]|= (0x0001 << BIT_FLT_MOTOR_HI);
		else
			HoldingReg[HR_UNIT_FAULT]&= ~(0x0001 << BIT_FLT_MOTOR_HI);
	}
	else
	{
		if(PosBlock_Hi)
			HoldingReg[HR_UNIT_FAULT]|= (0x0001 << BIT_FLT_MOTOR_HI);
		else
			HoldingReg[HR_UNIT_FAULT]&= ~(0x0001 << BIT_FLT_MOTOR_HI);
		
		if(PosBlock_Lo)
			HoldingReg[HR_UNIT_FAULT]|= (0x0001 << BIT_FLT_MOTOR_LO);
		else
			HoldingReg[HR_UNIT_FAULT]&= ~(0x0001 << BIT_FLT_MOTOR_LO);
	}
}


void ACT_Control(void)
{
	float speed;
	float delta_percent_start, delta_percent_stop;
	
	act_percent_input= ACT_GetPhysicalPosition();	// Read Position Sensor
	
	// Manual Set Position
	if(HAL_ACT_ManualPinPress())
	{ 
		manual_state= true;
	}
	else
	{
		if(manual_state)
		{
			manual_state= false;
		}
	}
	
	//Adapt Config
	if((HAL_ACT_AdaptPinPress()
		|| HoldingReg[HR_ACT_ADAPT])
		&& act_init_type!= ACT_INIT_TYPE_ADP)				
	{
		act_init_type= ACT_INIT_TYPE_ADP;
		act_init_cmd= ACT_INIT_CMD_STOP;
	}

#ifdef HR_ACT_CONFIG		 
	if(HoldingReg[HR_ACT_CONFIG] & BIT_ACT_DIRECT)
	{
		direct_ccw= 1;
	}
	else
	{
		direct_ccw= 0;
	}
#else
	direct_ccw= HAL_ACT_DirectPinPress();
#endif
	
	
	ACT_RunStatus();
	
	ACT_Block_State();	
	//
	if(act_init_type!= ACT_INIT_TYPE_NORMAL)
	{
		ACT_AdaptionProc();
	}
	else
	{
		act_delta_percent= fabs(act_percent_input - act_percent_set);
		
		delta_percent_start= 3.0f;
		delta_percent_stop= 1.0f;
		
		// Position Control
		if(act_delta_percent <= delta_percent_stop)
		{
			ACT_MotorStop();   
		}
		else
		{
			if(act_delta_percent > delta_percent_start)
			{
				if(PosBlock_Lo 
					&& PosBlock_Lo > 50)
				{
					act_init_type= ACT_INIT_TYPE_ADP;
					act_init_cmd= ACT_INIT_CMD_STOP;
				}
				else
				{
					if(PosBlock_Hi > 0
						&& PosBlock_Hi < 950)
					{
						act_init_type= ACT_INIT_TYPE_ADP;
						act_init_cmd= ACT_INIT_CMD_STOP;
					}
					else
					{
						if(act_init_type== ACT_INIT_TYPE_NORMAL)
						{
							if(act_percent_set <= PosBlock_Lo 
								&& PosBlock_Lo)
							{
								act_percent_set= PosBlock_Lo;
							}
							
							if(act_percent_set >= PosBlock_Hi
								&& PosBlock_Hi)
							{
								act_percent_set= PosBlock_Hi;
							}
						}
						
						ACT_MotorStart();
					}
				}
				
				if(act_percent_input < act_percent_set)
				{
					direct_inc_cmd= true;
				}
				else
				{
					direct_inc_cmd= false;
				}
			}
		}
	}
	
	//
	if(direct_inc_cmd!= direct_inc_state)
	{
		if(act_run_st== ACT_STOP)
		{
			if(direct_inc_cmd)
			{
				direct_inc_state= true;
			}
			else
			{
				direct_inc_state= false;
			}
			
			HAL_ACT_SetIncDirect(direct_inc_state); 
		}
		else
		{
			ACT_MotorStop();
		}
	}
	
	// Speed Set
	if(act_run_cmd)
	{
		if(act_init_type!= ACT_INIT_TYPE_NORMAL)
		{
			speed= 2.0f;
		}
		else
		{
			if(act_run_st== ACT_STOP)
			{
				speed= 0.5f; 
			}
			else
			{
				speed= 0.9f; 
			}
		}
		
		ACT_Speed_Set(speed);
	}
	else
	{
		ACT_Speed_Set(0.0f);
	}
	
	if(manual_state || act_block_st)
	{
		ACT_MotorStop();
	}
	
	if(Timer_Expires(&tm_motor_pid))
	{
		motor_pwm_curr= ACT_PID_Control();
	}
	
	if(act_block_st || act_run_cmd== ACT_STOP)
	{
		motor_pwm_curr= 0;
	}
	
	HAL_ACT_SetSpeed(motor_pwm_curr);
	
	last_act_run_cmd= act_run_cmd;
	last_act_block_st= act_block_st;
}

