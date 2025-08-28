#ifndef __FILTER_MED_AVG_H__
#define __FILTER_MED_AVG_H__
 
#include <stdint.h>

float filter_med_avg(float pv, float* buff, uint16_t qty, 
															float err_large, uint16_t large_cnt, uint16_t except_cnt);
															
#endif
