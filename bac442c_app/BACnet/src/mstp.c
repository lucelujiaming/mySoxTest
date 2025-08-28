/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2003-2007 Steve Karg

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to:
 The Free Software Foundation, Inc.
 59 Temple Place - Suite 330
 Boston, MA  02111-1307
 USA.

 As a special exception, if other files instantiate templates or
 use macros or  functions from this file, or you compile
 this file and link it with other works to produce a work based
 on this file, this file does not by itself cause the resulting
 work to be covered by the GNU General Public License. However
 the source code for this file must still be made available in
 accordance with section (3) of the GNU General Public License.

 This exception does not invalidate any other reasons why a work
 based on this file might be covered by the GNU General Public
 License.
 -------------------------------------------
####COPYRIGHTEND####*/

/** @file mstp.c  BACnet Master-Slave Twisted Pair (MS/TP) functions */

/* This clause describes a Master-Slave/Token-Passing (MS/TP) data link  */
/* protocol, which provides the same services to the network layer as  */
/* ISO 8802-2 Logical Link Control. It uses services provided by the  */
/* EIA-485 physical layer. Relevant clauses of EIA-485 are deemed to be  */
/* included in this standard by reference. The following hardware is assumed: */
/* (a)  A UART (Universal Asynchronous Receiver/Transmitter) capable of */
/*      transmitting and receiving eight data bits with one stop bit  */
/*      and no parity. */
/* (b)  An EIA-485 transceiver whose driver may be disabled.  */
/* (c)  A timer with a resolution of five milliseconds or less */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "mstp.h"
#include "txbuf.h"
#include "crc.h"
#include "silence_timer.h"
#include "hal_usart1_dma.h"

extern uint8_t MSTP_InputBuf[MAX_MPDU];
extern uint8_t MSTP_OutputBuf[MAX_MPDU];

extern bool dlmstp_compare_data_expecting_reply(
    uint8_t * request_pdu,
    uint16_t request_pdu_len,
    uint8_t src_address,
    uint8_t * reply_pdu,
    uint16_t reply_pdu_len,
    uint8_t dest_address);
/* MS/TP Frame Format */
/* All frames are of the following format: */
/* */
/* Preamble: two octet preamble: X`55', X`FF' */
/* Frame Type: one octet */
/* Destination Address: one octet address */
/* Source Address: one octet address */
/* Length: two octets, most significant octet first, of the Data field */
/* Header CRC: one octet */
/* Data: (present only if Length is non-zero) */
/* Data CRC: (present only if Length is non-zero) two octets, */
/*           least significant octet first */
/* (pad): (optional) at most one octet of padding: X'FF' */

/* The number of tokens received or used before a Poll For Master cycle */
/* is executed: 50. */
//#define Npoll 50
#define Npoll 100

/* The number of retries on sending Token: 1. */
#define Nretry_token 1

/* The minimum number of DataAvailable or ReceiveError events that must be */
/* seen by a receiving node in order to declare the line "active": 4. */
#define Nmin_octets 4

/* The minimum time without a DataAvailable or ReceiveError event within */
/* a frame before a receiving node may discard the frame: 60 bit times. */
/* (Implementations may use larger values for this timeout, */
/* not to exceed 100 milliseconds.) */
/* At 9600 baud, 60 bit times would be about 6.25 milliseconds */
/* const uint16_t Tframe_abort = 1 + ((1000 * 60) / 9600); */
#define Tframe_abort 95

/* The maximum idle time a sending node may allow to elapse between octets */
/* of a frame the node is transmitting: 20 bit times. */
#define Tframe_gap 20

/* The maximum time after the end of the stop bit of the final */
/* octet of a transmitted frame before a node must disable its */
/* EIA-485 driver: 15 bit times. */
#define Tpostdrive 15

/* The maximum time a node may wait after reception of a frame that expects */
/* a reply before sending the first octet of a reply or Reply Postponed */
/* frame: 250 milliseconds. */
/* note: we always send a reply postponed since a message other than
   the reply may be in the transmit queue */
#define Treply_delay 10        

/* Repeater turnoff delay. The duration of a continuous logical one state */
/* at the active input port of an MS/TP repeater after which the repeater */
/* will enter the IDLE state: 29 bit times < Troff < 40 bit times. */
#define Troff 30

/* The width of the time slot within which a node may generate a token: */
/* 10 milliseconds. */
#define Tslot 10

/* The maximum time a node may wait after reception of the token or */
/* a Poll For Master frame before sending the first octet of a frame: */
/* 15 milliseconds. */
#define Tusage_delay 15

/* The minimum time without a DataAvailable or ReceiveError event */
/* that a node must wait for a station to begin replying to a */
/* confirmed request: 255 milliseconds. (Implementations may use */
/* larger values for this timeout, not to exceed 300 milliseconds.) */
#define Treply_timeout 260

/* The minimum time without a DataAvailable or ReceiveError event that a */
/* node must wait for a remote node to begin using a token or replying to */
/* a Poll For Master frame: 20 milliseconds. (Implementations may use */
/* larger values for this timeout, not to exceed 100 milliseconds.) */
#define Tusage_timeout 25

/* we need to be able to increment without rolling over */
#define INCREMENT_AND_LIMIT_UINT8(x) {if (x < 0xFF) x++;}

bool MSTP_Line_Active(volatile struct mstp_port_struct_t *mstp_port)
{
    return (mstp_port->EventCount > Nmin_octets);
}


uint16_t MSTP_Create_Frame(
    uint8_t * buffer,   /* where frame is loaded */
    uint16_t buffer_len,        /* amount of space available */
    uint8_t frame_type, /* type of frame to send - see defines */
    uint8_t destination,        /* destination address */
    uint8_t source,     /* source address */
    uint8_t * data,     /* any data to be sent - may be null */
    uint16_t data_len)
{       /* number of bytes of data (up to 501) */
    uint8_t crc8 = 0xFF;        /* used to calculate the crc value */
    uint16_t crc16 = 0xFFFF;    /* used to calculate the crc value */
    uint16_t index = 0; /* used to load the data portion of the frame */

    /* not enough to do a header */
    if (buffer_len < 8)
        return 0;

    buffer[0] = 0x55;
    buffer[1] = 0xFF;
    buffer[2] = frame_type;
    crc8 = CRC_Calc_Header(buffer[2], crc8);
    buffer[3] = destination;
    crc8 = CRC_Calc_Header(buffer[3], crc8);
    buffer[4] = source;
    crc8 = CRC_Calc_Header(buffer[4], crc8);
    buffer[5] = data_len >> 8;  /* MSB first */
    crc8 = CRC_Calc_Header(buffer[5], crc8);
    buffer[6] = data_len & 0xFF;
    crc8 = CRC_Calc_Header(buffer[6], crc8);
    buffer[7] = ~crc8;

    index = 8;
    while (data_len && data && (index < buffer_len)) 
    {
        buffer[index] = *data;
        crc16 = CRC_Calc_Data(buffer[index], crc16);
        data++;
        index++;
        data_len--;
    }
    /* append the data CRC if necessary */
    if (index > 8) 
    {
        if ((index + 2) <= buffer_len) 
        {
            crc16 = ~crc16;
            buffer[index] = crc16 & 0xFF;       /* LSB first */
            index++;
            buffer[index] = crc16 >> 8;
            index++;
        } else
            return 0;
    }

    return index;       /* returns the frame length */
}


void MSTP_Create_And_Send_Frame(
    volatile struct mstp_port_struct_t *mstp_port,      /* port to send from */
    uint8_t frame_type, /* type of frame to send - see defines */
    uint8_t destination,        /* destination address */
    uint8_t source,     /* source address */
    uint8_t * data,     /* any data to be sent - may be null */
    uint16_t data_len)
{       /* number of bytes of data (up to 501) */
    uint16_t len = 0;   /* number of bytes to send */

    len = MSTP_Create_Frame((uint8_t *) & mstp_port->OutputBuffer[0], mstp_port->OutputBufferSize, frame_type, destination, source, data, data_len);

    RS485_Send_Frame(len);
    
        mstp_port->SilenceTimerReset();
}


void MSTP_Receive_Frame_FSM(volatile struct mstp_port_struct_t *mstp_port)
{
    static MSTP_RECEIVE_STATE receive_state = MSTP_RECEIVE_STATE_IDLE;
   
    switch (mstp_port->receive_state) 
    {
            /* In the IDLE state, the node waits for the beginning of a frame. */
        case MSTP_RECEIVE_STATE_IDLE:
            /* EatAnError */
            if (mstp_port->ReceiveError == true) 
                        {
                                            mstp_port->ReceiveError = false;
                                            mstp_port->SilenceTimerReset();
                                            INCREMENT_AND_LIMIT_UINT8(mstp_port->EventCount);
                                            /* wait for the start of a frame. */
                        } 
                        else
                        {
                            if (mstp_port->DataAvailable == true) 
                            {
                                /* Preamble1 */
                                if (mstp_port->DataRegister == 0x55) 
                                {
                                    /* receive the remainder of the frame. */
                                    mstp_port->receive_state = MSTP_RECEIVE_STATE_PREAMBLE;
                                }
                                mstp_port->DataAvailable = false;
                                mstp_port->SilenceTimerReset();
                                INCREMENT_AND_LIMIT_UINT8(mstp_port->EventCount);
                            }
                        }
            break;
            /* In the PREAMBLE state, the node waits for the second octet of the preamble. */
        case MSTP_RECEIVE_STATE_PREAMBLE:
            /* Timeout */
            if (mstp_port->SilenceTimer() > Tframe_abort) 
                        {
                                            /* a correct preamble has not been received */
                                            /* wait for the start of a frame. */
                                            mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
                        }
                        /* Error */
            else
                        {
                if (mstp_port->ReceiveError == true) 
                {
                    mstp_port->ReceiveError = false;
                    mstp_port->SilenceTimerReset();
                    INCREMENT_AND_LIMIT_UINT8(mstp_port->EventCount);
                    /* wait for the start of a frame. */
                    mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
                } 
                else
                {
                    if (mstp_port->DataAvailable == true) 
                    {
                        /* Preamble2 */
                        if (mstp_port->DataRegister == 0xFF) 
                        {
                            mstp_port->Index = 0;
                            mstp_port->HeaderCRC = 0xFF;
                            /* receive the remainder of the frame. */
                            mstp_port->receive_state = MSTP_RECEIVE_STATE_HEADER;
                        }
                        /* ignore RepeatedPreamble1 */
                        else
                        {
                            if (mstp_port->DataRegister == 0x55) 
                            {
                                /* wait for the second preamble octet. */
                                mstp_port->receive_state = MSTP_RECEIVE_STATE_PREAMBLE;
                            }
                            /* NotPreamble */
                            else 
                            {
                                /* wait for the start of a frame. */
                                mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
                            }
                        }
                        mstp_port->DataAvailable = false;
                        mstp_port->SilenceTimerReset();
                        INCREMENT_AND_LIMIT_UINT8(mstp_port->EventCount);
                    }
                }
            }
            break;
            /* In the HEADER state, the node waits for the fixed message header. */
        case MSTP_RECEIVE_STATE_HEADER:
            /* Timeout */
            if (mstp_port->SilenceTimer() > Tframe_abort) 
            {
                /* indicate that an error has occurred during the reception of a frame */
                mstp_port->ReceivedInvalidFrame = true;
                /* wait for the start of a frame. */
                mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
            }
            /* Error */
            else
            {
                if (mstp_port->ReceiveError == true) 
                {
                    mstp_port->ReceiveError = false;
                    mstp_port->SilenceTimerReset();
                    INCREMENT_AND_LIMIT_UINT8(mstp_port->EventCount);
                    /* indicate that an error has occurred during the reception of a frame */
                    mstp_port->ReceivedInvalidFrame = true;
                    /* wait for the start of a frame. */
                    mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
                } 
                else
                {
                    if (mstp_port->DataAvailable == true) 
                    {
                        /* FrameType */
                        if (mstp_port->Index == 0) 
                        {
                            mstp_port->HeaderCRC = CRC_Calc_Header(mstp_port->DataRegister, mstp_port->HeaderCRC);
                            mstp_port->FrameType = mstp_port->DataRegister;
                            mstp_port->Index = 1;
                        }
                        /* Destination */
                        else
                        {
                            if (mstp_port->Index == 1) 
                            {
                                mstp_port->HeaderCRC = CRC_Calc_Header(mstp_port->DataRegister, mstp_port->HeaderCRC);
                                mstp_port->DestinationAddress = mstp_port->DataRegister;
                                mstp_port->Index = 2;
                            }
                            /* Source */
                            else
                            {
                                if (mstp_port->Index == 2) 
                                {
                                    mstp_port->HeaderCRC = CRC_Calc_Header(mstp_port->DataRegister, mstp_port->HeaderCRC);
                                    mstp_port->SourceAddress = mstp_port->DataRegister;
                                    mstp_port->Index = 3;
                                }
                                /* Length1 */
                                else
                                {
                                    if (mstp_port->Index == 3) 
                                    {
                                        mstp_port->HeaderCRC = CRC_Calc_Header(mstp_port->DataRegister, mstp_port->HeaderCRC);
                                        mstp_port->DataLength = mstp_port->DataRegister * 256;
                                        mstp_port->Index = 4;
                                    }
                                    /* Length2 */
                                    else
                                    {
                                        if (mstp_port->Index == 4) 
                                        {
                                            mstp_port->HeaderCRC = CRC_Calc_Header(mstp_port->DataRegister,    mstp_port->HeaderCRC);
                                            mstp_port->DataLength += mstp_port->DataRegister;
                                            mstp_port->Index = 5;
                                        }
                                        /* HeaderCRC */
                                        else
                                        {
                                            if (mstp_port->Index == 5) 
                                            {
                                                mstp_port->HeaderCRC = CRC_Calc_Header(mstp_port->DataRegister, mstp_port->HeaderCRC);
                                                mstp_port->HeaderCRCActual = mstp_port->DataRegister;
                                                /* don't wait for next state - do it here */
                                                if (mstp_port->HeaderCRC != 0x55) 
                                                {
                                                    /* BadCRC */
                                                    /* indicate that an error has occurred during
                                                       the reception of a frame */
                                                    mstp_port->ReceivedInvalidFrame = true;
                                                    
                                                    /* wait for the start of the next frame. */
                                                    mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
                                                } 
                                                else 
                                                {
                                                    /* Note: proposed change to BACnet MSTP state machine!
                                                       If we don't decode data that is not for us, we could
                                                       get confused about the start if the Preamble 55 FF
                                                       is part of the data. */
                                                    /* Data */
                                                    if ((mstp_port->DataLength) && 
                                                        (mstp_port->DataLength <= mstp_port->InputBufferSize)) 
                                                    {
                                                        /* Data - decode anyway to keep from false  */
                                                        mstp_port->Index = 0;
                                                        mstp_port->DataCRC = 0xFFFF;
                                                        /* receive the data portion of the frame. */
                                                        mstp_port->receive_state = MSTP_RECEIVE_STATE_DATA;
                                                    } 
                                                    else 
                                                    {
                                                        /* FrameTooLong */
                                                        if (mstp_port->DataLength) 
                                                        {
                                                            /* indicate that a frame with an illegal or  */
                                                            /* unacceptable data length has been received */
                                                            mstp_port->ReceivedInvalidFrame = true;
                                                        }
                                                        /* NoData */
                                                        else
                                                        {
                                                            if (mstp_port->DataLength == 0) 
                                                            {
                                                                if ((mstp_port->DestinationAddress == mstp_port->This_Station)
                                                                    || (mstp_port->DestinationAddress == MSTP_BROADCAST_ADDRESS)
                                                                    || (mstp_port->Lurking)) 
                                                                {
                                                                    /* ForUs */
                                                                    /* indicate that a frame with no data has been received */
                                                                    mstp_port->ReceivedValidFrame = true;
                                                                } 
                                                                else 
                                                                {
                                                                    /* NotForUs - drop */
                                                                }
                                                            }
                                                        }
                                                        /* wait for the start of the next frame. */
                                                        mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
                                                    }
                                                }
                                            }
                                            /* not per MS/TP standard, but it is a case not covered */
                                            else 
                                            {
                                                mstp_port->ReceiveError = false;
                                                /* indicate that an error has occurred during  */
                                                /* the reception of a frame */
                                                mstp_port->ReceivedInvalidFrame = true;
                                                /* wait for the start of a frame. */
                                                mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                                    
                        mstp_port->SilenceTimerReset();
                        INCREMENT_AND_LIMIT_UINT8(mstp_port->EventCount);
                        mstp_port->DataAvailable = false;
                    }
                }
            }
            break;
            /* In the DATA state, the node waits for the data portion of a frame. */
        case MSTP_RECEIVE_STATE_DATA:
            /* Timeout */
            if (mstp_port->SilenceTimer() > Tframe_abort) 
            {
                /* indicate that an error has occurred during the reception of a frame */
                mstp_port->ReceivedInvalidFrame = true;
                
                /* wait for the start of the next frame. */
                mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
            }
            /* Error */
            else
            {
                if (mstp_port->ReceiveError == true) 
                {
                    mstp_port->ReceiveError = false;
                    mstp_port->SilenceTimerReset();
                    /* indicate that an error has occurred during the reception of a frame */
                    mstp_port->ReceivedInvalidFrame = true;
                    
                    /* wait for the start of the next frame. */
                    mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
                } 
                else
                {
                    if (mstp_port->DataAvailable == true) 
                    {
                        if (mstp_port->Index < mstp_port->DataLength) 
                        {
                            /* DataOctet */
                            mstp_port->DataCRC = CRC_Calc_Data(mstp_port->DataRegister, mstp_port->DataCRC);
                            mstp_port->InputBuffer[mstp_port->Index] = mstp_port->DataRegister;
                            mstp_port->Index++;
                            mstp_port->receive_state = MSTP_RECEIVE_STATE_DATA;
                        } 
                        else
                        {
                            if (mstp_port->Index == mstp_port->DataLength) 
                            {
                                /* CRC1 */
                                mstp_port->DataCRC = CRC_Calc_Data(mstp_port->DataRegister, mstp_port->DataCRC);
                                mstp_port->DataCRCActualMSB = mstp_port->DataRegister;
                                mstp_port->Index++;
                                mstp_port->receive_state = MSTP_RECEIVE_STATE_DATA;
                            } 
                            else
                            {
                                if (mstp_port->Index == (mstp_port->DataLength + 1)) 
                                {
                                    /* CRC2 */
                                    mstp_port->DataCRC = CRC_Calc_Data(mstp_port->DataRegister, mstp_port->DataCRC);
                                    mstp_port->DataCRCActualLSB = mstp_port->DataRegister;
                                    /* STATE DATA CRC - no need for new state */
                                    /* indicate the complete reception of a valid frame */
                                    if (mstp_port->DataCRC == 0xF0B8) 
                                    {
                                        if ((mstp_port->DestinationAddress == mstp_port->This_Station)
                                            || (mstp_port->DestinationAddress == MSTP_BROADCAST_ADDRESS)
                                            || (mstp_port->Lurking)) 
                                        {
                                            /* ForUs */
                                            /* indicate that a frame with no data has been received */
                                            mstp_port->ReceivedValidFrame = true;
                                        } 
                                        else 
                                        {
                                            /* NotForUs - drop */
                                        }
                                    } 
                                    else 
                                    {
                                        mstp_port->ReceivedInvalidFrame = true;
                                    }
                                    mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
                                }
                            }
                        }
                        mstp_port->DataAvailable = false;
                        mstp_port->SilenceTimerReset();
                    }
                }
            }
            break;
        default:
            /* shouldn't get here - but if we do... */
            mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
            break;
    }
    if ((receive_state != MSTP_RECEIVE_STATE_IDLE) &&
        (mstp_port->receive_state == MSTP_RECEIVE_STATE_IDLE)) 
    {
        //fflush(stderr);
    }
    receive_state = mstp_port->receive_state;
    return;
}

/* returns true if we need to transition immediately */

bool MSTP_Master_Node_FSM(volatile struct mstp_port_struct_t * mstp_port)
{
//    uint16_t length = 0;
    uint8_t next_poll_station = 0;
    uint8_t next_this_station = 0;
    uint8_t next_next_station = 0;
    uint16_t my_timeout = 10, ns_timeout = 0;
    /* transition immediately to the next state */
    bool transition_now = false;
    static MSTP_MASTER_STATE master_state = MSTP_MASTER_STATE_INITIALIZE;
    // uint8_t destination;
    bool matched= false;
    uint8_t frame_type;
       
    /* some calculations that several states need */
    next_poll_station = (mstp_port->Poll_Station + 1) % (mstp_port->Nmax_master + 1);
    next_this_station = (mstp_port->This_Station + 1) % (mstp_port->Nmax_master + 1);
    next_next_station = (mstp_port->Next_Station + 1) % (mstp_port->Nmax_master + 1);

    if (mstp_port->master_state != master_state) 
    {
        master_state = mstp_port->master_state;
    }
    printf("[%s:%s:%d] mstp_port->master_state = %d \n",
            __FILE__, __FUNCTION__, __LINE__, mstp_port->master_state);

    switch (mstp_port->master_state) 
    {
        case MSTP_MASTER_STATE_INITIALIZE:
            /* DoneInitializing */
            /* indicate that the next station is unknown */
            mstp_port->Next_Station = mstp_port->This_Station;
            mstp_port->Poll_Station = mstp_port->This_Station;
            /* cause a Poll For Master to be sent when this node first */
            /* receives the token */
            mstp_port->TokenCount = Npoll;
            mstp_port->SoleMaster = false;
            mstp_port->master_state = MSTP_MASTER_STATE_IDLE;
            transition_now = true;
            break;
        case MSTP_MASTER_STATE_IDLE:
                printf("[%s:%s:%d] mstp_comm_mode == MSTP_MODE_MASTER \n",
                        __FILE__, __FUNCTION__, __LINE__);
            /* In the IDLE state, the node waits for a frame. */
            /* LostToken */
            if (mstp_port->SilenceTimer() >= Tno_token) 
            {
                    /* assume that the token has been lost */
                    mstp_port->EventCount = 0;      /* Addendum 135-2004d-8 */
                    mstp_port->master_state = MSTP_MASTER_STATE_NO_TOKEN;
                    /* set the receive frame flags to false in case we received
                         some bytes and had a timeout for some reason */
                    mstp_port->ReceivedInvalidFrame = false;
                    mstp_port->ReceivedValidFrame = false;
                    transition_now = true;
            } 
            else 
            {
                printf("[%s:%s:%d] mstp_comm_mode == MSTP_MODE_MASTER \n",
                        __FILE__, __FUNCTION__, __LINE__);
                if (mstp_port->ReceivedInvalidFrame == true) 
                {
                    printf("[%s:%s:%d] mstp_comm_mode == MSTP_MODE_MASTER \n",
                            __FILE__, __FUNCTION__, __LINE__);
                    /* ReceivedInvalidFrame */
                    /* invalid frame was received */
                    mstp_port->ReceivedInvalidFrame = false;
                } 
                else
                {
                    printf("[%s:%s:%d] mstp_comm_mode == MSTP_MODE_MASTER \n",
                            __FILE__, __FUNCTION__, __LINE__);
                    if (mstp_port->ReceivedValidFrame == true) 
                    {
                        /* wait for the next frame - remain in IDLE */
                        /* destined for me! */
                        if ((mstp_port->DestinationAddress == mstp_port->This_Station)
                            || (mstp_port->DestinationAddress == MSTP_BROADCAST_ADDRESS)) 
                        {
                            switch (mstp_port->FrameType) 
                            {
                                case FRAME_TYPE_TOKEN:
                                    /* ReceivedToken */
                                    /* tokens can't be broadcast */
                                    if (mstp_port->DestinationAddress == MSTP_BROADCAST_ADDRESS) 
                                    {
                                        break;
                                    }
                                    mstp_port->ReceivedValidFrame = false;
                                    mstp_port->FrameCount = 0;
                                    mstp_port->SoleMaster = false;
                                    mstp_port->master_state = MSTP_MASTER_STATE_USE_TOKEN;
                                    transition_now = true;
                                    break;
                                case FRAME_TYPE_POLL_FOR_MASTER:
                                    /* ReceivedPFM */
                                    MSTP_Create_And_Send_Frame(mstp_port, FRAME_TYPE_REPLY_TO_POLL_FOR_MASTER, 
                                                                                mstp_port->SourceAddress, mstp_port->This_Station, NULL, 0);
                                    break;
                                case FRAME_TYPE_BACNET_DATA_NOT_EXPECTING_REPLY:
                                    /* indicate successful reception to the higher layers */
                                    mstp_port->ReceivePacketPending = true;
                                    break;
                                case FRAME_TYPE_BACNET_DATA_EXPECTING_REPLY:
                                    /*mstp_port->ReplyPostponedTimer = 0; */
                                    /* indicate successful reception to the higher layers  */
                                    /* broadcast DER just remains IDLE */
                                    mstp_port->ReceivePacketPending = true;
                                    if (mstp_port->DestinationAddress != MSTP_BROADCAST_ADDRESS) 
                                    {
                                        mstp_port->master_state = MSTP_MASTER_STATE_ANSWER_DATA_REQUEST;
                                    }
                                    break;
                                case FRAME_TYPE_TEST_REQUEST:
                                    MSTP_Create_And_Send_Frame(mstp_port,
                                                                    FRAME_TYPE_TEST_RESPONSE,
                                                                    mstp_port->SourceAddress,
                                                                    mstp_port->This_Station,
                                                                    mstp_port->InputBuffer, mstp_port->DataLength);
                                    break;
                                case FRAME_TYPE_TEST_RESPONSE:
                                    break;
                                default:
                                    break;
                            }
                        }
                        /* For DATA_EXPECTING_REPLY, we will keep the Rx Frame for
                           reference, and the flag will be cleared in the next state */
                        if (mstp_port->master_state != MSTP_MASTER_STATE_ANSWER_DATA_REQUEST) 
                        {
                            mstp_port->ReceivedValidFrame = false;
                        }
                    }
                }
            }
            break;
        case MSTP_MASTER_STATE_USE_TOKEN:
            /* In the USE_TOKEN state, the node is allowed to send one or  */
            /* more data frames. These may be BACnet Data frames or */
            /* proprietary frames. */
            /* FIXME: We could wait for up to Tusage_delay */
            if (!mstp_port->TransmitPacketPending) 
            {
                /* NothingToSend */
                mstp_port->FrameCount = mstp_port->Nmax_info_frames;
                mstp_port->master_state = MSTP_MASTER_STATE_DONE_WITH_TOKEN;
                transition_now = true;
            } 
            else 
            {
                if (mstp_port->TransmitPacketDER) 
                {
                    frame_type = FRAME_TYPE_BACNET_DATA_EXPECTING_REPLY;
                } 
                else 
                {
                    frame_type = FRAME_TYPE_BACNET_DATA_NOT_EXPECTING_REPLY;
                }
                MSTP_Create_And_Send_Frame(mstp_port, frame_type, mstp_port->TargetDestinationAddress, mstp_port->This_Station, 
                                (uint8_t *)&Handler_Transmit_Buffer[0], mstp_port->DataSendLength);
                mstp_port->TransmitPacketPending = false;
                mstp_port->FrameCount++;
                frame_type = mstp_port->OutputBuffer[2];
                // destination = mstp_port->OutputBuffer[3];

                switch (frame_type) 
                {
                    case FRAME_TYPE_BACNET_DATA_EXPECTING_REPLY:
                        if (mstp_port->DestinationAddress == MSTP_BROADCAST_ADDRESS) 
                        {
                                /* SendNoWait */
                                mstp_port->master_state = MSTP_MASTER_STATE_DONE_WITH_TOKEN;
                        } 
                        else 
                        {
                                /* SendAndWait */
                                mstp_port->master_state = MSTP_MASTER_STATE_WAIT_FOR_REPLY;
                        }
                        break;
                    case FRAME_TYPE_TEST_REQUEST:
                            /* SendAndWait */
                            mstp_port->master_state = MSTP_MASTER_STATE_WAIT_FOR_REPLY;
                            break;
                    case FRAME_TYPE_TEST_RESPONSE:
                    case FRAME_TYPE_BACNET_DATA_NOT_EXPECTING_REPLY:
                    default:
                            /* SendNoWait */
                            mstp_port->master_state = MSTP_MASTER_STATE_DONE_WITH_TOKEN;
                            break;
                 }
            }
            break;
        case MSTP_MASTER_STATE_WAIT_FOR_REPLY:
            /* In the WAIT_FOR_REPLY state, the node waits for  */
            /* a reply from another node. */
            if (mstp_port->SilenceTimer() >= Treply_timeout) 
            {
                /* ReplyTimeout */
                /* assume that the request has failed */
                mstp_port->FrameCount = mstp_port->Nmax_info_frames;
                mstp_port->master_state = MSTP_MASTER_STATE_DONE_WITH_TOKEN;
                /* Any retry of the data frame shall await the next entry */
                /* to the USE_TOKEN state. (Because of the length of the timeout,  */
                /* this transition will cause the token to be passed regardless */
                /* of the initial value of FrameCount.) */
                transition_now = true;
            } 
            else 
            {
                if (mstp_port->ReceivedInvalidFrame == true) 
                {
                    /* InvalidFrame */
                    /* error in frame reception */
                    mstp_port->ReceivedInvalidFrame = false;
                    mstp_port->master_state = MSTP_MASTER_STATE_DONE_WITH_TOKEN;
                    transition_now = true;
                } 
                else 
                {
                    if (mstp_port->ReceivedValidFrame == true) 
                    {
                        if (mstp_port->DestinationAddress == mstp_port->This_Station) 
                        {
                            switch (mstp_port->FrameType) 
                            {
                                case FRAME_TYPE_REPLY_POSTPONED:
                                    /* ReceivedReplyPostponed */
                                    mstp_port->master_state = MSTP_MASTER_STATE_DONE_WITH_TOKEN;
                                    break;
                                case FRAME_TYPE_TEST_RESPONSE:
                                    mstp_port->master_state = MSTP_MASTER_STATE_IDLE;
                                    break;
                                case FRAME_TYPE_BACNET_DATA_NOT_EXPECTING_REPLY:
                                    /* ReceivedReply */
                                    /* or a proprietary type that indicates a reply */
                                    /* indicate successful reception to the higher layers */
                                    //(void) MSTP_Put_Receive(mstp_port);
                                    mstp_port->ReceivePacketPending = true;
                                    mstp_port->master_state = MSTP_MASTER_STATE_DONE_WITH_TOKEN;
                                    break;
                                default:
                                    /* if proprietary frame was expected, you might
                                       need to transition to DONE WITH TOKEN */
                                    mstp_port->master_state = MSTP_MASTER_STATE_IDLE;
                                    break;
                            }
                        } 
                        else 
                        {
                            /* ReceivedUnexpectedFrame */
                            /* an unexpected frame was received */
                            /* This may indicate the presence of multiple tokens.  */
                            /* Synchronize with the network. */
                            /* This action drops the token. */
                            mstp_port->master_state = MSTP_MASTER_STATE_IDLE;
                        }
                        mstp_port->ReceivedValidFrame = false;
                        transition_now = true;
                    }
                }
            }
            break;
        case MSTP_MASTER_STATE_DONE_WITH_TOKEN:
            /* The DONE_WITH_TOKEN state either sends another data frame,  */
            /* passes the token, or initiates a Poll For Master cycle. */
            /* SendAnotherFrame */
            if (mstp_port->FrameCount < mstp_port->Nmax_info_frames) 
                        {
                /* then this node may send another information frame  */
                /* before passing the token.  */
                mstp_port->master_state = MSTP_MASTER_STATE_USE_TOKEN;
                transition_now = true;
            } 
            else
            {
                if (mstp_port->TokenCount < (Npoll - 1)) 
                {
                    /* Npoll changed in Errata SSPC-135-2004 */
                    if ((mstp_port->SoleMaster == true) &&
                        (mstp_port->Next_Station != next_this_station)) 
                    {
                        /* SoleMaster */
                        /* there are no other known master nodes to */
                        /* which the token may be sent (true master-slave operation).  */
                        mstp_port->FrameCount = 0;
                        mstp_port->TokenCount++;
                        mstp_port->master_state = MSTP_MASTER_STATE_USE_TOKEN;
                        transition_now = true;
                    } 
                    else 
                    {
                        /* SendToken */
                        /* Npoll changed in Errata SSPC-135-2004 */
                        /* The comparison of NS and TS+1 eliminates the Poll For Master  */
                        /* if there are no addresses between TS and NS, since there is no  */
                        /* address at which a new master node may be found in that case. */
                        mstp_port->TokenCount++;
                        /* transmit a Token frame to NS */
                        MSTP_Create_And_Send_Frame(mstp_port, FRAME_TYPE_TOKEN,
                            mstp_port->Next_Station, mstp_port->This_Station, NULL, 0);

                        mstp_port->RetryCount = 0;
                        mstp_port->EventCount = 0;
                        mstp_port->master_state = MSTP_MASTER_STATE_PASS_TOKEN;
                    }
                } 
                else
                {
                    if (next_poll_station == mstp_port->Next_Station) 
                    {
                        if (mstp_port->SoleMaster == true) 
                        {
                            /* SoleMasterRestartMaintenancePFM */
                            mstp_port->Poll_Station = next_next_station;

                            MSTP_Create_And_Send_Frame(mstp_port,
                                FRAME_TYPE_POLL_FOR_MASTER, mstp_port->Poll_Station,
                                mstp_port->This_Station, NULL, 0);
                            /* no known successor node */
                            mstp_port->Next_Station = mstp_port->This_Station;
                            mstp_port->RetryCount = 0;
                            /* changed in Errata SSPC-135-2004 */
                            mstp_port->TokenCount = 1;
                            /* mstp_port->EventCount = 0; removed in Addendum 135-2004d-8 */
                            /* find a new successor to TS */
                            mstp_port->master_state = MSTP_MASTER_STATE_POLL_FOR_MASTER;
                        } 
                        else 
                        {
                            /* ResetMaintenancePFM */
                            mstp_port->Poll_Station = mstp_port->This_Station;
                            /* transmit a Token frame to NS */
                            MSTP_Create_And_Send_Frame(mstp_port, FRAME_TYPE_TOKEN,
                                mstp_port->Next_Station, mstp_port->This_Station, NULL,
                                0);
                            mstp_port->RetryCount = 0;
                            /* changed in Errata SSPC-135-2004 */
                            mstp_port->TokenCount = 1;
                            mstp_port->EventCount = 0;
                            mstp_port->master_state = MSTP_MASTER_STATE_PASS_TOKEN;
                        }
                    } 
                    else 
                    {
                        /* SendMaintenancePFM */
                        mstp_port->Poll_Station = next_poll_station;

                        MSTP_Create_And_Send_Frame(mstp_port,
                            FRAME_TYPE_POLL_FOR_MASTER, mstp_port->Poll_Station,
                            mstp_port->This_Station, NULL, 0);
                        mstp_port->RetryCount = 0;
                        mstp_port->master_state = MSTP_MASTER_STATE_POLL_FOR_MASTER;
                    }
                }
            }
            break;
        case MSTP_MASTER_STATE_PASS_TOKEN:
            /* The PASS_TOKEN state listens for a successor to begin using */
            /* the token that this node has just attempted to pass. */
            if (mstp_port->SilenceTimer() <= Tusage_timeout) 
            {
                if (mstp_port->EventCount > Nmin_octets) 
                {
                    /* SawTokenUser */
                    /* Assume that a frame has been sent by the new token user.  */
                    /* Enter the IDLE state to process the frame. */
                    mstp_port->master_state = MSTP_MASTER_STATE_IDLE;
                    transition_now = true;
                }
            } 
            else 
            {
                if (mstp_port->RetryCount < Nretry_token) 
                {
                    /* RetrySendToken */
                    mstp_port->RetryCount++;
                    /* Transmit a Token frame to NS */
                    MSTP_Create_And_Send_Frame(mstp_port, FRAME_TYPE_TOKEN,
                        mstp_port->Next_Station, mstp_port->This_Station, NULL,
                        0);
                    mstp_port->EventCount = 0;
                    /* re-enter the current state to listen for NS  */
                    /* to begin using the token. */
                } 
                else 
                {
                    /* FindNewSuccessor */
                    /* Assume that NS has failed.  */
                    mstp_port->Poll_Station = next_next_station;
                    /* Transmit a Poll For Master frame to PS. */
                    MSTP_Create_And_Send_Frame(mstp_port,
                        FRAME_TYPE_POLL_FOR_MASTER, mstp_port->Poll_Station,
                        mstp_port->This_Station, NULL, 0);
                    /* no known successor node */
                    mstp_port->Next_Station = mstp_port->This_Station;
                    mstp_port->RetryCount = 0;
                    mstp_port->TokenCount = 0;
                    /* mstp_port->EventCount = 0; removed in Addendum 135-2004d-8 */
                    /* find a new successor to TS */
                    mstp_port->master_state = MSTP_MASTER_STATE_POLL_FOR_MASTER;
                }
            }
            break;
        case MSTP_MASTER_STATE_NO_TOKEN:
            /* The NO_TOKEN state is entered if mstp_port->SilenceTimer() becomes greater  */
            /* than Tno_token, indicating that there has been no network activity */
            /* for that period of time. The timeout is continued to determine  */
            /* whether or not this node may create a token. */
            my_timeout = Tno_token + (Tslot * mstp_port->This_Station);
            if (mstp_port->SilenceTimer() < my_timeout) 
            {
                if (mstp_port->EventCount > Nmin_octets) 
                {
                    /* SawFrame */
                    /* Some other node exists at a lower address.  */
                    /* Enter the IDLE state to receive and process the incoming frame. */
                    mstp_port->master_state = MSTP_MASTER_STATE_IDLE;
                    transition_now = true;
                }
            } 
            else 
            {
                ns_timeout = Tno_token + (Tslot * (mstp_port->This_Station + 1));
                if (mstp_port->SilenceTimer() < ns_timeout) 
                {
                    /* GenerateToken */
                    /* Assume that this node is the lowest numerical address  */
                    /* on the network and is empowered to create a token.  */
                    mstp_port->Poll_Station = next_this_station;
                    /* Transmit a Poll For Master frame to PS. */
                    MSTP_Create_And_Send_Frame(mstp_port, FRAME_TYPE_POLL_FOR_MASTER, 
                                            mstp_port->Poll_Station, mstp_port->This_Station, NULL, 0);
                    /* indicate that the next station is unknown */
                    mstp_port->Next_Station = mstp_port->This_Station;
                    mstp_port->RetryCount = 0;
                    mstp_port->TokenCount = 0;
                    /* mstp_port->EventCount = 0; removed Addendum 135-2004d-8 */
                    /* enter the POLL_FOR_MASTER state to find a new successor to TS. */
                    mstp_port->master_state = MSTP_MASTER_STATE_POLL_FOR_MASTER;
                }
            }
            break;
        case MSTP_MASTER_STATE_POLL_FOR_MASTER:
            /* In the POLL_FOR_MASTER state, the node listens for a reply to */
            /* a previously sent Poll For Master frame in order to find  */
            /* a successor node. */
            if (mstp_port->ReceivedValidFrame == true) 
            {
                if ((mstp_port->DestinationAddress == mstp_port->This_Station)
                    && (mstp_port->FrameType == FRAME_TYPE_REPLY_TO_POLL_FOR_MASTER)) 
                {
                    /* ReceivedReplyToPFM */
                    mstp_port->SoleMaster = false;
                    mstp_port->Next_Station = mstp_port->SourceAddress;
                    mstp_port->EventCount = 0;
                    /* Transmit a Token frame to NS */
                    MSTP_Create_And_Send_Frame(mstp_port, FRAME_TYPE_TOKEN,
                        mstp_port->Next_Station, mstp_port->This_Station, NULL,
                        0);
                    mstp_port->Poll_Station = mstp_port->This_Station;
                    mstp_port->TokenCount = 0;
                    mstp_port->RetryCount = 0;
                    mstp_port->master_state = MSTP_MASTER_STATE_PASS_TOKEN;
                } 
                else 
                {
                    /* ReceivedUnexpectedFrame */
                    /* An unexpected frame was received.  */
                    /* This may indicate the presence of multiple tokens. */
                    /* enter the IDLE state to synchronize with the network.  */
                    /* This action drops the token. */
                    mstp_port->master_state = MSTP_MASTER_STATE_IDLE;
                    transition_now = true;
                }
                mstp_port->ReceivedValidFrame = false;
            } 
            else
            {
                if ((mstp_port->SilenceTimer() > Tusage_timeout) ||
                    (mstp_port->ReceivedInvalidFrame == true)) 
                {
                    if (mstp_port->SoleMaster == true) 
                    {
                        /* SoleMaster */
                        /* There was no valid reply to the periodic poll  */
                        /* by the sole known master for other masters. */
                        mstp_port->FrameCount = 0;
                        /* mstp_port->TokenCount++; removed in 2004 */
                        mstp_port->master_state = MSTP_MASTER_STATE_USE_TOKEN;
                        transition_now = true;
                    } 
                    else 
                    {
                        if (mstp_port->Next_Station != mstp_port->This_Station) 
                        {
                            /* DoneWithPFM */
                            /* There was no valid reply to the maintenance  */
                            /* poll for a master at address PS.  */
                            mstp_port->EventCount = 0;
                            /* transmit a Token frame to NS */
                            MSTP_Create_And_Send_Frame(mstp_port, 
                                                        FRAME_TYPE_TOKEN,
                                                        mstp_port->Next_Station, 
                                                        mstp_port->This_Station,
                                                        NULL, 
                                                        0);
                            mstp_port->RetryCount = 0;
                            mstp_port->master_state = MSTP_MASTER_STATE_PASS_TOKEN;
                        } 
                        else 
                        {
                            if (next_poll_station != mstp_port->This_Station) 
                            {
                                /* SendNextPFM */
                                mstp_port->Poll_Station = next_poll_station;
                                /* Transmit a Poll For Master frame to PS. */
                                MSTP_Create_And_Send_Frame(mstp_port,
                                                            FRAME_TYPE_POLL_FOR_MASTER,
                                                            mstp_port->Poll_Station,
                                                            mstp_port->This_Station, 
                                                            NULL, 
                                                            0);
                                mstp_port->RetryCount = 0;

                                /* Re-enter the current state. */
                            } 
                            else 
                            {
                                /* DeclareSoleMaster */
                                /* to indicate that this station is the only master */
                                mstp_port->SoleMaster = true;
                                mstp_port->FrameCount = 0;
                                mstp_port->master_state =
                                    MSTP_MASTER_STATE_USE_TOKEN;
                                transition_now = true;
                            }
                        }
                    }
                    mstp_port->ReceivedInvalidFrame = false;
                }
            }
            break;
        case MSTP_MASTER_STATE_ANSWER_DATA_REQUEST:
            /* The ANSWER_DATA_REQUEST state is entered when a  */
            /* BACnet Data Expecting Reply, a Test_Request, or  */
            /* a proprietary frame that expects a reply is received. */
            if(mstp_port->TransmitPacketPending)
            {
                matched = dlmstp_compare_data_expecting_reply(mstp_port->InputBuffer, mstp_port->DataLength, mstp_port->SourceAddress, 
                                                        (uint8_t*)&Handler_Transmit_Buffer[0], mstp_port->DataSendLength, mstp_port->TargetDestinationAddress);
            }
            
            if (mstp_port->SilenceTimer() <= Treply_delay && mstp_port->TransmitPacketPending && matched) 
            {
                /* Reply */
                /* If a reply is available from the higher layers  */
                /* within Treply_delay after the reception of the  */
                /* final octet of the requesting frame  */
                /* (the mechanism used to determine this is a local matter), */
                /* then call MSTP_Create_And_Send_Frame to transmit the reply frame  */
                /* and enter the IDLE state to wait for the next frame. */
                if (mstp_port->TransmitPacketDER) 
                {
                    frame_type = FRAME_TYPE_BACNET_DATA_EXPECTING_REPLY;
                } 
                else 
                {
                    frame_type = FRAME_TYPE_BACNET_DATA_NOT_EXPECTING_REPLY;
                }
                MSTP_Create_And_Send_Frame(mstp_port, 
                                            frame_type, 
                                            mstp_port->TargetDestinationAddress, 
                                            mstp_port->This_Station,
                                            (uint8_t *)&Handler_Transmit_Buffer[0],
                                            mstp_port->DataSendLength);
                mstp_port->TransmitPacketPending = false;
                mstp_port->master_state = MSTP_MASTER_STATE_IDLE;
            } 
            else 
            {
                /* DeferredReply */
                /* If no reply will be available from the higher layers */
                /* within Treply_delay after the reception of the */
                /* final octet of the requesting frame (the mechanism */
                /* used to determine this is a local matter), */
                /* then an immediate reply is not possible. */
                /* Any reply shall wait until this node receives the token. */
                /* Call MSTP_Create_And_Send_Frame to transmit a Reply Postponed frame, */
                /* and enter the IDLE state. */
                MSTP_Create_And_Send_Frame(mstp_port,
                                            FRAME_TYPE_REPLY_POSTPONED, 
                                            mstp_port->SourceAddress,
                                            mstp_port->This_Station, 
                                            NULL, 
                                            0);
                mstp_port->master_state = MSTP_MASTER_STATE_IDLE;
            }
            /* clear our flag we were holding for comparison */
            mstp_port->ReceivedValidFrame = false;
            break;
        default:
            mstp_port->master_state = MSTP_MASTER_STATE_IDLE;
            break;
    }

    return transition_now;
}

bool MSTP_Slave_Node_FSM(volatile struct mstp_port_struct_t *mstp_port)
{
    // unsigned length = 0;
    // bool matched= false;

    if(mstp_port->ReceivedInvalidFrame)
    {
        mstp_port->ReceivedInvalidFrame= false;
    }
    else
    {
        if (mstp_port->ReceivedValidFrame) 
        {
            mstp_port->ReceivedValidFrame = false;
            switch (mstp_port->FrameType) 
            {
                case FRAME_TYPE_BACNET_DATA_EXPECTING_REPLY:
                    if (mstp_port->DestinationAddress != MSTP_BROADCAST_ADDRESS)
                    {
                        /* indicate successful reception to the higher layers  */
                        mstp_port->ReceivePacketPending = true;
                    }
                    break;
                case FRAME_TYPE_TEST_REQUEST:
                    MSTP_Create_And_Send_Frame(mstp_port,
                                            FRAME_TYPE_TEST_RESPONSE, 
                                            mstp_port->SourceAddress,
                                            mstp_port->This_Station, 
                                            &mstp_port->InputBuffer[0], 
                                            mstp_port->DataLength);
                    break;
                case FRAME_TYPE_BACNET_DATA_NOT_EXPECTING_REPLY:
                case FRAME_TYPE_TOKEN:
                case FRAME_TYPE_POLL_FOR_MASTER:
                case FRAME_TYPE_TEST_RESPONSE:
                default:
                    break;
            }
        } 
        else 
        {
            if (mstp_port->TransmitPacketPending) 
            {
                /* Reply */
                /* If a reply is available from the higher layers  */
                /* within Treply_delay after the reception of the  */
                /* final octet of the requesting frame  */
                /* (the mechanism used to determine this is a local matter), */
                /* then call MSTP_Send_Frame to transmit the reply frame  */
                /* and enter the IDLE state to wait for the next frame. */
                /* Note: optimized such that we are never a client */
                MSTP_Create_And_Send_Frame(mstp_port,
                                FRAME_TYPE_BACNET_DATA_NOT_EXPECTING_REPLY,
                                mstp_port->TargetDestinationAddress, 
                                mstp_port->This_Station, 
                                (uint8_t *)&Handler_Transmit_Buffer[0],
                                mstp_port->DataSendLength);
                mstp_port->TransmitPacketPending = false;
                mstp_port->ReceivePacketPending = false;
            }
        }
    }
    
    return false;
}

/* note: This_Station assumed to be set with the MAC address */
/* note: Nmax_info_frames assumed to be set (default=1) */
/* note: Nmax_master assumed to be set (default=127) */
/* note: InputBuffer and InputBufferSize assumed to be set */
/* note: OutputBuffer and OutputBufferSize assumed to be set */
/* note: SilenceTimer and SilenceTimerReset assumed to be set */

void MSTP_Init(volatile struct mstp_port_struct_t *mstp_port)
{
    if (mstp_port) {
        // mstp_port->InputBuffer = &MSTP_InputBuf[0];
        // mstp_port->InputBufferSize = sizeof(MSTP_InputBuf);
        // mstp_port->OutputBuffer = &MSTP_OutputBuf[0];
        // mstp_port->OutputBufferSize = sizeof(MSTP_OutputBuf);
        
        /* FIXME: these are adjustable, so you must set these in dlmstp */
        mstp_port->Nmax_info_frames = DEFAULT_MAX_INFO_FRAMES;
        mstp_port->Nmax_master = DEFAULT_MAX_MASTER;
        /* FIXME: point to functions */
        mstp_port->SilenceTimer = Silence_TimeElapsed;
        mstp_port->SilenceTimerReset = Silence_Reset;

        mstp_port->receive_state = MSTP_RECEIVE_STATE_IDLE;
        mstp_port->master_state = MSTP_MASTER_STATE_INITIALIZE;
        mstp_port->ReceiveError = false;
        mstp_port->DataAvailable = false;
        mstp_port->DataRegister = 0;
        mstp_port->DataCRC = 0;
        mstp_port->DataLength = 0;
        mstp_port->TargetDestinationAddress = 0;
        mstp_port->EventCount = 0;
        mstp_port->FrameType = FRAME_TYPE_TOKEN;
        mstp_port->FrameCount = 0;
        mstp_port->HeaderCRC = 0;
        mstp_port->Index = 0;
        mstp_port->Next_Station = mstp_port->This_Station;
        mstp_port->Poll_Station = mstp_port->This_Station;
        mstp_port->ReceivedInvalidFrame = false;
        mstp_port->ReceivedValidFrame = false;
        mstp_port->RetryCount = 0;
        mstp_port->SilenceTimerReset();
        mstp_port->SoleMaster = false;
        mstp_port->SourceAddress = 0;
        mstp_port->TokenCount = 0;
        mstp_port->Lurking = false;
       
        mstp_port->DataSendLength= 0;
        mstp_port->TransmitPacketDER= false;
        mstp_port->ReceivePacketPending= false;
        mstp_port->TransmitPacketPending= false;
        mstp_port->DestinationAddress = 0;
    }
}

