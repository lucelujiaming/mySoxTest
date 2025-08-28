// Version:1.0

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

#define LIT_ENDIAN_DATA		0
#define BIG_ENDIAN_DATA			1

void _delay_ms(uint16_t time);

uint8_t _high_byte_uint16(uint16_t data);
uint8_t _low_byte_uint16(uint16_t data);

uint16_t _high_word_uint32(uint32_t data);
uint16_t _low_word_uint32(uint32_t data);

//Get uint16_t form uint8_t buffer
//be: 0-Little Endian(Low Byte First), 1-Big Endian(High Byte First)
//Modbus:be is Big Endian
uint16_t _get_uint16_from_uint8(uint8_t* pdata, uint8_t be);

//Get uint32_t form uint16_t buffer
//wbe: 0-Little Endian(Low Word First), 1-Big Endian(High Word First)
//Modbus:wbe is Big Endian
uint32_t _get_uint32_from_uint16(uint16_t* pdata, uint8_t wbe);

//Get uint32_t form uint8_t buffer
//be: 0-Little Endian(Low Byte First), 1-Big Endian(High Byte First)
//wbe: 0-Little Endian(Low Word First), 1-Big Endian(High Word First)
//Modbus: be= 0, wbe= 1
//MBus: be= 0, wbe= 0
uint32_t _get_uint32_from_uint8_t(uint8_t* pdata, uint8_t be, uint8_t wbe);

//Set uint16_t to uint8_t buffer
//wbe: 0-Little Endian(Low Word First), 1-Big Endian(High Word First)
void _set_uint16_to_uint8(uint8_t* pdata,uint16_t data, uint8_t be);

//Set uint32_t to uint16_t buffer
//wbe: 0-Little Endian(Low Word First), 1-Big Endian(High Word First)
void _set_uint32_to_uint16(uint16_t* pdata,uint32_t data, uint8_t wbe);

//Set uint32_t to uint8_t buffer
//be: 0-Little Endian(Low Byte First), 1-Big Endian(High Byte First)
//wbe: 0-Little Endian(Low Word First), 1-Big Endian(High Word First)
void _set_uint32_to_uint8(uint8_t* pdata,uint32_t value, uint8_t be, uint8_t wbe);

uint32_t _swap_word_int32(uint32_t data);
uint16_t _swap_byte_int16(uint16_t data);
uint8_t _swap_nibble_int8(uint8_t data);

uint32_t _real_to_word(float data);
float _word_to_real(uint32_t data);

float _hi_lo_limit(float pv, float hi, float lo);
float _min(float a, float b);
float _max(float a, float b);

uint8_t _in_range_limit(float pv, float hi, float lo);
uint8_t _in_range_band(float pv, float set, float pct);
float _ramp(float last, float new, float max_delta);

#endif
