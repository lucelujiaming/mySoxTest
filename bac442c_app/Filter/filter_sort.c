#include "filter_sort.h"

void filter_bubble_sort(float *data_array, uint16_t len)
{
	bool is_ok = false;
	uint16_t i,j;
	float temp= 0;

	for(i = 0;i<len-1 && !is_ok;++i)
	{
		is_ok = true;
		for(j = len - 1;j > i;--j)
		{
			if(data_array[j] > data_array[j-1])	//½µĞòÅÅÁĞ
			{
				 temp = data_array[j];
				 data_array[j] = data_array[j-1];
				 data_array[j-1] = temp;
				 is_ok = false;
			}
		}
	}
}
