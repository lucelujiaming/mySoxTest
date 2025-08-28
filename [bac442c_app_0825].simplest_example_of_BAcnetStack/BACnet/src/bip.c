/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2005 Steve Karg

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
 Boston, MA  02111-1307, USA.

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

#include <stdlib.h>
#include <stdio.h> 

#include <stdint.h>     /* for standard integer types uint8_t etc. */
#include <stdbool.h>    /* for the standard bool type. */
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "bacdcode.h"
#include "bacint.h"
#include "bip.h"
#include "net.h"        /* custom per port */

/** @file bip.c  Configuration and Operations for BACnet/IP */

static int BIP_Socket = -1;
/* port to use - stored in host byte order */
static uint16_t BIP_Port = 0xBAC0;
/* IP Address - stored in host byte order */
static struct in_addr BIP_Address;
/* Broadcast Address - stored in host byte order */
static struct in_addr BIP_Broadcast_Address;

static uint8_t mtu[MAX_MPDU] = { 0 };
/** Setter for the BACnet/IP socket handle.
 * 
 * @param sock_fd [in] Handle for the BACnet/IP socket.
 */
 
void bip_set_socket(int sock_fd)
{
    BIP_Socket = sock_fd;
}

/** Getter for the BACnet/IP socket handle.
 * 
 * @return The handle to the BACnet/IP socket.
 */
 
int bip_socket(void)
{
    return BIP_Socket;
}


bool bip_valid(void)
{
    return (BIP_Socket != -1);
}

/* set using network byte order */

void bip_set_addr(uint32_t net_address)
{
    BIP_Address.s_addr = net_address;
}

/* returns host byte order */

uint32_t bip_get_addr(void)
{
    return BIP_Address.s_addr;
}

/* set using network byte order */

void bip_set_broadcast_addr(uint32_t net_address)
{
    BIP_Broadcast_Address.s_addr = net_address;
}

/* returns host byte order */

uint32_t bip_get_broadcast_addr(void)
{
    return BIP_Broadcast_Address.s_addr;
}

/* set using host byte order */

void bip_set_dest_port(uint16_t port)
{
    BIP_Port = port;
}

void bip_set_port(uint16_t port)
{
    BIP_Port = port;
}

/* returns host byte order */

uint16_t bip_get_port(void)
{
    return BIP_Port;
}


static int bip_decode_bip_address(
    uint8_t * pdu,      /* buffer to extract encoded address */
    struct in_addr *address,    /* in host format */
    uint16_t * port)
{
    int len = 0;
    // uint32_t raw_address = 0;

    if (pdu) 
    {
       // (void) decode_unsigned32(&pdu[0], &raw_address);
       // address->s_addr = raw_address;
        char cIP[32];
        memset(cIP, 0x00, 32);
        sprintf(cIP, "%d.%d.%d.%d", pdu[0], pdu[1], pdu[2], pdu[3]);
        // printf("bip_decode_bip_address::cIP = %s \r\n", cIP);
        inet_pton(AF_INET, cIP, address);
        (void) decode_unsigned16(&pdu[4], port);
        len = 6;
    }

    return len;
}

/** Function to send a packet out the BACnet/IP socket (Annex J).
 * @ingroup DLBIP
 *
 * @param dest [in] Destination address (may encode an IP address and port #).
 * @param npdu_data [in] The NPDU header (Network) information (not used).
 * @param pdu [in] Buffer of data to be sent - may be null (why?).
 * @param pdu_len [in] Number of bytes in the pdu buffer.
 * @return Number of bytes sent on success, negative number on failure.
 */

int bip_send_pdu(
    BACNET_ADDRESS * dest,      /* destination address */
    BACNET_NPDU_DATA * npdu_data,       /* network information */
    uint8_t * pdu,      /* any data to be sent - may be null */
    unsigned pdu_len)
{       /* number of bytes of data */
    struct sockaddr_in bip_dest;
    
    int mtu_len = 0;
    int bytes_sent = 0;
    /* addr and port in host format */
    struct in_addr address;
    address.s_addr = inet_addr("192.168.168.129");
    uint16_t port = 0;

        memset(&mtu[0], 0, MAX_MPDU);
    
    (void) npdu_data;
    /* assumes that the driver has already been initialized */
    if (BIP_Socket < 0)
    {
        return BIP_Socket;
    }
    
    mtu[0] = BVLL_TYPE_BACNET_IP;
    bip_dest.sin_family = AF_INET;
    
    if (dest->net == BACNET_BROADCAST_NETWORK) 
    {
        /* broadcast */
        address.s_addr = BIP_Broadcast_Address.s_addr;
        port = BIP_Port;
            printf("[%s:%s:%d] port = %d  \n", 
                __FILE__, __FUNCTION__, __LINE__, port);
        mtu[1] = BVLC_ORIGINAL_BROADCAST_NPDU;
    } 
    else 
    {
            if (dest->mac_len == 6) 
            {
                bip_decode_bip_address(&dest->mac[0], &address, &port);
                // printf("[%s:%s:%d] port = %d  \n", 
                //       __FILE__, __FUNCTION__, __LINE__, port);
                mtu[1] = BVLC_ORIGINAL_UNICAST_NPDU;
            } 
            else 
            {
                /* invalid address */
                return -1;
            }
    }
  
    bip_dest.sin_addr.s_addr = address.s_addr;
    bip_dest.sin_port = htons(port);
    
    // printf("[%s:%s:%d] bip_dest.sin_addr = %s and bip_dest.sin_port = %d  \n", 
    //     __FILE__, __FUNCTION__, __LINE__, 
    //     inet_ntoa(bip_dest.sin_addr), port);
    memset(&(bip_dest.sin_zero), '\0', 8);
    mtu_len = 2;
    mtu_len +=
        encode_unsigned16(&mtu[mtu_len],
        (uint16_t) (pdu_len + 4 /*inclusive */ ));
    memcpy(&mtu[mtu_len], pdu, pdu_len);
    mtu_len += pdu_len;
    
    // printf("[%s:%s:%d] BIP_Socket = %d  \n", 
    //     __FILE__, __FUNCTION__, __LINE__, BIP_Socket);

    /* Send the packet */
    bytes_sent =
        sendto(BIP_Socket, (uint8_t *) mtu, mtu_len, 0,
        (struct sockaddr *)(&bip_dest), sizeof(struct sockaddr));

        /////
        if(bytes_sent != mtu_len)
        {
            // close(PORT_BACNET_IP);
            // socket(PORT_BACNET_IP, Sn_MR_UDP, NetConfig.socket[PORT_BACNET_IP], 0); 
            bip_cleanup();
            printf("[%s:%s:%d] bip_cleanup = %d  \n", 
                __FILE__, __FUNCTION__, __LINE__, bytes_sent);
        }
        
    return bytes_sent;
}

/* receives a BACnet/IP packet */
/* returns the number of octets in the PDU, or zero on failure */

uint16_t bip_receive(
    BACNET_ADDRESS * src,       /* source address */
    uint8_t * pdu,      /* PDU data */
    uint16_t max_pdu,   /* amount of space available in the PDU  */
    unsigned timeout)
{       /* number of milliseconds to wait for a packet */
    int received_bytes = 0;
    uint16_t pdu_len = 0;       /* return value */
    struct sockaddr_in sin = { 0 };
    socklen_t sin_len = sizeof(sin);
    uint16_t i = 0;

    /* Make sure the socket is open */
    if (BIP_Socket < 0)
    {
        return 0;
    }

    /* we could just use a non-blocking socket, but that consumes all*/
    // printf("[%s:%s:%d] BIP_Socket = %d \r\n", 
    //                     __FILE__, __FUNCTION__, __LINE__, BIP_Socket);
   
    received_bytes = recvfrom(BIP_Socket, 
        (uint8_t *) &pdu[0], max_pdu, 0, (struct sockaddr *) &sin, &sin_len);
    
    // printf("[%s:%s:%d] recvfrom return %d \r\n", 
    //                         __FILE__, __FUNCTION__, __LINE__, received_bytes);
    /* See if there is a problem */
    if (received_bytes < 0) 
    {
        return 0;
    }

    /* no problem, just no bytes */
    if (received_bytes == 0)
    {
        return 0;
    }
    
    /* the signature of a BACnet/IP packet */
    if (pdu[0] != BVLL_TYPE_BACNET_IP)
    {
        return 0;
    }
    
    if ((pdu[1] == BVLC_ORIGINAL_UNICAST_NPDU) || (pdu[1] == BVLC_ORIGINAL_BROADCAST_NPDU)) 
    {
        /* ignore messages from me */
        if ((sin.sin_addr.s_addr == BIP_Address.s_addr) && (sin.sin_port == BIP_Port)) 
        {
            pdu_len = 0;
            printf("[%s:%s:%d] pdu_len = %d \r\n", 
                                    __FILE__, __FUNCTION__, __LINE__, pdu_len);
        } 
        else 
        {
            /* copy the source address - into host format */
            src->mac_len = 6;
            memcpy(&src->mac[0], &sin.sin_addr.s_addr, 4);
            memcpy(&src->mac[4], &sin.sin_port, 2);
            
            /* FIXME: check destination address */
            /* see if it is broadcast or for us */
            /* decode the length of the PDU - length is inclusive of BVLC */
            (void) decode_unsigned16(&pdu[2], &pdu_len);
            /* subtract off the BVLC header */
            pdu_len -= 4;
            if (pdu_len < max_pdu) 
            {
                /* shift the buffer to return a valid PDU */
                for (i = 0; i < pdu_len; i++) 
                {
                    pdu[i] = pdu[4 + i];
                }
            }
            /* ignore packets that are too large */
            /* clients should check my max-apdu first */
            else 
            {
                pdu_len = 0;
            }
            // printf("[%s:%s:%d] pdu_len = %d \r\n", 
            //                        __FILE__, __FUNCTION__, __LINE__, pdu_len);
        }
    } 
    else 
    {
        if (pdu[1] == BVLC_FORWARDED_NPDU) 
        {
            (void) decode_unsigned32(&pdu[4], (uint32_t *) & sin.sin_addr.s_addr);
            (void) decode_unsigned16(&pdu[8], &sin.sin_port);
            if ((sin.sin_addr.s_addr == htonl(BIP_Address.s_addr)) &&
                (sin.sin_port == htons(BIP_Port))) 
            {
                /* ignore messages from me */
                pdu_len = 0;
            } 
            else 
            {
                /* copy the real source address - into host format */
                src->mac_len = 6;
                memcpy(&src->mac[0], &sin.sin_addr.s_addr, 4);
                memcpy(&src->mac[4], &sin.sin_port, 2);
                /* FIXME: check destination address */
                /* see if it is broadcast or for us */
                /* decode the length of the PDU - length is inclusive of BVLC */
                (void) decode_unsigned16(&pdu[2], &pdu_len);
                /* subtract off the BVLC header */
                pdu_len -= 10;
                if (pdu_len < max_pdu) 
                {
                    /* shift the buffer to return a valid PDU */
                    for (i = 0; i < pdu_len; i++) 
                    {
                        pdu[i] = pdu[4 + 6 + i];
                    }
                } 
                else 
                {
                    /* ignore packets that are too large */
                    /* clients should check my max-apdu first */
                    pdu_len = 0;
                }
            }
            // printf("[%s:%s:%d] pdu_len = %d \r\n", 
            //                         __FILE__, __FUNCTION__, __LINE__, pdu_len);
        } 
        else 
        {
        }
    }
    
    // printf("[%s:%s:%d] pdu_len = %d \r\n", 
    //                         __FILE__, __FUNCTION__, __LINE__, pdu_len);

    return pdu_len;
}


void bip_get_my_address(BACNET_ADDRESS * my_address)
{
    int i = 0;

    my_address->mac_len = 6;
    memcpy(&my_address->mac[0], &BIP_Address.s_addr, 4);
    memcpy(&my_address->mac[4], &BIP_Port, 2);
    my_address->net = 0;        /* local only, no routing */
    my_address->len = 0;        /* no SLEN */
    for (i = 0; i < MAX_MAC_LEN; i++) 
    {
        /* no SADR */
        my_address->adr[i] = 0;
    }

    return;
}


void  bip_get_broadcast_address(BACNET_ADDRESS * dest)
{       /* destination address */
   int i = 0;  /* counter */

    if (dest) {
        dest->mac_len = 6;
        memcpy(&dest->mac[0], &BIP_Broadcast_Address.s_addr, 4);
        memcpy(&dest->mac[4], &BIP_Port, 2);
        // Switch Endian
        char cTemp = dest->mac[4];
        dest->mac[4] = dest->mac[5];
        dest->mac[5] = cTemp;
        
        dest->net = BACNET_BROADCAST_NETWORK;
        dest->len = 0;  /* no SLEN */
        for (i = 0; i < MAX_MAC_LEN; i++) {
            /* no SADR */
            dest->adr[i] = 0;
        }
    }
    return;
}
