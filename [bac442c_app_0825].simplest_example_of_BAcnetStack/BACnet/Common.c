// Version:1.0

#include "Common.h"

uint32_t _abs_int32(int32_t data)
{
	return (data>0?data:-data);
}

uint16_t _abs_int16(int16_t data)
{
	return (data>0?data:-data);
}

float _abs_float(float data)
{
	return (data>0?data:-data);
}
/////////////////////////////////////////////////////////
uint16_t _get_int16_int8_big_endian(uint8_t* pdata)
{
	return ((((uint16_t)(*pdata)<<8)&0xFF00)|*(pdata+1));
}

uint16_t _get_int16_int8_little_endian(uint8_t* pdata)
{
	return ((((uint16_t)(*(pdata+1))<<8)&0xFF00)|*(pdata));
}

uint32_t _get_int32_int8_modbus_be(uint8_t *buf)
{
	uint16_t hi_word, lo_word;
	
	lo_word= _get_int16_int8_big_endian(buf + 2);
	hi_word= _get_int16_int8_big_endian(buf);
	
	return ((((uint32_t)(hi_word)<<16)&0xFFFF0000) | lo_word);
}

uint32_t _get_int32_int8_modbus_le(uint8_t *buf)
{
	uint16_t hi_word, lo_word;
	
	hi_word= _get_int16_int8_big_endian(buf + 2);
	lo_word= _get_int16_int8_big_endian(buf);
	
	return ((((uint32_t)(hi_word)<<16)&0xFFFF0000) | lo_word);
}

void _set_int8_int32_modbus_be(uint8_t* buf,uint32_t value)
{
	uint16_t hi_word, lo_word;
	
	hi_word= (value >> 16) & 0x00FF;
	lo_word= value & 0xFF00;
	
	buf[0]= _high_byte_int16(hi_word);
	buf[1]= _low_byte_int16(hi_word);
	buf[2]= _high_byte_int16(lo_word);
	buf[3]= _low_byte_int16(lo_word);
}

void _set_int8_int32_modbus_le(uint8_t* buf,uint32_t value)
{
	uint16_t hi_word, lo_word;
	
	lo_word= (value >> 16) & 0x00FF;
	hi_word= value & 0xFF00;
	
	buf[0]= _high_byte_int16(hi_word);
	buf[1]= _low_byte_int16(hi_word);
	buf[2]= _high_byte_int16(lo_word);
	buf[3]= _low_byte_int16(lo_word);
}

/////////////////////////////////////////////////////////
uint32_t _get_int32_int16_modbus_le(uint16_t* pdata)
{
	return ((((uint32_t)(*(pdata+1))<<16)&0xFFFF0000) | (*pdata));
}

uint32_t _get_int32_int16_modbus_be(uint16_t* pdata)
{
	return ((((uint32_t)(*pdata)<<16)&0xFFFF0000) | (*(pdata + 1)));
}

void _set_int16_int32_modbus_le(uint16_t* pdata,uint32_t data)
{
	*pdata= data & 0x0000FFFF;
	*(pdata+1)= (data>>16) & 0x0000FFFF;
}

void _set_int16_int32_modbus_be(uint16_t* pdata,uint32_t data)
{
	*(pdata + 1)= data & 0x0000FFFF;
	*pdata= (data>>16) & 0x0000FFFF;
}
/////////////////////////////////////////////////////////
uint32_t _get_int32_int8_mbus(uint8_t* pdata)
{
	uint32_t high_word,low_word;
	
	low_word= (uint32_t)_get_int16_int8_little_endian(pdata);
	high_word= (uint32_t)_get_int16_int8_little_endian(pdata+2);
	
	return ((((high_word)<<16)&0xFFFF0000) | low_word);
}

void _set_int8_int32_mbus(uint8_t* pdata,uint32_t value)
{
	uint8_t i;

	for(i= 0;i<4;i++)
	{
		*(pdata+i)= value & 0x000000FF;
		value>>= 8;
	}
}

uint16_t _swap_int16(uint16_t data)
{
	return (((data<<8)&0xFF00)|((data>>8)&0x00FF));
}

uint8_t _swap_int8(uint8_t data)
{
	return (((data<<4)&0xF0)|((data>>4)&0x0F));
}
///////////////////////////////////////////////////////
uint8_t _high_byte_int16(uint16_t data)
{
	return (uint8_t)(data>>8);
}

uint8_t _low_byte_int16(uint16_t data)
{
	return (uint8_t)(data & 0x00FF);
}

uint16_t _high_word_int32(uint32_t data)
{
	return ((uint16_t)((data>>16)&0x0000FFFF));
}

uint16_t _low_word_int32(uint32_t data)
{
	return ((uint16_t)(data & 0x0000FFFF));
}
///////////////////////////////////////////////////////////
uint16_t _BV16(uint8_t i)
{
	return ((uint16_t)1<<i);
}

uint32_t _BV32(uint8_t i)
{
	return ((uint32_t)1<<i);
}
