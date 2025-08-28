// Version:1.0

#include "common.h"
#include <math.h>

void _delay_ms(uint16_t time)
{    
   uint16_t i=0;  
   while(time--)
   {
      i=18000;  
      while(i--) ;    
   }
}

uint8_t _high_byte_uint16(uint16_t data)
{
	return (uint8_t)(data>>8);
}

uint8_t _low_byte_uint16(uint16_t data)
{
	return (uint8_t)(data & 0x00FF);
}

uint16_t _high_word_uint32(uint32_t data)
{
	return (uint16_t)((data>>16)&0x0000FFFF);
}

uint16_t _low_word_uint32(uint32_t data)
{
	return (uint16_t)(data & 0x0000FFFF);
}

//Get uint16_t form uint8_t buffer
//be: 0-Little Endian(Low Byte First), 1-Big Endian(High Byte First)
//Modbus:be is Big Endian
uint16_t _get_uint16_from_uint8(uint8_t* pdata, uint8_t be)
{
	if(be)
	{
		return ((((uint16_t)(*pdata)<<8)&0xFF00)|*(pdata+1));
	}
	else
	{
		return ((((uint16_t)(*(pdata+1))<<8)&0xFF00)|*(pdata));
	}
}

//Get uint32_t form uint16_t buffer
//wbe: 0-Little Endian(Low Word First), 1-Big Endian(High Word First)
//Modbus:wbe is Big Endian
uint32_t _get_uint32_from_uint16(uint16_t* pdata, uint8_t wbe)
{
	if(wbe)
	{
		return ((uint32_t)((pdata[0]<<16)&0xFFFF0000) | pdata[1]);
	}
	else
	{
		return ((uint32_t)((pdata[1]<<16)&0xFFFF0000) | pdata[0]);
	}
}

//Get uint32_t form uint8_t buffer
//be: 0-Little Endian(Low Byte First), 1-Big Endian(High Byte First)
//wbe: 0-Little Endian(Low Word First), 1-Big Endian(High Word First)
//Modbus: be= 0, wbe= 1
//MBus: be= 0, wbe= 0
uint32_t _get_uint32_from_uint8_t(uint8_t* pdata, uint8_t be, uint8_t wbe)
{
	uint16_t word[2];
	
	word[0]= _get_uint16_from_uint8(pdata, be);
	word[1]= _get_uint16_from_uint8(pdata+2, be);
	
	if(wbe)
	{
		return ((((uint32_t)(word[0])<<16)&0xFFFF0000) | word[1]);
	}
	else
	{
		return ((((uint32_t)(word[1])<<16)&0xFFFF0000) | word[0]);
	}
}

//Set uint16_t to uint8_t buffer
//wbe: 0-Little Endian(Low Word First), 1-Big Endian(High Word First)
void _set_uint16_to_uint8(uint8_t* pdata,uint16_t data, uint8_t be)
{
	if(be)
	{
		*pdata= (data>>8) & 0x00FF;
		*(pdata + 1)= data & 0x00FF;
	}
	else
	{
		*pdata= data & 0x00FF;
		*(pdata+1)= (data>>8) & 0x00FF;
	}
}

//Set uint32_t to uint16_t buffer
//wbe: 0-Little Endian(Low Word First), 1-Big Endian(High Word First)
void _set_uint32_to_uint16(uint16_t* pdata,uint32_t data, uint8_t wbe)
{
	if(wbe)
	{
		*pdata= (data>>16) & 0x0000FFFF;
		*(pdata + 1)= data & 0x0000FFFF;
	}
	else
	{
		*pdata= data & 0x0000FFFF;
		*(pdata+1)= (data>>16) & 0x0000FFFF;
	}
}

//Set uint32_t to uint8_t buffer
//be: 0-Little Endian(Low Byte First), 1-Big Endian(High Byte First)
//wbe: 0-Little Endian(Low Word First), 1-Big Endian(High Word First)
void _set_uint32_to_uint8(uint8_t* pdata,uint32_t value, uint8_t be, uint8_t wbe)
{
	uint16_t word_hi, word_lo;
	
	word_hi= _high_word_uint32(value);
	word_lo= _low_word_uint32(value);
	
	if(wbe)
	{
		_set_uint16_to_uint8(pdata, word_hi, be);
		_set_uint16_to_uint8(pdata + 2, word_lo, be);
	}
	else
	{
		_set_uint16_to_uint8(pdata, word_lo, be);
		_set_uint16_to_uint8(pdata + 2, word_hi, be);
	}
}

uint32_t _swap_word_int32(uint32_t data)
{
	return (((data<<16)&0xFFFF0000)|((data>>16)&0x0000FFFF));
}

uint16_t _swap_byte_int16(uint16_t data)
{
	return (((data<<8)&0xFF00)|((data>>8)&0x00FF));
}

uint8_t _swap_nibble_int8(uint8_t data)
{
	return (((data<<4)&0xF0)|((data>>4)&0x0F));
}

uint32_t _real_to_word(float data)
{
	return (*((uint32_t *)(&data)));
}

float _word_to_real(uint32_t data)
{
	return (*((float *)(&data)));
}

float _hi_lo_limit(float pv, float hi, float lo)
{
	if(pv > hi)
	{
		pv= hi;
	}
	else
	{
		if(pv < lo)
		{
			pv= lo;
		}
	}
	
	return pv;
}

float _max(float a, float b)
{
	if(a >= b)
		return a;
	else
		return b;
}

float _min(float a, float b)
{
	if(a <= b)
		return a;
	else
		return b;
}

uint8_t _in_range_limit(float pv, float hi, float lo)
{
	if(pv <= hi && pv >= lo)
		return 1;
	
	return 0;
}

uint8_t _in_range_band(float pv, float set, float rate)
{
	return _in_range_limit(pv, set*(1+rate), set*(1-rate));
}

float _ramp(float last, float new_data, float max_delta)
{
	float ret_value;
	
	if(fabs(new_data - last) > max_delta && max_delta > 0.0f)
	{
		if(new_data > last)
		{
			ret_value= last + max_delta;
		}
		else
		{
			ret_value= last - max_delta;
		}
	}
	else
	{
		ret_value= new_data;
	}
	
	return ret_value;
}



