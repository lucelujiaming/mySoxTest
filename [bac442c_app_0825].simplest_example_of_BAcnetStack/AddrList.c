// Unit Addr:Device ID
// Reg Addr:Register Address in Device e.g.:0...9999
// Type Addr:Register Address with Type Base Address e.g.:30001,40001
// List Addr:Register Address in Ram List
// Map Addr:Register Address map to Modbus Addr

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Common.h"
#include "Modbus.h"
#include "AddrList.h"
#include "Value_Type.h" 

//#include "BACnetStruct.h"
#include "bacenum.h"
#include "bacdef.h"
#include "config.h"

#include "modbus_list.h"
#include "bacnet_list.h"
#include "tcp_list.h"
#include "fs.h"
//
TCP_LIST_ADDRESS TcpList[TCP_LIST_MAX];

#define MB_LIST_COM1    0
#define MB_LIST_COM2    1
#define MB_LIST_TCP        2
#define BAC_LIST_MSTP    3

MODBUS_LIST_ADDRESS* ModbusList;        //ExSram
//
ADDRESS_INFO* AddrInfoList;                //ExSram

//
uint16_t *GetVarList;    //ExSram
uint16_t *SetVarList;    //ExSram
uint16_t *SetVarChgMap;        //ExSram

uint16_t ListVarCnt= 0; 
uint16_t BacnetAvCnt;

ADDRESS_INFO* AddrInfoChain_Usart[2];
//////////////////////////////////////////////////////
void List_var_eep_read_param(void)
{
    bool ret_value=false;
    
    ret_value= ModbusListReload(ModbusList, 0, MODBUS_LIST_MAX);
    if(!ret_value){return;}
}

#define GETVAR_LIST_SIZE    (VAR_LENGTH * sizeof(uint16_t))
#define SETVAR_LIST_SIZE    (VAR_LENGTH * sizeof(uint16_t))
#define VAR_CHGMAP_SIZE     (((VAR_LENGTH + 15)/16)*sizeof(uint16_t))
#define MODBUS_LIST_SIZE    (MODBUS_LIST_MAX*sizeof(MODBUS_LIST_ADDRESS))
#define ADDRINFO_LIST_SIZE  (ADDR_INFO_LENGTH*sizeof(ADDRESS_INFO))

#define RAM_SIZE  (GETVAR_LIST_SIZE + SETVAR_LIST_SIZE + VAR_CHGMAP_SIZE + MODBUS_LIST_SIZE + ADDRINFO_LIST_SIZE)
void list_var_init(void)
{
    // uint32_t *ram_addr= malloc(RAM_SIZE + 1024); // Bank1_SRAM3_ADDR;    
    // GetVarList= (uint16_t *)(ram_addr);
    GetVarList = malloc(GETVAR_LIST_SIZE);
    // ram_addr+= (uint32_t)VAR_LENGTH*sizeof(uint16_t);
    memset(GetVarList, 0x00, GETVAR_LIST_SIZE);
    
    // SetVarList= (uint16_t *)(ram_addr);
    SetVarList = malloc(SETVAR_LIST_SIZE);
    // ram_addr+= (uint32_t)VAR_LENGTH*sizeof(uint16_t);
    memset(SetVarList, 0x00, SETVAR_LIST_SIZE);
    
    // SetVarChgMap= (uint16_t *)(ram_addr);
    SetVarChgMap = malloc(VAR_CHGMAP_SIZE);
    // ram_addr+= (uint32_t)((VAR_LENGTH + 15)/16)*sizeof(uint16_t);
    memset(SetVarChgMap, 0x00, VAR_CHGMAP_SIZE);
    
    // ModbusList= (MODBUS_LIST_ADDRESS *)(ram_addr); 
    ModbusList = malloc(MODBUS_LIST_SIZE);
    // ram_addr+= (uint32_t)MODBUS_LIST_MAX*sizeof(MODBUS_LIST_ADDRESS);
    memset(ModbusList, 0x00, MODBUS_LIST_SIZE);
    
    // AddrInfoList = (ADDRESS_INFO*)(ram_addr);
     AddrInfoList = malloc(ADDRINFO_LIST_SIZE);
    // ram_addr+= (uint32_t)ADDR_INFO_LENGTH*sizeof(ADDRESS_INFO);
    memset(AddrInfoList, 0x00, ADDRINFO_LIST_SIZE);


    for(int i= 0;i<MODBUS_LIST_MAX;i++)
    {
        ModbusList[i].address.unit_addr = (i+1) / 2;
        ModbusList[i].address.reg_pts =  4;
        ModbusList[i].address.reg_addr = i * 4 + 4;
        ModbusList[i].value32_be = 1;
        ModbusList[i].value_type = VT_UINT32;
        ModbusList[i].com_port = (i % 2) + 1;
    }

    // memset((void*)Bank1_SRAM3_ADDR, 0, ram_addr - Bank1_SRAM3_ADDR);
   //  memset(ram_addr, 0x00, RAM_SIZE);
    
//    GetVarList= (uint16_t *)FSMC_SRAM_Malloc((uint32_t)VAR_LENGTH*sizeof(uint16_t));
//    SetVarList= (uint16_t *)FSMC_SRAM_Malloc((uint32_t)VAR_LENGTH*sizeof(uint16_t));
//    SetVarChgMap= (uint16_t *)FSMC_SRAM_Malloc((uint32_t)((VAR_LENGTH + 15)/16)*sizeof(uint16_t));
//    ModbusList= (MODBUS_LIST_ADDRESS *)FSMC_SRAM_Malloc((uint32_t)MODBUS_LIST_MAX*sizeof(MODBUS_LIST_ADDRESS));
//    AddrInfoList= (ADDRESS_INFO *)FSMC_SRAM_Malloc((uint32_t)ADDR_INFO_LENGTH*sizeof(ADDRESS_INFO));
    
    List_var_eep_read_param();
}

//////////////////////////////////////////////////////////////
bool Valid_DeviceAddr(uint8_t addr)
{
    if(addr>= 1 && addr <=247)
    {
        return true;
    }

    return false;
}

//地址表有效，返回true
bool Valid_AddrTbl(REG_ADDRESS* addr)
{
    if(Valid_DeviceAddr(addr->unit_addr) 
        && addr->reg_pts > 0
        && addr->reg_addr > 0)
    {
        return true;
    }
    return false;
}

//地址表有效，返回true
bool Valid_AddrList(MODBUS_LIST_ADDRESS* list)
{
    if(Valid_AddrTbl(&list->address) 
        && list->com_port)
    {
        return true;
    }
    return false;
}

bool Valid_AddrInfo(ADDRESS_INFO* addr_info)
{
    if(addr_info->get_var_ptr
        && addr_info->set_var_ptr
        && addr_info->list.modbus_list)
    {
        return true;
    }
    return false;
}

bool AddrTbl_Online(ADDRESS_INFO *addr_info)
{
    if(addr_info->status.online)
    {
        return true;
    }
    return false;
}

ADDRESS_INFO *NextValidAddrInfo(ADDRESS_INFO* head, ADDRESS_INFO *cur_node)
{
    ADDRESS_INFO *node;
    
    if(head)
    {
        if(cur_node== NULL)
        {
            cur_node= head;
            if(cur_node->status.sec_to_refind== 0)
            {
                return cur_node;
            }
        }
        
        node= cur_node->next;
    
        do{
            if(node== NULL)
            {
                break;
            }
            
            if(node->status.sec_to_refind== 0)
            {
                return node;
            }
            
            node= node->next;
            
        }while(cur_node!= node);
    }
    
    return NULL;
}

bool DeviceInAddrTbl(uint16_t unit_addr, uint16_t reg_addr, uint8_t reg_type, REG_ADDRESS* addr_tbl)
{
    if(unit_addr== addr_tbl->unit_addr 
        && reg_type == addr_tbl->reg_type 
        && reg_addr>= addr_tbl->reg_addr 
        && reg_addr < (addr_tbl->reg_addr + addr_tbl->reg_pts))
    {
        return true;
    }
    return false;
}

//
ADDRESS_INFO* AddrInfo_ListAddr(uint16_t lst_addr)
{
    ADDRESS_INFO *addr_info;
    uint16_t reg_pts;
    uint16_t i;
    
    addr_info= AddrInfoList;
    for(i= 0;i< ADDR_INFO_LENGTH;i++)
    {
        if(addr_info->protocol== PROTOCOL_MODBUS)
        {
            reg_pts= addr_info->list.modbus_list->address.reg_pts;
        }
        else
        {
            reg_pts= addr_info->list.bacnet_list->address.reg_pts;
        }
    
        if(Valid_AddrInfo(addr_info) 
            && (lst_addr >= addr_info->list_addr)
            && (lst_addr < addr_info->list_addr + reg_pts))
        {
            return addr_info;
        }
        
        addr_info++;
    }
    
    return NULL;
}

////////////////////////////////////////////////////////////////
void Init_AddrInfoList(void)
{
    uint16_t i, port;
    uint16_t idx;
    ADDRESS_INFO *node_tail[2], *node;    
    MODBUS_LIST_ADDRESS* modbus_list;
    
    list_var_init();
    
    ListVarCnt= 0;
    BacnetAvCnt= 0;
    idx= 0;

    node= AddrInfoList;
    modbus_list= ModbusList;
    for(i= 0;i<MODBUS_LIST_MAX;i++)
    {
        node->protocol= PROTOCOL_MODBUS;
        node->idx= idx;
        node->list.modbus_list= modbus_list;
        node->next= NULL;
        
        printf("[%s:%s:%d] modbus_list->value_type = %d  \n", 
              __FILE__, __FUNCTION__, __LINE__, modbus_list->value_type);
                
        // if(Valid_AddrList(modbus_list))
        {
            // Init status
            node->status.sec_to_refind= 0;
            node->status.multi_set_disable= 0;
            node->status.repeat_times= 3;
            node->status.retry_times= 3;
            // Fixme
            node->status.online = 1;
            
            //
            if(ListVarCnt + modbus_list->address.reg_pts <= VAR_LENGTH)
            {
                node->list_addr= ListVarCnt;
                node->get_var_ptr= (uint16_t *)(GetVarList + ListVarCnt);    //
                node->set_var_ptr= (uint16_t *)(SetVarList + ListVarCnt);    
                node->av_idx= BacnetAvCnt;
            
                ListVarCnt+= modbus_list->address.reg_pts;
                
                printf("[%s:%s:%d] modbus_list->value_type = %d  \n", 
                __FILE__, __FUNCTION__, __LINE__, modbus_list->value_type);
                if(modbus_list->value_type>= VT_UINT32)
                {
                    BacnetAvCnt+= modbus_list->address.reg_pts / 2;
                    printf("[%s:%s:%d] BacnetAvCnt = %d  \n", 
                        __FILE__, __FUNCTION__, __LINE__, BacnetAvCnt);
                }
                else
                {
                    BacnetAvCnt+= modbus_list->address.reg_pts;
                    printf("[%s:%s:%d] BacnetAvCnt = %d  \n", 
                        __FILE__, __FUNCTION__, __LINE__, BacnetAvCnt);
                }
                
                //
                port= modbus_list->com_port - 1;
                if(AddrInfoChain_Usart[port])
                {
                    node_tail[port]->next= node;
                    node_tail[port]= node_tail[port]->next;
                }
                else
                {
                    AddrInfoChain_Usart[port]= node;
                    node_tail[port]= AddrInfoChain_Usart[port];
                }
            }
            else
            {
                break;
            }
        }
                
        idx++;
        node++;
        modbus_list++;
    }

    if(AddrInfoChain_Usart[0])
    {
        node_tail[0]->next= AddrInfoChain_Usart[0];
    }
    
    if(AddrInfoChain_Usart[1])
    {
        node_tail[1]->next= AddrInfoChain_Usart[1];
    }
    printf("[%s:%s:%d] BacnetAvCnt = %d  \n", 
        __FILE__, __FUNCTION__, __LINE__, BacnetAvCnt);
}

uint32_t UnitAddr_ListAddr(uint16_t lst_addr)
{
    ADDRESS_INFO* addr_info;
    
    if(lst_addr < ListVarCnt)
    {
        addr_info= AddrInfo_ListAddr(lst_addr);
        
        if(Valid_AddrInfo(addr_info))
        {
            if(addr_info->protocol== PROTOCOL_MODBUS)
            {
                return addr_info->list.modbus_list->address.unit_addr;
            }
            else
            {
                return addr_info->list.bacnet_list->address.unit_addr;
            }
        }
    }
    
    return 0;
}

void SetVarChgFlag(uint16_t  lst_addr, uint16_t points)
{
    uint16_t i;

    for(i= 0;i<points;i++)
    {
        SetVarChgMap[lst_addr / 16]|= (uint16_t)1<<(lst_addr % 16);
        lst_addr++;
    }
}

void ClrVarChgFlag(uint16_t  lst_addr, uint16_t points)
{
    uint16_t i;

    for(i= 0;i<points;i++)
    {
        SetVarChgMap[lst_addr / 16]&= ~((uint16_t)1<<(lst_addr % 16));
        lst_addr++;
    }
}


bool SetVarIsModified(uint16_t lst_addr)
{
    if(lst_addr < ListVarCnt)
    {
        if(SetVarChgMap[lst_addr/16] & ((uint16_t)1<<(lst_addr % 16)))
        {
            return true;
        }
    }
    return false;
}

//Single AddrInfo
//查找连续的设定标
//首先查设定缓冲区是否有效，后查找连续为1的位
uint16_t SetVarChanged(ADDRESS_INFO *addr_info, uint16_t *reg_offset)
{
    volatile uint16_t words ;
    volatile uint8_t bits;
    uint16_t list_addr;
    uint16_t ret_value= 0;
    bool found= false;
    
    REG_ADDRESS *addr;
    ADDRESS_INFO *node;
    
    node= addr_info;
    
    list_addr= node->list_addr + *reg_offset;
    if(list_addr >= ListVarCnt || node== NULL)
    {
        return 0;
    }
    
    if(node->protocol== PROTOCOL_MODBUS)
    {
        addr= &node->list.modbus_list->address;
    }
    else
    {
        addr= &node->list.bacnet_list->address;
    }
    
    while(list_addr < (node->list_addr + addr->reg_pts))
    {
        words= list_addr / 16;
        bits= list_addr % 16;
        
        if(SetVarChgMap[words]== 0 && bits== 0 
            && list_addr + 16 < (node->list_addr + addr->reg_pts))
        {
            list_addr+= 16;
        }
        else
        {
            if(SetVarChgMap[words] & ((uint16_t)1<<bits))
            {
                ret_value++;
                if(!found)
                {
                    found= true;
                    *reg_offset= (list_addr - node->list_addr);
                }
                
                if(node->status.multi_set_disable)
                {
                    return ret_value;
                }
            }
            else
            {
                if(ret_value)
                {
                    break;
                }
            }
                
            list_addr++;
        }
    }
        
    return ret_value;
}
/////////////////////////////////////////////////////////
bool IsWritable_ListAddr(uint16_t lst_addr)
{
    ADDRESS_INFO *addr_info;
    uint8_t reg_type;

    addr_info= AddrInfo_ListAddr(lst_addr);
    
    if(addr_info)
    {
        if(addr_info->protocol== PROTOCOL_MODBUS)
        {
            reg_type= addr_info->list.modbus_list->address.reg_type;
            if(reg_type== HREG_TYPE || reg_type== OCOIL_TYPE)
            {
                return true;
            }
        }
        else
        {
            reg_type= addr_info->list.bacnet_list->address.reg_type;
            if(reg_type!= OBJECT_ANALOG_INPUT 
                    && reg_type!= OBJECT_BINARY_INPUT
                    && reg_type!= OBJECT_MULTI_STATE_INPUT)
            {
                return true;
            }
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////////////
uint16_t BACnet_AvCount(void)
{
    return BacnetAvCnt;
}

uint16_t BACnet_BvCount(void)
{
    return ADDR_INFO_LENGTH;
}

// av index == av instance, av index != list addr
uint16_t BACnet_AvInstanceToListAddr(uint32_t object_instance)
{
    uint32_t ret_value= 0;
    uint8_t i;
    uint16_t av_points= 0;
    ADDRESS_INFO *addr_info= NULL;
    ADDRESS_INFO *node= NULL;
    bool double_word= false;
    
    for(i= 0;i< 2;i++)
    {
        addr_info= AddrInfoChain_Usart[i];
        
        if(addr_info)
        {
            node= addr_info;
            
            do{
                double_word= false;
                if(node->protocol== PROTOCOL_MODBUS)
                {
                    av_points= node->list.modbus_list->address.reg_pts;
                    
                    if(node->list.modbus_list->value_type >=2)
                    {
                        double_word= true;
                        av_points/= 2;
                    }
                }
                else
                {
                    av_points= node->list.bacnet_list->address.reg_pts;
                    
                    if(node->list.bacnet_list->address.reg_type <= 2)
                    {
                        double_word= true;
                        av_points/= 2;
                    }
                }
                
                if(object_instance >= node->av_idx && object_instance < node->av_idx + av_points)
                {
                    if(double_word)
                    {
                        ret_value= node->list_addr + (object_instance - node->av_idx) * 2;
                    }
                    else
                    {
                        ret_value= node->list_addr + (object_instance - node->av_idx);
                    }
                    return ret_value;
                }
                
                node= node->next;
            }while(node!= addr_info);
        }
    }
    
    return ret_value;
}

float BACnet_AvPresentValue(uint32_t object_instance)
{
    float ret_value= 0.0;
    uint16_t reg_offset;
    uint16_t list_addr;
    ADDRESS_INFO *addr_info;
    
    list_addr= BACnet_AvInstanceToListAddr(object_instance);
    
    addr_info= AddrInfo_ListAddr(list_addr);
    
   //     printf("[%s:%s:%d] addr_info->status.online = %d  \n", 
   //               __FILE__, __FUNCTION__, __LINE__, addr_info->status.online);
    if(Valid_AddrInfo(addr_info) && addr_info)
    {
        reg_offset= list_addr - addr_info->list_addr;
        
        if(AddrTbl_Online(addr_info))
        {
            // printf("[%s:%s:%d] addr_info->protocol = %d  \n", 
            //          __FILE__, __FUNCTION__, __LINE__, addr_info->protocol);
            if(addr_info->protocol== PROTOCOL_MODBUS)
            {
                printf("[%s:%s:%d] addr_info->get_var_ptr = %08X + %02X  \n", 
                          __FILE__, __FUNCTION__, __LINE__, addr_info->get_var_ptr, reg_offset);
                ret_value= GetRealValueFromInt16(addr_info->get_var_ptr + reg_offset, 
                                                                                addr_info->list.modbus_list->value_type, 
                                                                                addr_info->list.modbus_list->value32_be); 
                printf("[%s:%s:%d] ret_value = %f  \n", 
                          __FILE__, __FUNCTION__, __LINE__, ret_value);
            }
            else
            {
                ret_value= GetRealValue_BACnet(addr_info->get_var_ptr + reg_offset,
                                                                            (BACNET_OBJECT_TYPE)addr_info->list.bacnet_list->address.reg_type);
                
                printf("[%s:%s:%d] ret_value = %f  \n", 
                          __FILE__, __FUNCTION__, __LINE__, ret_value);
            }
        }
    }
    
    return ret_value;
}

char *BACnetAnalogValueDescription(uint32_t object_instance)
{
    static char text_string[32]="";
    uint16_t list_addr;
    ADDRESS_INFO *addr_info;
    
    list_addr= BACnet_AvInstanceToListAddr(object_instance);
    addr_info= AddrInfo_ListAddr(list_addr);
    if(addr_info== NULL)
    {
        sprintf(text_string, "Error");
        return text_string;
    }
    
    if(addr_info->protocol== PROTOCOL_MODBUS)
    {
        switch(addr_info->list.modbus_list->value_type)
        {
        case VT_UINT16:
            sprintf(text_string, "uint16");
            break;
        case VT_SINT16:
            sprintf(text_string, "int16");
            break;
        case VT_UINT32:
            sprintf(text_string, "uint32");
            break;
        case VT_SINT32:
            sprintf(text_string, "int32");
            break;
        case VT_REAL:
            sprintf(text_string, "real");
            break;
        default:
            break;
        }
    }
    else
    {
        switch(addr_info->list.bacnet_list->address.reg_type)
        {
        case OBJECT_ANALOG_INPUT:
            sprintf(text_string, "analog input");
            break;
        case OBJECT_ANALOG_OUTPUT:
            sprintf(text_string, "analog output");
            break;
        case OBJECT_ANALOG_VALUE:
            sprintf(text_string, "analog value");
            break;
        case OBJECT_BINARY_INPUT:
            sprintf(text_string, "binary input");
            break;
    case OBJECT_BINARY_OUTPUT:
            sprintf(text_string, "binary output");
            break;
    case OBJECT_BINARY_VALUE:
            sprintf(text_string, "binary value");
            break;
        case OBJECT_MULTI_STATE_INPUT:
            sprintf(text_string, "multi state input");
            break;
    case OBJECT_MULTI_STATE_OUTPUT:
            sprintf(text_string, "multi state output");
            break;
    case OBJECT_MULTI_STATE_VALUE:
            sprintf(text_string, "multi state value");
            break;
        default:
            break;
        }
    }
   
  return text_string;
}

char *BACnetAnalogValueName(uint32_t object_instance)
{
    static char text_string[64]="";
    uint16_t reg_offset;
    uint16_t list_addr;
    ADDRESS_INFO *addr_info;
    
    list_addr= BACnet_AvInstanceToListAddr(object_instance);
    addr_info= AddrInfo_ListAddr(list_addr);
    if(addr_info== NULL)
    {
        sprintf(text_string, "Error");
        return text_string;
    }
    
    reg_offset= list_addr - addr_info->list_addr;
    
    if(addr_info->protocol== PROTOCOL_MODBUS)
    {
        sprintf(text_string, "mbrtu %d-%dx-%u", 
                            addr_info->list.modbus_list->address.unit_addr, 
                            addr_info->list.modbus_list->address.reg_type,
                            addr_info->list.modbus_list->address.reg_addr + reg_offset);
    }
    else
    {
        switch(addr_info->list.bacnet_list->address.reg_type)
        {
        case OBJECT_ANALOG_INPUT:
            sprintf(text_string, "mstp %d-ai-%d", 
                            addr_info->list.bacnet_list->address.unit_addr, 
                            addr_info->list.bacnet_list->address.reg_addr + reg_offset);
            break;
        case OBJECT_ANALOG_OUTPUT:
            sprintf(text_string, "mstp %d-ao-%d", 
                            addr_info->list.bacnet_list->address.unit_addr, 
                            addr_info->list.bacnet_list->address.reg_addr + reg_offset);
            break;
        case OBJECT_ANALOG_VALUE:
            sprintf(text_string, "mstp %d-av-%d", 
                            addr_info->list.bacnet_list->address.unit_addr, 
                            addr_info->list.bacnet_list->address.reg_addr);
            break;
        case OBJECT_BINARY_INPUT:
            sprintf(text_string, "mstp %d-bi-%d", 
                            addr_info->list.bacnet_list->address.unit_addr, 
                            addr_info->list.bacnet_list->address.reg_addr + reg_offset);
            break;
    case OBJECT_BINARY_OUTPUT:
            sprintf(text_string, "mstp %d-bo-%d", 
                            addr_info->list.bacnet_list->address.unit_addr, 
                            addr_info->list.bacnet_list->address.reg_addr);
            break;
    case OBJECT_BINARY_VALUE:
            sprintf(text_string, "mstp %d-bv-%d", 
                            addr_info->list.bacnet_list->address.unit_addr, 
                            addr_info->list.bacnet_list->address.reg_addr + reg_offset);
            break;
        case OBJECT_MULTI_STATE_INPUT:
            sprintf(text_string, "mstp %d-msi-%d", 
                            addr_info->list.bacnet_list->address.unit_addr, 
                            addr_info->list.bacnet_list->address.reg_addr + reg_offset);
            break;
    case OBJECT_MULTI_STATE_OUTPUT:
            sprintf(text_string, "mstp:%d-mso-%d", 
                            addr_info->list.bacnet_list->address.unit_addr, 
                            addr_info->list.bacnet_list->address.reg_addr + reg_offset);
            break;
    case OBJECT_MULTI_STATE_VALUE:
            sprintf(text_string, "mstp %d-msv-%d", 
                            addr_info->list.bacnet_list->address.unit_addr, 
                            addr_info->list.bacnet_list->address.reg_addr + reg_offset);
            break;
        default:
            break;
        }
    }
   
  return text_string;
}

bool BACnet_AvPresentValueSet(uint32_t object_instance, float value, uint8_t priority)
{
    bool ret_value= false;
    uint16_t reg_offset, length;
    uint16_t list_addr;
    ADDRESS_INFO *addr_info;
    
    list_addr= BACnet_AvInstanceToListAddr(object_instance);
    addr_info= AddrInfo_ListAddr(list_addr);
    
        printf("[%s:%s:%d] addr_info->status.online = %d  \n", 
                  __FILE__, __FUNCTION__, __LINE__, addr_info->status.online);
    if(Valid_AddrInfo(addr_info) && addr_info && addr_info->status.online)
    {
        reg_offset= list_addr - addr_info->list_addr;
        
        if(addr_info->set_var_ptr)
        {
            if(addr_info->protocol== PROTOCOL_MODBUS  
                && reg_offset < addr_info->list.modbus_list->address.reg_pts)
            {
                printf("[%s:%s:%d] addr_info->list.modbus_list->address.reg_type = %d  \n", 
                          __FILE__, __FUNCTION__, __LINE__, addr_info->list.modbus_list->address.reg_type);
                printf("[%s:%s:%d] addr_info->list.modbus_list->address.reg_type = %d  \n", 
                          __FILE__, __FUNCTION__, __LINE__, addr_info->list.modbus_list->address.reg_type);
                if(addr_info->list.modbus_list->address.reg_type == HREG_TYPE
                        || addr_info->list.modbus_list->address.reg_type == OCOIL_TYPE)
                {
                    length= SetRealValueToInt16(addr_info->set_var_ptr + reg_offset, value, 
                                                                    addr_info->list.modbus_list->value_type,
                                                                    addr_info->list.modbus_list->value32_be);
                    printf("[%s:%s:%d] value = %f  \n", __FILE__, __FUNCTION__, __LINE__, value);
                    // Fixme
                    length= SetRealValueToInt16(addr_info->get_var_ptr + reg_offset, value, 
                                                                    addr_info->list.modbus_list->value_type,
                                                                    addr_info->list.modbus_list->value32_be);
                    
                    SetVarChgFlag(list_addr, length);
                    
                    ret_value= true;
                    printf("[%s:%s:%d] ret_value = %s  \n", 
                              __FILE__, __FUNCTION__, __LINE__, ret_value ? "TRUE" : "FALSE");
                }
            }
            else
            {
                printf("[%s:%s:%d] addr_info->list.bacnet_list->address.reg_type = %-2x  \n", 
                          __FILE__, __FUNCTION__, __LINE__, addr_info->list.bacnet_list->address.reg_type);
                if(addr_info->list.bacnet_list->address.reg_type != OBJECT_ANALOG_INPUT
                && addr_info->list.bacnet_list->address.reg_type != OBJECT_BINARY_INPUT
                && addr_info->list.bacnet_list->address.reg_type != OBJECT_MULTI_STATE_INPUT)
                {
                    length= SetRealValue_BACnet(addr_info->set_var_ptr + reg_offset, value, 
                                                                    (BACNET_OBJECT_TYPE)addr_info->list.bacnet_list->address.reg_type);
                    SetVarChgFlag(list_addr, length);
                    
                    ret_value= true;
                    printf("[%s:%s:%d] ret_value = %s  \n", 
                              __FILE__, __FUNCTION__, __LINE__, ret_value ? "TRUE" : "FALSE");
                }
            }
        }
    }
        printf("[%s:%s:%d] ret_value = %s  \n", 
                  __FILE__, __FUNCTION__, __LINE__, ret_value ? "TRUE" : "FALSE");
    return ret_value;
}

//////////////////////////////////////////////////////////////////////////////////
uint16_t GetInputValue(uint16_t reg_offset)
{
    if(reg_offset < VAR_LENGTH)
    {
        return GetVarList[reg_offset];
    }
    return 0;
}

bool SetInputValue(uint16_t reg_offset, uint16_t value)
{
    if(reg_offset < VAR_LENGTH)
    {
        GetVarList[reg_offset]= value;
        return true;
    }
    return false;
}

///////////
uint16_t GetOutputValue(uint16_t reg_offset)
{
    if(reg_offset < VAR_LENGTH)
    {
        return SetVarList[reg_offset];
    }
    return 0;
}

bool SetOutputValue(uint16_t reg_offset, uint16_t value)
{
    if(reg_offset < VAR_LENGTH)
    {
        if(GetVarList[reg_offset]!= value)    
        {
            SetVarList[reg_offset]= value;
            SetVarChgFlag(reg_offset, 1);
        }
        return true;
    }
    return false;
}

///////////
float GetCommSuccessRate(ADDR_STATUS *status)
{
    float ret_value= 0.0f;
    
    if(status->request_times!= 0)
    {
        ret_value= status->success_times;
        ret_value*= 100;
        ret_value/= status->request_times;
    }
    
    return ret_value;
}


