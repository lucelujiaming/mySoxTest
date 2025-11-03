
#include <stdint.h>
#include <string.h>
#include "drv_eep_24cxx.h" 
#include "variables.h"
#include "eeprom_offchip.h"
#include "modbus.h"
#include "mb_var_def.h"
#include "task_config.h"

#define EEP_OFFCHIP_MAX_SIZE    128

#define EEP_ADDR_MOTOR_BASE        5
#define EEP_MOTOR_SIZE        4

//
#define EEP_ADDR_CONFIG_BASE    10
#define EEP_CONFIG_SIZE    70

#define EEP_ADDR_DEVICE_BASE        80
#define EEP_DEVICE_SIZE        10

#define EEP_ADDR_BACNET_BASE        90
#define EEP_BACNET_SIZE        30

#define EEP_ADDR_UPDATE_BASE        120
#define EEP_UPDATE_SIZE        8

uint16_t VirtVarTab_OffChip[EEP_OFFCHIP_MAX_SIZE];
uint16_t Config_VarCnt;
uint16_t Device_VarCnt;
uint16_t BACnet_VarCnt;
uint16_t Update_VarCnt;

//
uint16_t EEP_Offchip_ReadWord(uint16_t ofs)
{
    uint16_t ret_value;
    
    if(ofs >= EEP_OFFCHIP_MAX_SIZE)
        return 0xFFFF;
    
    DRV_24Cxx_ReadBlock(ofs*sizeof(uint16_t), (uint8_t*)&ret_value, sizeof(uint16_t));
    return ret_value;
}

void EEP_Offchip_WriteWord(uint16_t ofs, uint16_t value)
{
    if(ofs >= EEP_OFFCHIP_MAX_SIZE)
        return;
    
    DRV_24Cxx_WriteBlock(ofs*sizeof(uint16_t), (uint8_t*)&value, sizeof(uint16_t));
}

// Addr to Index
uint16_t EEP_Offchip_AddrToIndex(uint16_t addr)
{
    uint16_t idx;
    
    for(idx= 0;idx < EEP_OFFCHIP_MAX_SIZE; idx++)
    {
        if(addr== VirtVarTab_OffChip[idx])
        {
            return idx;
        }
    }
    
    return EEP_OFFCHIP_MAX_SIZE;
}

// Config Index to Addr
uint16_t EEP_Offchip_IndexToAddr(uint16_t index)
{
    return VirtVarTab_OffChip[index];
}
///////////////////////////////////////////////////////////////////////
// Motor Reload
void EEP_Offchip_Motor_Reload(uint16_t addr, uint16_t* dest, uint16_t len)
{
    if(HoldingReg[HR_UNIT_RESET])
        return;
    
    if(addr + len> EEP_MOTOR_SIZE)
        return;
    
    DRV_24Cxx_ReadBlock((EEP_ADDR_MOTOR_BASE + addr)*sizeof(uint16_t), (uint8_t*)dest, len*sizeof(uint16_t));
}

// Motor Save
void EEP_Offchip_Motor_Save(uint16_t addr, uint16_t* src, uint16_t len)
{
    if(HoldingReg[HR_UNIT_RESET])
        return;
    
    if(addr + len> EEP_MOTOR_SIZE)
        return;
    
    IWDG_ReloadCounter();
    DRV_24Cxx_WriteBlock((EEP_ADDR_MOTOR_BASE + addr)*sizeof(uint16_t), (uint8_t*)src, len*sizeof(uint16_t));
}

// Config Reload 
void EEP_Offchip_Reload(uint16_t addr, uint16_t* dest, uint16_t len)
{
    uint16_t index;
    
    if(HoldingReg[HR_UNIT_RESET])
    return;
    
    index= EEP_Offchip_AddrToIndex(addr);
    
    if(index + len> EEP_OFFCHIP_MAX_SIZE)
        return;
    
    IWDG_ReloadCounter();
    DRV_24Cxx_ReadBlock(index*sizeof(uint16_t), (uint8_t*)dest, len*sizeof(uint16_t));
}

// Config Save
void EEP_Offchip_Save(uint16_t addr, uint16_t* src, uint16_t len)
{
    uint16_t index;
    
    if(HoldingReg[HR_UNIT_RESET])
    return;
    
    index= EEP_Offchip_AddrToIndex(addr);
    if(index + len> EEP_OFFCHIP_MAX_SIZE)
        return;
    
    DRV_24Cxx_WriteBlock(index*sizeof(uint16_t), (uint8_t*)src, len*sizeof(uint16_t));
}

void EEP_Offchip_Def_Save(uint16_t sig_addr)
{
    uint16_t i;
    uint16_t value;
    uint16_t addr;
    uint16_t var_cnt= 0;
    uint16_t addr_base;
    
    if(sig_addr== EEP_ADDR_CONFIG_SIG)
    {
        var_cnt= Config_VarCnt;
        addr_base= EEP_ADDR_CONFIG_BASE;
    }
    else
    {
        if(sig_addr== EEP_ADDR_DEVICE_SIG)
        {
            var_cnt= Device_VarCnt;
            addr_base= EEP_ADDR_DEVICE_BASE;
        }
        else
        {
            if(sig_addr== EEP_ADDR_BACNET_SIG)
            {
                var_cnt= BACnet_VarCnt;
                addr_base= EEP_ADDR_BACNET_BASE;
            }
            else
            {
                if(sig_addr== EEP_ADDR_UPDATE_SIG)
                {
                    var_cnt= Update_VarCnt;
                    addr_base= EEP_ADDR_UPDATE_BASE;
                }
            }
        }
    }
    
    for(i= 0;i< var_cnt;i++)
    {
        addr= VirtVarTab_OffChip[i + addr_base];
        
        if(addr!= 65535)
        {
            value= HoldingReg[addr];
            EEP_Offchip_WriteWord(addr_base + i, value);
        }
    }
    
    if(var_cnt)
        EEP_Offchip_WriteWord(sig_addr, SIGNATURE);
}

void EEP_Offchip_VarTab_Init(void)
{
    uint8_t i;
    uint8_t len;
    MB_VAR_SET* ptr;
    
    memset(VirtVarTab_OffChip, 0xFF, EEP_OFFCHIP_MAX_SIZE*sizeof(uint16_t));
    
    len= VAR_GetTableLen();
    ptr= VAR_GetTablePtr();
    
    Config_VarCnt= 0;
    Device_VarCnt= 0;
    BACnet_VarCnt= 0;
    Update_VarCnt= 0;
    
    for(i= 0;i< len;i++)
    {
        if(ptr->prm== PR_SV)
        {
            if(ptr->addr <=  HR_UNIT_RESET)
            {
                VirtVarTab_OffChip[EEP_ADDR_CONFIG_BASE + Config_VarCnt]= ptr->addr;
                Config_VarCnt++;
            }
            else
            {
                if(ptr->addr <=  HR_DUCT_AREA)
                {
                    VirtVarTab_OffChip[EEP_ADDR_DEVICE_BASE + Device_VarCnt]= ptr->addr;
                    Device_VarCnt++;
                }
                else
                {
                    if(ptr->addr <=  HR_LOCATION + 15)
                    {
                        VirtVarTab_OffChip[EEP_ADDR_BACNET_BASE + BACnet_VarCnt]= ptr->addr;
                        BACnet_VarCnt++;
                    }
                    else
                    {
                        if(ptr->addr <=  HR_UPD_TIME_STATMP_H)
                        {
                            VirtVarTab_OffChip[EEP_ADDR_UPDATE_BASE + Update_VarCnt]= ptr->addr;
                            Update_VarCnt++;
                        }
                    }
                }
            }
        }
        ptr++;
    }
}



