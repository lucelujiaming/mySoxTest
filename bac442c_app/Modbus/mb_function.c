
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "common.h"
#include "mb_crc16.h"
#include "mb_function.h"

int16_t mb_frame_npdu_decode(MB_TYPE bus, uint8_t *apdu, uint8_t *pdu, int16_t pdu_len, uint8_t *id, uint16_t *invokeID)
{
    int16_t apdu_len;
    uint16_t crc;
    
    if(bus> MB_TCP || !pdu 
        || !apdu || !pdu_len 
        || !id)
    {
        return 0;
    }
    
    apdu_len= 0;
    
    switch(bus)
    {
        case MB_RTU:
            if(pdu_len >= 5)
            {
                crc= modbus_crc16(pdu, pdu_len-2);
                
                if(pdu[pdu_len-1]== _low_byte_uint16(crc) 
                    && pdu[pdu_len-2]== _high_byte_uint16(crc))
                {
                    *id= pdu[0];
                    
                    apdu_len= pdu_len - 3;    //crc & id
                    memmove(apdu, pdu + 1, apdu_len);
                }
            }
            break;
        case MB_TCP:
            if(pdu_len >= 9)
            {
                if(invokeID 
                    && pdu[2]== 0 && pdu[3]== 0 
                    && apdu_len == pdu[5] && apdu_len <= 254)
                {
                    *id= pdu[6];
                    *invokeID= _get_uint16_from_uint8(pdu, BIG_ENDIAN_DATA);
                    
                    apdu_len= pdu_len - 7;
                    memmove(apdu, pdu + 7, apdu_len);
                }
            }
            break;
        default:
            break;
    }
    
    return apdu_len;
}

int16_t mb_frame_npdu_encode(MB_TYPE bus, uint8_t *pdu, uint8_t *apdu, int16_t apdu_len, uint8_t id, uint16_t invokeID)
{
    int16_t frm_len= 0;
    uint16_t crc;
    
    if(bus> MB_TCP || !pdu || !apdu || !apdu_len)
    {
        return 0;
    }
    
    if(bus== MB_RTU)
    {
        pdu[0]= id;
        memmove(pdu + 1, apdu, apdu_len);
        apdu_len++;
        
        crc= modbus_crc16(pdu, apdu_len);
        pdu[apdu_len]= (uint8_t)(crc>>8);
        pdu[apdu_len + 1]= (uint8_t)(crc& 0x00FF);
        frm_len= apdu_len + 2;
    }
    else
    {
        if(bus== MB_TCP)
        {
            if(apdu_len> 0 && apdu_len <= 254)
            {
                pdu[0]= _high_byte_uint16(invokeID);
                pdu[1]= _low_byte_uint16(invokeID);
                pdu[2]= 0;
                pdu[3]= 0;
                pdu[4]= _high_byte_uint16(apdu_len);
                pdu[5]= _low_byte_uint16(apdu_len);
                pdu[6]= id;
                
                memmove(pdu + 7, apdu, apdu_len);
                frm_len= apdu_len + 7;
            }
        }
    }
    
    return frm_len;
}





