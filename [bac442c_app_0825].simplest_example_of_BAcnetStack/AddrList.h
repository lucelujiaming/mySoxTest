#ifndef __ADDR_LIST_H__
#define __ADDR_LIST_H__

// Unit Addr:Device ID
// Reg Addr:Register Address in Device e.g.:0...9999
// Type Addr:Register Address with Type Base Address e.g.:30001,40001
// List Addr:Register Address in Ram List

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "Modbus.h"
#include "bacenum.h"
#include "modbus_list.h"
#include "bacnet_list.h"

// #define MODBUS_LIST_MAX        200
#define MODBUS_LIST_MAX        5
// #define BACNET_LIST_MAX        200
#define TCP_LIST_MAX    50

// #define ADDR_INFO_LENGTH    MODBUS_LIST_MAX//(MODBUS_LIST_MAX*3 + BACNET_LIST_MAX)
#define ADDR_INFO_LENGTH    10

typedef union
{
    MODBUS_LIST_ADDRESS* modbus_list;
    BACNET_LIST_ADDRESS* bacnet_list;
}REG_LIST;

typedef enum
{
    PROTOCOL_MODBUS= 0,
    PROTOCOL_BACNET= 1
}PROTOCOL_TYPE;

// Define Communication control Block
typedef struct
{
    volatile uint32_t request_times;
    volatile uint32_t success_times;
    
    volatile uint16_t sec_to_refind;    // 重试3次后（9次发送），仍无法解决，设定延时时间，进行重发(秒)。每次延时时间到，retry_times= 1
    volatile uint8_t multi_set_disable;    //
    volatile uint8_t repeat_times;        // 如果出现超时或通信错误，连续发送次数（3）
    volatile uint8_t retry_times;        // 如果重复发送无法解决，将在下一个指令循环时重试（3）
    volatile uint8_t online;
}ADDR_STATUS;    //5

typedef struct _ADDRESS_INFO
{
    PROTOCOL_TYPE protocol;
    uint16_t idx;
    uint16_t list_addr;
    uint16_t av_idx;
    uint16_t *get_var_ptr;    //由底部向上生长
    uint16_t *set_var_ptr;    //由顶部向下生长，当二者之间的间隔不足100 word时，认为已经满了
    REG_LIST list;
    ADDR_STATUS status;
    
    struct _ADDRESS_INFO* next;
}ADDRESS_INFO;    //25


#define VAR_LENGTH    5000

extern MODBUS_LIST_ADDRESS* ModbusList;        //ExSram
extern ADDRESS_INFO* AddrInfoList;                //ExSram
extern ADDRESS_INFO* AddrInfoChain_Usart[2];
/**Define Device List**/
////////////////////////////////////////////////////////////////////////////////
//List Addr:在长数据表中的索引号
REG_TYPE RegType_TypeAddr(uint32_t addr);
uint16_t RegAddr_TypeAddr(uint32_t addr);
uint32_t TypeAddr_RegAddr(uint16_t addr, REG_TYPE type);
REG_TYPE RegType_AddrTbl(REG_ADDRESS* addr_tbl);

bool IsWriteCmd(uint8_t cmd);
bool IsCoilType(REG_TYPE reg_type);
//////////////////////////////////////////////////////////////
//地址表有效，返回true
bool Valid_DeviceAddr(uint8_t addr);
bool Valid_TypeAddr(uint32_t type_addr);
bool Valid_AddrTbl(REG_ADDRESS* addr_tbl);
bool Valid_AddrInfo(ADDRESS_INFO *addr_info);

bool AddrTbl_Online(ADDRESS_INFO *addr_info);

bool DeviceInAddrTbl(uint16_t unit_addr, uint16_t reg_addr, uint8_t reg_type, REG_ADDRESS* addr_tble);
ADDRESS_INFO* AddrInfo_ListAddr(uint16_t lst_addr);
ADDRESS_INFO *NextValidAddrInfo(ADDRESS_INFO* head, ADDRESS_INFO *cur_node);
void Init_AddrInfoList(void);

uint32_t UnitAddr_ListAddr(uint16_t lst_addr);
bool Valid_ListAddr(uint16_t lst_addr);
void SetVarChgFlag(uint16_t  lst_addr, uint16_t points);
void ClrVarChgFlag(uint16_t  lst_addr, uint16_t points);
bool SetVarIsModified(uint16_t lst_addr);
uint16_t SetVarChanged(ADDRESS_INFO *addr_info, uint16_t *reg_offset);
bool IsWritable_ListAddr(uint16_t lst_addr);

void var_eep_init(void);
void var_eep_read_block(void *var_buf, uint16_t eep_offset, uint16_t var_length);
void var_eep_write_block(void *var_buf, uint16_t eep_addr, uint16_t var_length);
void var_eep_write_word(uint16_t eep_offset, uint16_t reg_value);
void var_eep_read_param(void);

void CheckAddrInfoListOnLine(void);
//
uint16_t GetInputValue(uint16_t reg_offset);
bool SetInputValue(uint16_t reg_offset, uint16_t value);

uint16_t GetOutputValue(uint16_t reg_offset);
bool SetOutputValue(uint16_t reg_offset, uint16_t value);

uint16_t GetAddrListValue(uint16_t reg_offset);
bool SetAddrListValue(uint16_t reg_offset, uint16_t value);
uint16_t* GetAddrListPtr(uint16_t reg_offset);

uint16_t GetDvcListValue(uint16_t reg_offset);
bool SetDvcListValue(uint16_t reg_offset, uint16_t value);
uint16_t* GetDvcListPtr(uint16_t reg_offset);

bool VarIsEepSaved(uint16_t reg_addr);
//
uint16_t BACnet_AvCount(void);
uint16_t BACnet_BvCount(void);
bool BACnet_AvPresentValueSet(uint32_t object_instance, float value, uint8_t priority);
float BACnet_AvPresentValue(uint32_t object_instance);
BACNET_BINARY_PV BACnet_BvPresentValue(uint32_t object_instance);
char *BACnetAnalogValueName(uint32_t object_instance);
char *BACnetAnalogValueDescription(uint32_t object_instance);

float GetCommSuccessRate(ADDR_STATUS *status);
extern ADDRESS_INFO* AddrInfoChain;

#endif
