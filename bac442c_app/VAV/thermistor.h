
#ifndef _THERMISTOR_3950_H_
#define _THERMISTOR_3950_H_

typedef enum
{
	NTC_NONE= 0,
	NTC_10K_B3950= 1,
	NTC_20K_HW= 2,
	NTC_10K_II= 3,
	NTC_10K_III= 4
}NTC_TYPE_T;

float GetThermTemp(float kohm, NTC_TYPE_T type, float max, float min);
float Temp_F2C(float temp);
float Temp_F2C(float temp);

#endif
