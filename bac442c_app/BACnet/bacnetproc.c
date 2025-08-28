#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "bacnetproc.h"
#include "handlers.h"
#include "txbuf.h"
#include "client.h"

static uint8_t BACnet_Recieve_Buffer[MAX_MPDU];

void BacnetInit()
{
    bip_set_addr(inet_addr("192.168.168.129"));
    bip_set_broadcast_addr(inet_addr("192.168.168.255"));

    bip_set_port(47808);
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


void BacnetTask(void)
{
    bool i_am= false;
    uint16_t delay_cnt= 0;
    uint16_t len, err_cnt= 0;

    while (1)
    {
        if(!i_am)
        {
            printf("[%s:%s:%d] i_am = %s  \n", 
                __FILE__, __FUNCTION__, __LINE__, i_am?"true":"false");
            sleep(2);
            
            delay_cnt++;
            printf("[%s:%s:%d] delay_cnt = %d  \n", 
                __FILE__, __FUNCTION__, __LINE__, delay_cnt);
            
            if(delay_cnt >= 4 && (!i_am))
            {
                Send_I_Am((uint8_t *)&Handler_Transmit_Buffer[0]);
                printf("[%s:%s:%d] Send_I_Am \r\n", 
                    __FILE__, __FUNCTION__, __LINE__);
                i_am= true;
            }
        }
        else
        {
            // printf("[%s:%s:%d] Before bip_task \n", 
            //      __FILE__, __FUNCTION__, __LINE__);
            len= bip_task();
            if(len != 0)
            {
                // printf("[%s:%s:%d] bip_task return %d and bip_socket = %d \n", 
                //      __FILE__, __FUNCTION__, __LINE__, len, bip_socket());
            }
            
            if(len < 4 && len)
            {
                printf("[%s:%s:%d] bip_task NG return %d \r\n", 
                            __FILE__, __FUNCTION__, __LINE__, len);
                if(++err_cnt>= 3)
                {
                    bip_cleanup(); 
                    err_cnt= 0;
                }
            }
        }
        // sleep(2);
    }

        
}
