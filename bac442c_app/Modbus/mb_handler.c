
#include <stdint.h>
#include "common.h"
#include "modbus.h"
#include "mb_handler.h"
#include "mb_function.h"

uint16_t mb_read_coil_type(uint8_t * apdu, uint16_t apdu_len, uint8_t *coil_data, uint16_t coil_max)
{
    uint16_t ret_value;
    MB_ERROR except;
    uint16_t reg_addr, reg_pts;
    uint16_t i;
    uint16_t value;
    
    if(apdu_len != 5 || !coil_max)
    {
        return 0;
    }
    
    except= ERR_NONE;
    ret_value= 0;
    
    reg_addr= _get_uint16_from_uint8(apdu + 1, BIG_ENDIAN_DATA);
    reg_pts= _get_uint16_from_uint8(apdu + 3, BIG_ENDIAN_DATA);
    
    if(reg_pts > COIL_POINTS_MAX)    //数据规定长度
    {
        except= ERR_DATA_ADDR;
        goto EXCEPT;
    }
    
    apdu[1]= (reg_pts + 7)/8;
    ret_value= apdu[1] + 2;
    apdu+= 2;

    for(i= 0;i<reg_pts;i++)
    {
        value= 0;
        
        if(reg_addr < coil_max)
        {
            if(coil_data[reg_addr])
            {
                value= 1;
            }
        }
        
        if(value)
        {
            apdu[i/8]|= 0x01<<(i%8);
        }
        else
        {
            apdu[i/8]&= ~(0x01<<(i%8));
        }
        
        reg_addr++;
    }
    
    return    ret_value;
    
EXCEPT:
    ret_value= mb_exception_handler(apdu, except);
    return ret_value;
}

uint16_t mb_read_reg_type(uint8_t * apdu, uint16_t apdu_len, uint16_t *reg_data, uint16_t reg_max)
{
    uint16_t ret_value;
    MB_ERROR except;
    uint16_t reg_addr, reg_pts;
    uint16_t i;
    uint16_t value;
    
    if(apdu_len != 5 || !reg_max)
    {
        return 0;
    }
    
    except= ERR_NONE;
    ret_value= 0;
    
    reg_addr= _get_uint16_from_uint8(apdu + 1, BIG_ENDIAN_DATA);
    reg_pts= _get_uint16_from_uint8(apdu + 3, BIG_ENDIAN_DATA);
    
    if(reg_pts > REG_POINTS_MAX)    //数据规定长度
    {
        except= ERR_DATA_ADDR;
        goto EXCEPT;
    }
    
    apdu[1]= reg_pts *2;
    ret_value= apdu[1] + 2;
    apdu+= 2;

    for(i= 0;i<reg_pts;i++)
    {
        value= 0;
        
        if(reg_addr < reg_max)
        {
            value= reg_data[reg_addr];
        }
        
        apdu[0]= _high_byte_uint16(value);
        apdu[1]= _low_byte_uint16(value);
        
        reg_addr++;
        apdu+= 2;
    }
    
    return    ret_value;
    
EXCEPT:
    ret_value= mb_exception_handler(apdu, except);
    return ret_value;
}

uint16_t mb_exception_handler(uint8_t * apdu, MB_ERROR err_code)
{
    apdu[0]|= 0x80;
    apdu[1]= err_code;
    
    return 2;
}
///////////////////////////////////////////////////////////////////////////////

#if (ICOIL_MAX > 0)
uint16_t mb_read_input_coil_handler(uint8_t * apdu, uint16_t apdu_len)
{
    return mb_read_coil_type(apdu, apdu_len, InputCoil, ICOIL_MAX);
}
#endif


#if (IREG_MAX > 0)
uint16_t mb_read_input_reg_handler(uint8_t * apdu, uint16_t apdu_len)
{
    return mb_read_reg_type(apdu, apdu_len, (uint16_t*)InputReg, IREG_MAX);
}
#endif

//----------------------------------------------------------------------
#if (OCOIL_MAX > 0)
uint16_t mb_read_coil_handler(uint8_t * apdu, uint16_t apdu_len)
{
    return mb_read_coil_type(apdu, apdu_len, Coil, OCOIL_MAX);
}

uint16_t mb_force_single_coil_handler(uint8_t * apdu, uint16_t apdu_len)
{
    uint16_t ret_value;
    MB_ERROR except;
    uint16_t reg_addr;
    uint16_t value;
    
    if(apdu_len != 5 || OCOIL_MAX== 0)
    {
        return 0;
    }
    
    except= ERR_NONE;
    ret_value= 0;
    
    reg_addr= _get_uint16_from_uint8(apdu + 1, BIG_ENDIAN_DATA);
    value= _get_uint16_from_uint8(apdu + 3, BIG_ENDIAN_DATA);
    
    if(reg_addr >= OCOIL_MAX)
    {
        except= ERR_DATA_ADDR;
        goto EXCEPT;
    }
    
    if(Coil[reg_addr]!= value)
    {
        mb_force_coil_callback(reg_addr, value);
    }
    
    return    5;
    
EXCEPT:
    ret_value= mb_exception_handler(apdu, except);
    return ret_value;
}

uint16_t mb_force_multi_coil_handler(uint8_t * apdu, uint16_t apdu_len)
{
    uint16_t ret_value;
    MB_ERROR except;
    uint16_t reg_addr, reg_pts;
    uint16_t i;
    uint16_t value;
    
    if(OCOIL_MAX== 0)
    {
        return 0;
    }
    
    except= ERR_NONE;
    ret_value= 0;
    
    reg_addr= _get_uint16_from_uint8(apdu + 1, BIG_ENDIAN_DATA);
    reg_pts= _get_uint16_from_uint8(apdu + 3, BIG_ENDIAN_DATA);
    
    if(reg_pts > COIL_POINTS_MAX || (apdu[5]!= (reg_pts + 7)/8))    //数据规定长度
    {
        except= ERR_DATA_ADDR;
        goto EXCEPT;
    }
    
    apdu+= 6;
    for(i= 0;i<reg_pts;i++)
    {
        if(reg_addr < OCOIL_MAX)
        {
            value= 0;
            if(apdu[i/8] & (0x01 << (i%8)))
            {
                value= 1;
            }
            
            if(Coil[reg_addr]!= value)
            {
                mb_force_coil_callback(reg_addr, value);
            }
        }
        
        reg_addr++;
    }
    
    return    5;
    
EXCEPT:
    ret_value= mb_exception_handler(apdu, except);
    return ret_value;
}
#endif

//--------------------------------------------------------------------------
#if (HREG_MAX > 0)
uint16_t mb_read_holding_reg_handler(uint8_t * apdu, uint16_t apdu_len)
{
    return mb_read_reg_type(apdu, apdu_len, (uint16_t*)HoldingReg, HREG_MAX);
}

uint16_t mb_preset_single_reg_handler(uint8_t * apdu, uint16_t apdu_len)
{
    uint16_t ret_value;
    MB_ERROR except;
    uint16_t reg_addr;
    uint16_t value;
    
    if(apdu_len != 5 || HREG_MAX== 0)
    {
        return 0;
    }
    
    except= ERR_NONE;
    ret_value= 0;
    
    reg_addr= _get_uint16_from_uint8(apdu + 1, BIG_ENDIAN_DATA);
    value= _get_uint16_from_uint8(apdu + 3, BIG_ENDIAN_DATA);
    
    if(reg_addr >= HREG_MAX)
    {
        except= ERR_DATA_ADDR;
        goto EXCEPT;
    }
    
    if(HoldingReg[reg_addr]!= value)
    {
        mb_preset_reg_callback(reg_addr, value);
    }
    
    return    5;
    
EXCEPT:
    ret_value= mb_exception_handler(apdu, except);
    return ret_value;
}

uint16_t mb_preset_multi_reg_handler(uint8_t * apdu, uint16_t apdu_len)
{
    uint16_t ret_value;
    MB_ERROR except;
    uint16_t reg_addr, reg_pts;
    uint16_t i;
    uint16_t value;
    
    if(HREG_MAX== 0)
    {
        return 0;
    }
    
    except= ERR_NONE;
    ret_value= 0;
    
    reg_addr= _get_uint16_from_uint8(apdu + 1, BIG_ENDIAN_DATA);
    reg_pts= _get_uint16_from_uint8(apdu + 3, BIG_ENDIAN_DATA);
    
    if(reg_pts > REG_POINTS_MAX || (apdu[5]!= reg_pts *2))    //数据规定长度
    {
        except= ERR_DATA_ADDR;
        goto EXCEPT;
    }
    
    apdu+= 6;
    for(i= 0;i<reg_pts;i++)
    {
        if(reg_addr < HREG_MAX)
        {
            value= _get_uint16_from_uint8(apdu, BIG_ENDIAN_DATA);
            
            if(HoldingReg[reg_addr]!= value)
            {
                mb_preset_reg_callback(reg_addr, value);
            }
        }
        
        reg_addr++;
        apdu+= 2;
    }
    
    return    5;
    
EXCEPT:
    ret_value= mb_exception_handler(apdu, except);
    return ret_value;
}
#endif



