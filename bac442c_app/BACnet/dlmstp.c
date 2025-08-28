/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2007 Steve Karg

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
 use macros or inline functions from this file, or you compile
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
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "bacdef.h"
#include "dlmstp.h"
#include "hal_usart1_dma.h"
#include "crc.h"
#include "npdu.h"
#include "apdu.h"
#include "bacaddr.h"
#include "handlers.h"
#include "address.h"
#include "device.h"

#include "modbus.h"
#include "mb_var_def.h"
#include "variables.h"

#include "hal_usart1_dma.h"

// extern struct mstp_port_struct_t MSTP_Port;

/* This file has been customized for use with small microprocessors */
/* Assumptions:
    Only one MS/TP datalink layer
*/
#include "silence_timer.h"

/*  The value 255 is used to denote broadcast when used as a */
/* destination address but is not allowed as a value for a station. */
/* Station addresses for master nodes can be 0-127.  */
/* Station addresses for slave nodes can be 127-254.  */
#define MSTP_BROADCAST_ADDRESS 255

/* MS/TP Frame Type */
/* Frame Types 8 through 127 are reserved by ASHRAE. */
#define FRAME_TYPE_TOKEN 0
#define FRAME_TYPE_POLL_FOR_MASTER 1
#define FRAME_TYPE_REPLY_TO_POLL_FOR_MASTER 2
#define FRAME_TYPE_TEST_REQUEST 3
#define FRAME_TYPE_TEST_RESPONSE 4
#define FRAME_TYPE_BACNET_DATA_EXPECTING_REPLY 5
#define FRAME_TYPE_BACNET_DATA_NOT_EXPECTING_REPLY 6
#define FRAME_TYPE_REPLY_POSTPONED 7
/* Frame Types 128 through 255: Proprietary Frames */
/* These frames are available to vendors as proprietary (non-BACnet) frames. */
/* The first two octets of the Data field shall specify the unique vendor */
/* identification code, most significant octet first, for the type of */
/* vendor-proprietary frame to be conveyed. The length of the data portion */
/* of a Proprietary frame shall be in the range of 2 to 501 octets. */
#define FRAME_TYPE_PROPRIETARY_MIN 128
#define FRAME_TYPE_PROPRIETARY_MAX 255

#define MSTP_MODE_NONE        0
#define MSTP_MODE_MASTER    1
#define MSTP_MODE_SLAVE        2

uint8_t mstp_comm_mode;

/* we need to be able to increment without rolling over */
#define INCREMENT_AND_LIMIT_UINT8(x) {if (x < 0xFF) x++;}

bool dlmstp_init(char *ifname)
{
    (void) ifname;
    /* initialize hardware */

    return true;
}

void dlmstp_cleanup(void)
{
    /* nothing to do for static buffers */
}

void dlmstp_set_comm_mode(uint8_t mode)
{
    mstp_comm_mode= mode;
}

void dlmstp_fill_bacnet_address(
    BACNET_ADDRESS * src,
    uint8_t mstp_address)
{
    int i = 0;

    if (mstp_address == MSTP_BROADCAST_ADDRESS) {
        /* mac_len = 0 if broadcast address */
        src->mac_len = 0;
        src->mac[0] = 0;
    } else {
        src->mac_len = 1;
        src->mac[0] = mstp_address;
    }
    /* fill with 0's starting with index 1; index 0 filled above */
    for (i = 1; i < MAX_MAC_LEN; i++) {
        src->mac[i] = 0;
    }
    src->net = 0;
    src->len = 0;
    for (i = 0; i < MAX_MAC_LEN; i++) {
        src->adr[i] = 0;
    }
}

bool dlmstp_compare_data_expecting_reply(
    uint8_t * request_pdu,
    uint16_t request_pdu_len,
    uint8_t src_address,
    uint8_t * reply_pdu,
    uint16_t reply_pdu_len,
    uint8_t dest_address)
{
    uint16_t offset;
    
    /* One way to check the message is to compare NPDU
       src, dest, along with the APDU type, invoke id.
       Seems a bit overkill */
    struct DER_compare_t 
    {
        BACNET_NPDU_DATA npdu_data;
        BACNET_ADDRESS address;
        uint8_t pdu_type;
        uint8_t invoke_id;
        uint8_t service_choice;
    };
    struct DER_compare_t request;
    struct DER_compare_t reply;

    /* decode the request data */
    request.address.mac[0] = src_address;
    request.address.mac_len = 1;
    offset = npdu_decode(&request_pdu[0], NULL, &request.address, &request.npdu_data);
    if (request.npdu_data.network_layer_message) 
    {
        return false;
    }
    request.pdu_type = request_pdu[offset] & 0xF0;
    if (request.pdu_type != PDU_TYPE_CONFIRMED_SERVICE_REQUEST) 
    {
        return false;
    }
    request.invoke_id = request_pdu[offset + 2];
    /* segmented message? */
    if (request_pdu[offset] & BIT3)
    {
        request.service_choice = request_pdu[offset + 5];
    }
    else
    {
        request.service_choice = request_pdu[offset + 3];
    }
    /* decode the reply data */
    reply.address.mac[0] = dest_address;
    reply.address.mac_len = 1;
    offset = npdu_decode(&reply_pdu[0], &reply.address, NULL, &reply.npdu_data);
 
    if (reply.npdu_data.network_layer_message) 
    {
        return false;
    }
    /* reply could be a lot of things:
       confirmed, simple ack, abort, reject, error */
    reply.pdu_type = reply_pdu[offset] & 0xF0;
    
    switch (reply.pdu_type) 
    {
        case PDU_TYPE_CONFIRMED_SERVICE_REQUEST:
            reply.invoke_id = reply_pdu[offset + 2];
            /* segmented message? */
            if (reply_pdu[offset] & BIT3)
            {
                reply.service_choice = reply_pdu[offset + 5];
            }
            else
            {
                reply.service_choice = reply_pdu[offset + 3];
            }
            break;
        case PDU_TYPE_SIMPLE_ACK:
            reply.invoke_id = reply_pdu[offset + 1];
            reply.service_choice = reply_pdu[offset + 2];
            break;
        case PDU_TYPE_COMPLEX_ACK:
            reply.invoke_id = reply_pdu[offset + 1];
            /* segmented message? */
            if (reply_pdu[offset] & BIT3)
            {
                reply.service_choice = reply_pdu[offset + 4];
            }
            else
            {
                reply.service_choice = reply_pdu[offset + 2];
            }
            break;
        case PDU_TYPE_ERROR:
            reply.invoke_id = reply_pdu[offset + 1];
            reply.service_choice = reply_pdu[offset + 2];
            break;
        case PDU_TYPE_REJECT:
        case PDU_TYPE_ABORT:
            reply.invoke_id = reply_pdu[offset + 1];
            break;
        default:
            return false;
    }
    
    if (request.invoke_id != reply.invoke_id) 
    {
        return false;
    }
    /* these services don't have service choice included */
    if ((reply.pdu_type != PDU_TYPE_REJECT) && (reply.pdu_type != PDU_TYPE_ABORT)) 
    {
        if (request.service_choice != reply.service_choice) 
        {
            return false;
        }
    }
    if (request.npdu_data.protocol_version != reply.npdu_data.protocol_version) 
    {
        return false;
    }
    if (request.npdu_data.priority != reply.npdu_data.priority) 
    {
        return false;
    }
    if (!bacnet_address_same(&request.address, &reply.address)) 
    {
        return false;
    }

    return true;
}


/* returns number of bytes sent on success, zero on failure */
int dlmstp_send_pdu(
	struct mstp_port_struct_t *mstp_port,
    BACNET_ADDRESS * dest,      /* destination address */
    BACNET_NPDU_DATA * npdu_data,       /* network information */
    uint8_t * pdu,      /* any data to be sent - may be null */
    unsigned pdu_len)     /* number of bytes of data */
{      
    int bytes_sent = 0;

    if (mstp_port->TransmitPacketPending == false) 
    {
        mstp_port->TransmitPacketDER = npdu_data->data_expecting_reply;
		// mstp_port->TransmitPacketDER= 
		// 	 BACNET_DATA_EXPECTING_REPLY(pdu[BACNET_PDU_CONTROL_BYTE_OFFSET]);
        mstp_port->DataSendLength = pdu_len;
        mstp_port->TargetDestinationAddress = dest->mac[0];
        mstp_port->TransmitPacketPending = true;
		
		// memcpy(mstp_port->TransmitBuffer, pdu, pdu_len);
        
        bytes_sent = pdu_len;
    }

    return bytes_sent;
}

/* Return the length of the packet */
uint16_t dlmstp_receive(
	struct mstp_port_struct_t *mstp_port,
    BACNET_ADDRESS * src,       /* source address */
    uint8_t * pdu,      /* PDU data */
    uint16_t max_pdu,   /* amount of space available in the PDU  */
    unsigned timeout)
{       /* milliseconds to wait for a packet */
    uint16_t pdu_len = 0;       /* return value */

    /* only do master state machine while rx is idle */
    if (mstp_port->receive_state == MSTP_RECEIVE_STATE_IDLE) 
    {
            if(mstp_comm_mode == MSTP_MODE_MASTER 
                && mstp_port->This_Station < 128)
            {
                while (MSTP_Master_Node_FSM(mstp_port)) 
                {
                    /* do nothing while some states fast transition */
                };
            }
            else
            {
                 while (MSTP_Slave_Node_FSM(mstp_port)) 
                {
                        /* do nothing while some states fast transition */
                };
            }
    }
    /* if there is a packet that needs processed, do it now. */
    if (mstp_port->ReceivePacketPending)// && !mstp_port->TransmitPacketPending) 
    {
        mstp_port->ReceivePacketPending = false;
        pdu_len = mstp_port->DataLength;
        src->mac_len = 1;
        src->mac[0] = mstp_port->SourceAddress;
        /* data is already in the pdu pointer */
    }

    return pdu_len;
}

void dlmstp_set_mac_address(struct mstp_port_struct_t *mstp_port, uint8_t mac_address)
{
    /* Master Nodes can only have address 0-127 */
   if (mac_address <= 127) 
   {
            mstp_port->This_Station = mac_address;
            /* FIXME: implement your data storage */
            /* I2C_Write_Byte(
                 EEPROM_DEVICE_ADDRESS,
                 mac_address,
                 EEPROM_MSTP_MAC_ADDR); */   

            if (mac_address > mstp_port->Nmax_master)
            {
                dlmstp_set_max_master(mstp_port, mac_address);
            }
    }
    return;
}

uint8_t dlmstp_mac_address(struct mstp_port_struct_t *mstp_port)
{
    return mstp_port->This_Station;
}

/* This parameter represents the value of the Max_Info_Frames property of */
/* the node's Device object. The value of Max_Info_Frames specifies the */
/* maximum number of information frames the node may send before it must */
/* pass the token. Max_Info_Frames may have different values on different */
/* nodes. This may be used to allocate more or less of the available link */
/* bandwidth to particular nodes. If Max_Info_Frames is not writable in a */
/* node, its value shall be 1. */
void dlmstp_set_max_info_frames(struct mstp_port_struct_t *mstp_port,
    uint8_t max_info_frames)
{
    if (max_info_frames >= 1) 
        {
       mstp_port->Nmax_info_frames = max_info_frames;
        /* FIXME: implement your data storage */
        /* I2C_Write_Byte(
           EEPROM_DEVICE_ADDRESS,
           (uint8_t)max_info_frames,
           EEPROM_MSTP_MAX_INFO_FRAMES_ADDR); */
            
            VAR_SetIntValue(HR_MAX_INFO_FRAME,  mstp_port->Nmax_info_frames, 0);
    }

    return;
}

uint8_t dlmstp_max_info_frames(struct mstp_port_struct_t *mstp_port)
{
    return mstp_port->Nmax_info_frames;
}

/* This parameter represents the value of the Max_Master property of the */
/* node's Device object. The value of Max_Master specifies the highest */
/* allowable address for master nodes. The value of Max_Master shall be */
/* less than or equal to 127. If Max_Master is not writable in a node, */
/* its value shall be 127. */
void dlmstp_set_max_master(struct mstp_port_struct_t *mstp_port, uint8_t max_master)
{
    if (max_master <= 127) 
    {
        if (mstp_port->This_Station <= max_master) 
        {
           mstp_port->Nmax_master = max_master;
            /* FIXME: implement your data storage */
            /* I2C_Write_Byte(
                 EEPROM_DEVICE_ADDRESS,
                 max_master,
                 EEPROM_MSTP_MAX_MASTER_ADDR); */
            VAR_SetIntValue(HR_MAX_MASTER,  mstp_port->Nmax_master, 0);
        }
    }

    return;
}

uint8_t dlmstp_max_master(struct mstp_port_struct_t *mstp_port)
{
    return mstp_port->Nmax_master;
}

void dlmstp_get_my_address(struct mstp_port_struct_t *mstp_port, BACNET_ADDRESS * my_address)
{
    int i = 0;  /* counter */

    my_address->mac_len = 1;
    my_address->mac[0] = mstp_port->This_Station;
    my_address->net = 0;        /* local only, no routing */
    my_address->len = 0;
    for (i = 0; i < MAX_MAC_LEN; i++) 
    {
        my_address->adr[i] = 0;
    }

    return;
}

void dlmstp_get_broadcast_address(BACNET_ADDRESS * dest)
{       /* destination address */
    int i = 0;  /* counter */

    if (dest) 
    {
        dest->mac_len = 1;
        dest->mac[0] = MSTP_BROADCAST_ADDRESS;
        dest->net = BACNET_BROADCAST_NETWORK;
        dest->len = 0;  /* always zero when DNET is broadcast */
        for (i = 0; i < MAX_MAC_LEN; i++) 
        {
            dest->adr[i] = 0;
        }
    }

    return;
}


