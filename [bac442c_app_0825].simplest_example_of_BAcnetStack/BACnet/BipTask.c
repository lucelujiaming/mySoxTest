
#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "BipTask.h"
#include "handlers.h"
#include "txbuf.h"

static uint8_t BACnet_Recieve_Buffer[MAX_MPDU];

void BACnet_IP_Init(void)
{
        
    bip_set_addr(inet_addr("192.168.168.129"));
    bip_set_broadcast_addr(inet_addr("192.168.168.255"));

    bip_set_dest_port(47808);
    bip_init(NULL);
    
}

uint16_t bip_task(void)
{
    uint16_t pdu_len = 0;
    BACNET_ADDRESS src = { 0 };

    /* returns 0 bytes on timeout */
    pdu_len = bip_receive(&src, (uint8_t*)BACnet_Recieve_Buffer, MAX_MPDU, 1000);
    if(pdu_len != 0)
    {
        // printf("[%s:%s:%d] recvfrom return %d \r\n", 
        //                    __FILE__, __FUNCTION__, __LINE__, pdu_len);
    }
    /* process */
    if (pdu_len > 4) 
    {
        npdu_handler(&src, (uint8_t*)BACnet_Recieve_Buffer, pdu_len);
    }
    
    return pdu_len;
}
 
