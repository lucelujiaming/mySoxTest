#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "common.h"
#include "filter_sort.h"
#include "filter_med_avg.h"

#define SORT_BUFF_MAX		200
float sort_buff[SORT_BUFF_MAX];

#define LEN_MIN	40
#define LEN_MAX	200

#define ERR_MIN	(0.5f)
#define ERR_MAX	(3.0f)

float filter_med_avg(float pv, float* data, uint16_t qty, 
															float err_large, uint16_t large_cnt, uint16_t except_err)
{ 
	float ret_value= 0;
	float sum;
	uint8_t err_cnt, i;
	uint8_t positive;
	
	if(qty > SORT_BUFF_MAX)
	{
		return (-1);
	}
	
	positive= 0;
	err_cnt= 0;
	sum= 0;
	
	if(err_large > 0.0f)
	{
		for(i= 0;i< large_cnt;i++)
		{
			if(fabs(data[i] - pv) > err_large)
			{
				if(err_cnt== 0)
				{
					if(data[i] > pv)
					{
						positive= 1;
						err_cnt= 1;
						sum= data[i];
					}
					else
					{
						positive= 0;
						err_cnt= 1;
						sum= data[i];
					}
				}
				else
				{
					//Positive Error
					if((data[i] > pv && positive) || (data[i] < pv && !positive))
					{
						err_cnt++;
						sum+= data[i];
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	
	if(err_cnt >= large_cnt)
	{
		ret_value= sum / err_cnt;
		
		for(i= 0;i< qty;i++)
		{
			data[i]= ret_value;
		}
	}
	else
	{
		memmove(sort_buff, data, qty*sizeof(float));
		filter_bubble_sort(sort_buff, qty);	
		
		sum= 0;
		for(i= except_err;i< qty - except_err;i++)
		{
			sum+= sort_buff[i];
		}
		
		ret_value= (sum / (qty - except_err*2));
	}
	
	return ret_value;
}
															
