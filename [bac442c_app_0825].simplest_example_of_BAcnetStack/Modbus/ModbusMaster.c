#include <string.h>
#include "Common.h"
#include "ModbusMaster.h"
#include "Modbus.h"
#include "mstp_modbus_usart2.h"
#include "mstp_modbus_usart3.h"
#include "AddrList.h"

//
MB_MASTER_DATA MB_Master[2];

//
void ModbusMaster_Init(MB_MASTER_DATA *master,
                                                MB_TYPE bus,
                                                ADDRESS_INFO *addr_info, 
                                                uint16_t sec_refind, 
                                                uint32_t interval_ms)
{    
    master->bus= bus;
    master->addr_info= addr_info;
    master->expect_ack= 0;
    master->write_to_device= 0;
    
    master->pdu_send_size= 0;
    master->pdu_recieve_size= 0;
    
    memset((void*)&master->tm_interval,0,sizeof(MTIMER));
    
    memset((void*)&master->read_blk, 0, sizeof(MB_CMD_BLK));
    memset((void*)&master->write_blk, 0, sizeof(MB_CMD_BLK));

    master->read_blk.addr_info= addr_info;
    master->write_blk.addr_info= addr_info;
    
    master->sec_to_refind= 300;
    if(sec_refind)
    {
        master->sec_to_refind= sec_refind;
    }
    
    if(addr_info)
    {
        if(interval_ms== 0)
        {
            interval_ms= 20;
        }
    
        Timer_SetParam((MTIMER*)&master->tm_interval, true, interval_ms);
        Timer_Init((MTIMER*)&master->tm_interval);
        Timer_Start((MTIMER*)&master->tm_interval);
    }
}
/////////////////////////////////////////
uint16_t master_get_var_changed(MB_MASTER_DATA *master)
{
    ADDRESS_INFO *node;
    REG_TYPE reg_type;
    
    if(!master->write_blk.addr_info)
        return 0;
    
    node= master->write_blk.addr_info;
    
    do
    {
        reg_type= (REG_TYPE)node->list.modbus_list->address.reg_type;
        if(AddrTbl_Online(node) &&  (reg_type== HREG_TYPE || reg_type== OCOIL_TYPE))
        {
            master->write_blk.reg_pts= SetVarChanged(node, &master->write_blk.reg_offset);
            if(master->write_blk.reg_pts)
            {
                master->write_blk.addr_info= node;
                return master->write_blk.reg_pts;
            }
        }
        node= node->next;
        master->write_blk.reg_offset= 0;
        master->write_blk.reg_pts= 0;
        
    }while(node!= master->write_blk.addr_info);
    
    return 0;
}

uint16_t master_read_frame_construct(MB_MASTER_DATA *master)
{
    REG_TYPE reg_type;
    uint16_t max_points;
    uint16_t reg_addr, reg_pts= 0;
    uint16_t frm_size= 0;
    MODBUS_LIST_ADDRESS* modbus_addr;
    MB_CMD_BLK *rd_cmd;
    
    rd_cmd= &master->read_blk;
    modbus_addr= rd_cmd->addr_info->list.modbus_list;
    
    if(rd_cmd->addr_info== NULL)
    {
        return 0;
    }
    
    if(rd_cmd->reg_offset > modbus_addr->address.reg_pts)
    {
        return 0;
    }
        
    reg_type= (REG_TYPE)modbus_addr->address.reg_type;
    switch(reg_type)
    {  
    case OCOIL_TYPE:
        rd_cmd->fc_code= CMD_READ_OUTPUT_COIL;
        max_points= 1000;
        break;
    case ICOIL_TYPE:
        rd_cmd->fc_code= CMD_READ_INPUT_COIL;
        max_points= 1000;
        break;
    case IREG_TYPE:
        rd_cmd->fc_code= CMD_READ_INPUT_REGISTER;
        max_points= 120;
        break;
    case HREG_TYPE:
        rd_cmd->fc_code= CMD_READ_HOLDING_REGISTER;
        max_points= 120;
        break;
    default:
        max_points= 0;
        break;
    }
    
    if(max_points== 0)
    {
        return 0;
    }
    
    reg_addr= modbus_addr->address.reg_addr + rd_cmd->reg_offset - 1;
    
    reg_pts= modbus_addr->address.reg_pts - rd_cmd->reg_offset;
    if(reg_pts > max_points)
    {
        reg_pts= max_points;
    }
    rd_cmd->reg_pts = reg_pts;
    
    master->pdu[0]= modbus_addr->address.unit_addr;
    master->pdu[1]= rd_cmd->fc_code;
    master->pdu[2]= _high_byte_int16(reg_addr);
    master->pdu[3]= _low_byte_int16(reg_addr);
    master->pdu[4]= _high_byte_int16(reg_pts);
    master->pdu[5]= _low_byte_int16(reg_pts);
        
    frm_size= 6;    
    
    return frm_size;
}

uint16_t master_write_frame_construct(MB_MASTER_DATA *master)
{
    REG_TYPE reg_type;
    uint16_t max_points, i;
    uint16_t reg_addr, reg_value;
    uint16_t frm_size= 0;
    MODBUS_LIST_ADDRESS* modbus_addr;
    MB_CMD_BLK *wr_cmd;
    
    wr_cmd= &master->write_blk;
    
    if(wr_cmd->addr_info== NULL)
    {
        return 0;
    }
    
    if(wr_cmd->addr_info->set_var_ptr== NULL)
    {
        return 0;
    }
    
    modbus_addr= wr_cmd->addr_info->list.modbus_list;
    
    if(wr_cmd->reg_offset + wr_cmd->reg_pts > modbus_addr->address.reg_pts || wr_cmd->reg_pts== 0)
    {
        return 0;
    }
    
    reg_type= (REG_TYPE)modbus_addr->address.reg_type;
    switch(reg_type)
    {  
        case OCOIL_TYPE:
            max_points= 1000;
            break;
        case HREG_TYPE:
            max_points= 120;
            break;
        default:
            max_points= 0;
            break;
    }
    
    if(max_points== 0)
    {
        return 0;
    }
    
    reg_addr= modbus_addr->address.reg_addr + wr_cmd->reg_offset - 1;
    
    if(wr_cmd->reg_pts > max_points)
    {
        wr_cmd->reg_pts= max_points;
    }
    
    if(reg_type== HREG_TYPE)
    {
        if(wr_cmd->addr_info->status.multi_set_disable== 0)
        {
            wr_cmd->fc_code= CMD_PRESET_MULTIPLE_REGISTERS;
        }
        else
        {
            wr_cmd->fc_code= CMD_PRESET_SINGLE_REGISTER;
            wr_cmd->reg_pts= 1;
        }
    }
    else
    {
        if(reg_type== OCOIL_TYPE)
        {
            if(wr_cmd->addr_info->status.multi_set_disable== 0)
            {
                wr_cmd->fc_code= CMD_FORCE_MULTIPLE_COILS;
            }
            else
            {
                wr_cmd->fc_code= CMD_FORCE_SINGLE_COIL;
                wr_cmd->reg_pts= 1;
            }
        }
    }
    
    master->pdu[0]= modbus_addr->address.unit_addr;
    master->pdu[1]= wr_cmd->fc_code;
    master->pdu[2]= _high_byte_int16(reg_addr);
    master->pdu[3]= _low_byte_int16(reg_addr);
        
    switch(wr_cmd->fc_code)
    {
        case CMD_FORCE_SINGLE_COIL:
            if(wr_cmd->addr_info->set_var_ptr[wr_cmd->reg_offset])
            {
                master->pdu[4]= 0xFF;
                master->pdu[5]= 0x00;
            }
            else
            {
                master->pdu[4]= 0x00;
                master->pdu[5]= 0x00;
            }
            frm_size= 6;
            break;
        case CMD_FORCE_MULTIPLE_COILS:
            master->pdu[4]= _high_byte_int16(wr_cmd->reg_pts);
            master->pdu[5]= _low_byte_int16(wr_cmd->reg_pts);
            master->pdu[6]= (wr_cmd->reg_pts + 7)/8;

            for(i=0;i< wr_cmd->reg_pts;i++)
            {
                if(wr_cmd->addr_info->set_var_ptr[wr_cmd->reg_offset + i])
                {
                    master->pdu[7 + i / 8]|= 1<<(i % 8);
                }
                else
                {
                    master->pdu[7 + i / 8]&= ~(1<<(i % 8));
                }
            }
                
            frm_size= master->pdu[6] + 7;
            break;
        case CMD_PRESET_SINGLE_REGISTER:
            reg_value= *((uint16_t*)(wr_cmd->addr_info->set_var_ptr + wr_cmd->reg_offset));

            master->pdu[4]= _high_byte_int16(reg_value);
            master->pdu[5]= _low_byte_int16(reg_value);
            frm_size= 6;
            break;
        case CMD_PRESET_MULTIPLE_REGISTERS:
            master->pdu[4]= _high_byte_int16(wr_cmd->reg_pts);
            master->pdu[5]= _low_byte_int16(wr_cmd->reg_pts);
            master->pdu[6]= wr_cmd->reg_pts*2;

            for(i=0;i< wr_cmd->reg_pts;i++)
            {
                reg_value= *((uint16_t*)(wr_cmd->addr_info->set_var_ptr + wr_cmd->reg_offset + i));
                
                master->pdu[7+i*2]= _high_byte_int16(reg_value);
                master->pdu[8+i*2]= _low_byte_int16(reg_value);
            }
                
            frm_size= 7 + master->pdu[6];
            break;
        default:
            break;
    }
    
    master->pdu_send_size= frm_size;
    
    return frm_size;
}


// Check read or write function response over time
// If  retry times is more than 3, return the DEVIDE ADDRESS
void master_error_proc(MB_MASTER_DATA *master)
{
    MB_CMD_BLK *mb_cmd;
    
    if(master->write_to_device)
    {
        mb_cmd= &master->write_blk;
    }
    else
    {
        mb_cmd= &master->read_blk;
    }
    
    if(mb_cmd->addr_info== NULL)
        return;
    
    if(mb_cmd->addr_info->status.repeat_times)
    {
        if(--mb_cmd->addr_info->status.repeat_times== 0)
        {
            if(mb_cmd->fc_code== CMD_PRESET_MULTIPLE_REGISTERS 
                || mb_cmd->fc_code== CMD_FORCE_MULTIPLE_COILS)
            {
                mb_cmd->addr_info->status.multi_set_disable= 1;
            }
            else
            {
                if((mb_cmd->fc_code== CMD_PRESET_SINGLE_REGISTER 
                    || mb_cmd->fc_code== CMD_FORCE_SINGLE_COIL)
                    && mb_cmd->addr_info->status.multi_set_disable)
                {
                    mb_cmd->addr_info->status.multi_set_disable= 0;
                }
            }
            //
            if(mb_cmd->addr_info->status.retry_times)
            {
                if(--mb_cmd->addr_info->status.retry_times)
                {
                    mb_cmd->addr_info->status.repeat_times= 3;
                }
                else
                {
                    mb_cmd->addr_info->status.online= 0;    
                    mb_cmd->addr_info->status.sec_to_refind= master->sec_to_refind;
                }
            }
            
            mb_cmd->addr_info= NextValidAddrInfo(master->addr_info, mb_cmd->addr_info);
            mb_cmd->reg_offset= 0;
            mb_cmd->reg_pts= 0;
            mb_cmd->fc_code= 0;
        }
            
        if(!master->write_to_device)
        {
            if(++master->read_blk.cmd_cnt>= 3)
            {
                master->read_blk.cmd_cnt= 0;
                master->write_to_device= 1;
            }
        }
        else
        {
            if(++master->write_blk.cmd_cnt>= 9)
            {
                master->write_blk.cmd_cnt= 0;
                master->write_to_device= 0;
            }
        }
    }
}

// Check read or write function response over time
// If  retry times is more than 3, return the DEVIDE ADDRESS
void master_ack_to_proc(MB_MASTER_DATA *master)
{
    master_error_proc(master);
    
    master->tm_interval.ticks_value= master->tm_interval.ticks_limit - 10;
    Timer_Start((MTIMER*)&master->tm_interval);
}
//////////////////////////////////////////////////////////////////////////
bool ModbusMaster_FrameAnalysis(MB_MASTER_DATA *master)
{
    uint16_t i;
    bool ack_ok= false;
    uint16_t reg_addr;
    MB_CMD_BLK *mb_cmd;
    
    if(master->pdu_recieve_size)
    {
        if(master->write_to_device)
        {
            mb_cmd= &master->write_blk;
        }
        else
        {
            mb_cmd= &master->read_blk;
        }
        
        if(master->pdu[0]== mb_cmd->addr_info->list.modbus_list->address.unit_addr || master->pdu[0]== MODBUS_BROADCAST_ADDR)
        {
            if(mb_cmd->fc_code == master->pdu[1])
            {
                switch(master->pdu[1])
                {
                case CMD_READ_OUTPUT_COIL:
                case CMD_READ_INPUT_COIL:
                    if(mb_cmd->reg_pts<= (uint16_t)master->pdu[2]*8)
                    {
                        for(i= 0;i< mb_cmd->reg_pts;i++)
                        {
                            if(master->pdu[3 + i / 8] & (1<<(i % 8)))
                            {
                                mb_cmd->addr_info->get_var_ptr[mb_cmd->reg_offset + i] = 1;
                            }
                            else
                            {
                                mb_cmd->addr_info->get_var_ptr[mb_cmd->reg_offset + i] = 0;
                            }
                        }
                        ack_ok= true;
                    }
                    break;
                case CMD_READ_INPUT_REGISTER:
                case CMD_READ_HOLDING_REGISTER:
                    for(i= 0;i<(master->pdu[2]/2);i++)
                    {
                        mb_cmd->addr_info->get_var_ptr[mb_cmd->reg_offset + i]= _get_int16_int8_big_endian(master->pdu + 3 + 2 * i);
                    }
                    
                    ack_ok= true;
                    break;
                case CMD_FORCE_SINGLE_COIL:
                case CMD_FORCE_MULTIPLE_COILS:
                case CMD_PRESET_SINGLE_REGISTER:
                case CMD_PRESET_MULTIPLE_REGISTERS:
                    reg_addr= mb_cmd->addr_info->list.modbus_list->address.reg_addr + mb_cmd->reg_offset -1;
                    if(_get_int16_int8_big_endian(master->pdu + 2)== reg_addr)    //Check data
                    {
                        ClrVarChgFlag(mb_cmd->addr_info->list_addr + mb_cmd->reg_offset, mb_cmd->reg_pts);
                        ack_ok= true;
                    }
                    break;
                default:
                    break;
                }
            }
            else
            {
                if((mb_cmd->fc_code | 0x80) == master->pdu[1])
                {
                }
            }
        }
    }
    
    if(ack_ok)
    {
        mb_cmd->addr_info->status.success_times++;
        
        mb_cmd->addr_info->status.repeat_times= 3;
        mb_cmd->addr_info->status.retry_times= 3;
        mb_cmd->addr_info->status.online= 1;
        
        mb_cmd->reg_offset= mb_cmd->reg_pts + mb_cmd->reg_offset;
        
        if(mb_cmd->reg_offset >= mb_cmd->addr_info->list.modbus_list->address.reg_pts)
        {
            mb_cmd->addr_info= NextValidAddrInfo(master->addr_info, mb_cmd->addr_info);
            mb_cmd->reg_offset= 0;
            mb_cmd->reg_pts= 0;
            mb_cmd->fc_code= 0; 
        }
        
        if(!master->write_to_device)
        {
            if(++master->read_blk.cmd_cnt>= 3)
            {
                master->read_blk.cmd_cnt= 0;
                master->write_to_device= 1;
            }
        }
        else
        {
            if(++master->write_blk.cmd_cnt>= 9)
            {
                master->write_blk.cmd_cnt= 0;
                master->write_to_device= 0;
            }
        }
    }
    else
    {
        master_error_proc(master);
    }
    
    Timer_Restart((MTIMER*)&master->tm_interval);
    
    return ack_ok;
}



///////////////////////////////////////////////////////////////////////////////////////
void ModbusMaster_RefindTimer(MB_MASTER_DATA *master)
{
    ADDRESS_INFO *node= master->addr_info;
    
    if(master->addr_info)
    {
        node= master->addr_info;
        do{
            if(node->status.sec_to_refind)
            {
                if(-- node->status.sec_to_refind== 0)
                {
                    node->status.retry_times= 1;
                    node->status.repeat_times= 3; 
                }
            }
            node= node->next;
        }while(node!= master->addr_info);
    }
}

void master_proc(MB_MASTER_DATA *master)
{
    uint16_t reg_pts;
    
    if((master->addr_info->list.modbus_list->com_port== 1 && USART2_Ack_Timeout())
        || (master->addr_info->list.modbus_list->com_port== 2 && USART3_Ack_Timeout()))
    {
        master_ack_to_proc(master);
    }
    else
    {
        if(Timer_Expires((MTIMER*)&master->tm_interval))
        {
            master->pdu_send_size= 0;
            
            if(master->write_to_device)
            {
                if(master->write_blk.addr_info== NULL)
                {
                    master->write_blk.addr_info= master->addr_info;
                }
                    
                if(master->write_blk.addr_info)
                {
                    reg_pts= master_get_var_changed(master);
                    
                    if(reg_pts)
                    {
                        master_write_frame_construct(master);
                    }
                }
                
                if(master->pdu_send_size)
                {
                    master->write_blk.addr_info->status.request_times++;
                }
                else
                {
                    master->write_to_device= false;
                    master->write_blk.cmd_cnt= 0;
                }
            }
            else
            {
                if(master->read_blk.addr_info== NULL || master->read_blk.addr_info->status.sec_to_refind) 
                {
                    master->read_blk.addr_info= NextValidAddrInfo(master->addr_info, NULL);
                }
                
                if(master->read_blk.addr_info)
                {
                    master->pdu_send_size= master_read_frame_construct(master);
                    if(master->pdu_send_size)
                    {
                        master->read_blk.addr_info->status.request_times++;
                    }
                }
            }
        }
    }
}

void ModbusMasterTask(MB_MASTER_DATA *master)
{
    uint16_t frame_length;
    uint8_t *usart_buff;
    
    if(master->addr_info->list.modbus_list->com_port== 1)
    {
        //Modbus Master Task
        usart_buff= USART2_MODBUS_GetBuf();

        if(master->pdu_send_size)
        {
            frame_length= mb_frame_npdu_encode(MB_RTU, usart_buff, master->pdu, master->pdu_send_size, 0);
            master->pdu_send_size= 0;
            
            if(frame_length)
            {
                USART2_SendFrame(frame_length);    
                Timer_Stop(&master->tm_interval);
            }
        }
    
        if(USART2_MODBUS_FrameReceived())    
        {
            frame_length= USART2_MODBUS_GetFrameLength();
            master->pdu_recieve_size= mb_frame_npdu_decode(MB_RTU, master->pdu, usart_buff, frame_length, NULL);
            ModbusMaster_FrameAnalysis(master);
        } 
    }
    else
    {
        if(master->addr_info->list.modbus_list->com_port== 2)
        {
                //Modbus Master Task
            usart_buff= USART3_MODBUS_GetBuf();

            if(master->pdu_send_size)
            {
                frame_length= mb_frame_npdu_encode(MB_RTU, usart_buff, master->pdu, master->pdu_send_size, 0);
                master->pdu_send_size= 0;
                
                if(frame_length)
                {
                    USART3_SendFrame(frame_length);    
                    Timer_Stop(&master->tm_interval);
                }
            }
        
            if(USART3_MODBUS_FrameReceived())    
            {
                frame_length= USART3_MODBUS_GetFrameLength();
                master->pdu_recieve_size= mb_frame_npdu_decode(MB_RTU, master->pdu, usart_buff, frame_length, NULL);
                ModbusMaster_FrameAnalysis(master);
            }
        }
    }
    
    master_proc(master);
}


