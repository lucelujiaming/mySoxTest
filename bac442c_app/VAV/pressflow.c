#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// #include "stm32f10x_dma.h"
// #include "stm32f10x_adc.h"

#include "pressflow.h"
#include "mtimer.h"
#include <math.h>
#include "common.h"
#include "filter_sort.h"
#include "modbus.h"
#include "mb_var_def.h"
#include "drv_sdp800.h"
#include "filter_med_avg.h"

#include "hal_adc1_dma.h"
#include "hal_pwm_tim1_oc1.h"

#define PRESS_SPL	150

#define PRESS_LARGE_ERR	(3.0f)
#define PRESS_LARGE_CNT	30

#define PRESS_EXCEPT_ERR_MAX	20
#define PRESS_EXCEPT_ERR_MIN	0

#define PRESS_SPL_MAX	PRESS_SPL
#define PRESS_SPL_MIN	30

extern volatile uint16_t adc_value[ADC_CH];
extern float power_scale;

float press_value[PRESS_SPL];
volatile float press_avg;
float lc_press, lc_temp;

MTIMER tm_Press;
	
// Unit:Pa
float Press_GetAvgValue(void)
{
	Error Err;
	float temp;
	float sum;
	static uint8_t init= 0;
	static float spl_cnt= PRESS_SPL_MIN;
	static float except_cnt= PRESS_EXCEPT_ERR_MIN;
	float lasrge_err;
	float flow_rate, dp_rate;
	uint8_t i;
	
	if(!init)
	{
		Timer_Init(&tm_Press, 1, 25);
		Timer_Start(&tm_Press);
		init= 1;
	}
	
	if(Timer_Expires(&tm_Press))
	{
		if(HoldingReg[HR_UNIT_RESET])
			return press_avg;

		Err= DRV_SDP800_ReadResults(&lc_press, &temp);
		
		if(Err)	//Error
		{
			press_avg= 999.9f;
		}
		else	
		{
			memmove(&press_value[1], &press_value[0], sizeof(float)*(PRESS_SPL-1));
			press_value[0]= lc_press;
			
			sum= 0;
			for(i= 0; i< PRESS_SPL_MIN/3; i++)
			{
				sum+= press_value[i];
			}
			sum/= PRESS_SPL_MIN/3;
			
			if(fabs(press_avg) > 0.0f)
			{
				dp_rate= sum - press_avg;
				dp_rate/=  press_avg;
				
				flow_rate= HoldingReg[HR_DUCT_AIR_FLOW] - HoldingReg[HR_FLOW_REQUIRE];
				flow_rate/= HoldingReg[HR_FLOW_REQUIRE];
				
				if(fabs(dp_rate) >= 0.15f 	//Flow Rate > 8%
					|| fabs(flow_rate) > 0.05f)
				{
					spl_cnt= PRESS_SPL_MIN;
				}
				else
				{
					spl_cnt++;
				}
			}
			
			lasrge_err= fabs(press_avg*0.10f);
			
			if(lasrge_err < 5.0f)
				lasrge_err= 5.0f;
			
			spl_cnt= _hi_lo_limit(spl_cnt, PRESS_SPL_MAX, PRESS_SPL_MIN);
			
			except_cnt= spl_cnt/10;
			except_cnt= _hi_lo_limit(except_cnt, PRESS_EXCEPT_ERR_MAX, PRESS_EXCEPT_ERR_MIN);
						
			press_avg= (filter_med_avg(press_avg, &press_value[0], spl_cnt,
											lasrge_err, PRESS_LARGE_CNT, except_cnt) + press_avg*3.0f)/4.0f; 
			
			if(spl_cnt>= PRESS_SPL_MAX)
			{
				Timer_SetLimit(&tm_Press, 500);
			}
			else
			{
				Timer_SetLimit(&tm_Press, 200);
			}
			
			if(press_avg < 0.2f)
			{
				press_avg= 0.0f; 
			}
			
			lc_temp= (lc_temp*9.0f + temp)/10.0f; 
		}
	}
	
	HoldingReg[DBG_PRESS_SPL_CNT]= spl_cnt;
	HoldingReg[DBG_PRESS_EXP_CNT]= except_cnt;
	
	return press_avg;
}

float Press_GetSAT(void)
{
	return lc_temp;
}

float Press_GetDuctVelocity(float press, float coeff)
{
	float duct_press;
	float ret_value;
	float air_desity;

	if(press < 0 || press> 999.0f)
	{ 
		return 0;
	}
	
	duct_press=  press;
	duct_press/= coeff;
	air_desity= 1.2f;
	duct_press/= air_desity/2.0f;

	ret_value= sqrtf(duct_press);
	
	return ret_value; 	
}


	
