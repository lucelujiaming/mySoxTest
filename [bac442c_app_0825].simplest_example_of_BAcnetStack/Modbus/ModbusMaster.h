#ifndef __CMB_MASTER_H__
#define __CMB_MASTER_H__

#include "Modbus.h"
#include "AddrList.h" 
#include "Timer.h"

typedef struct 
{
    ADDRESS_INFO *addr_info;

    uint16_t reg_offset;
    uint8_t reg_pts;
    uint8_t fc_code;
    
    uint8_t cmd_cnt;
}MB_CMD_BLK;    //8 

typedef struct
{
    MB_TYPE bus;
    uint8_t pdu[256];
    ADDRESS_INFO *addr_info;
    volatile uint8_t expect_ack;
    volatile uint8_t write_to_device;
    
    volatile uint16_t pdu_send_size;
    volatile uint16_t pdu_recieve_size;
    
    MTIMER tm_interval;
    
    MB_CMD_BLK read_blk;
    MB_CMD_BLK write_blk;

    uint16_t sec_to_refind;
}MB_MASTER_DATA;

extern MB_MASTER_DATA MB_Master[2];

//define function 
void ModbusMaster_Init(MB_MASTER_DATA *master,
                                                MB_TYPE bus,
                                                ADDRESS_INFO *addr_info, 
                                                uint16_t sec_refind, 
                                                uint32_t interval_ms);


bool ModbusMaster_FrameAnalysis(MB_MASTER_DATA *master);
void master_ack_to_proc(MB_MASTER_DATA *master);
void master_proc(MB_MASTER_DATA *master);
uint16_t master_write_frame_construct(MB_MASTER_DATA *master);

void ModbusMaster_RefindTimer(MB_MASTER_DATA *master);
void ModbusMasterTask(MB_MASTER_DATA *master);
#endif



