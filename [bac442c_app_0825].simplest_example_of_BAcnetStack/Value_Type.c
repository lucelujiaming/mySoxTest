

#include "Value_Type.h" 
#include "Common.h"
#include "bacenum.h"

float GetRealValueFromInt16(uint16_t *buf, VALUE_TYPE_T type, bool be)
{
    float ret_value= 0;
    uint32_t u32_value;
    uint16_t u16_value;
    
    switch(type)
    {
        case VT_UINT16:
            u16_value= *buf;
            if(be)
            {
                u16_value= _swap_int16(u16_value);
            }
            ret_value= u16_value;
            break;
        case VT_SINT16:
            u16_value= *buf;
            if(be)
            {
                u16_value= _swap_int16(u16_value);
            }
            ret_value= (int16_t)u16_value;
            break;
        case VT_UINT32:
            if(be)
            {
                ret_value= (uint32_t)_get_int32_int16_modbus_be(buf);
            }
            else
            {
                ret_value= (uint32_t)_get_int32_int16_modbus_le(buf);
            }
            break;
        case VT_SINT32:
            if(be)
            {
                ret_value= (int32_t)_get_int32_int16_modbus_be(buf);
            }
            else
            {
                ret_value= (int32_t)_get_int32_int16_modbus_le(buf);
            }
            break;
        case VT_REAL:
            if(be)
            {
                u32_value= _get_int32_int16_modbus_be(buf);
            }
            else
            {
                u32_value= _get_int32_int16_modbus_le(buf);
            }
            ret_value= *((float*)(&u32_value));
            break;
        default:
            break;
    }
    return ret_value;
}

uint8_t SetRealValueToInt16(uint16_t *buf, float value, VALUE_TYPE_T type, bool be)
{
    uint8_t ret_value= 0;
    uint16_t u16_value = 0;
    int16_t s16_value = 0;
    uint32_t u32_value = 0;
    int32_t s32_value = 0;
    
    switch(type)
    {
        case VT_UINT16:
            u16_value= value;
            if(be)
            {
                u16_value= _swap_int16(u16_value);
            }
            buf[0]= u16_value;
            ret_value= 1;
            break;
        case VT_SINT16:
            s16_value= value;
            if(be)
            {
                u16_value= _swap_int16((uint16_t)s16_value);
            }
            buf[0]= (uint16_t)u16_value;
            ret_value= 1;
            break;
        case VT_UINT32:
            u32_value= value;
            if(be)
            {
                buf[0]= _high_word_int32(u32_value);
                buf[1]= _low_word_int32(u32_value);
            }
            else
            {
                buf[0]= _low_word_int32(u32_value);
                buf[1]= _high_word_int32(u32_value);
            }
            ret_value= 2;
            break;
        case VT_SINT32:
            s32_value= value;
            if(be)
            {
                buf[0]= _high_word_int32((uint32_t)s32_value);
                buf[1]= _low_word_int32((uint32_t)s32_value);
            }
            else
            {
                buf[0]= _low_word_int32((uint32_t)s32_value);
                buf[1]= _high_word_int32((uint32_t)s32_value);
            }
            ret_value= 2;
            break;
        case VT_REAL:
            u32_value= *((uint32_t*)(&value));
            if(be)
            {
                buf[0]= _high_word_int32(u32_value);
                buf[1]= _low_word_int32(u32_value);
            }
            else
            {
                buf[0]= _low_word_int32(u32_value);
                buf[1]= _high_word_int32(u32_value);
            }
            ret_value= 2;
            break;
        default:
            break;
    }
    return ret_value;
}

uint8_t SetRealValue_BACnet(uint16_t *buf, float value, BACNET_OBJECT_TYPE type)
{
    if(type <= OBJECT_ANALOG_VALUE)
    {
        *((float*)buf)= value;
        return 2;
    }
    else
    {
        *buf= value;
        return 1;
    }
}

float GetRealValue_BACnet(uint16_t *buf, BACNET_OBJECT_TYPE type)
{
    if(type <= OBJECT_ANALOG_VALUE)
    {
        return (*((float*)buf));
    }
    else
    {
        return (*buf);
    }
}
