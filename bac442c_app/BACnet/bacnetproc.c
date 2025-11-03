#include <stdint.h>
#include <string.h>

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

int getLocalIP(char * cLocalIP, int iLocalIPLen)
{
    memset(cLocalIP, 0x00, iLocalIPLen);
    FILE * fp = fopen("/root/app/current_first_ip", "r");
    if(fp < 0)
    {
        printf("open failed: %s\r\n", "/root/app/current_first_ip");
        return 0;
    }
    int nReadLen = fread(cLocalIP, sizeof(char), iLocalIPLen, fp);
    printf("cLocalIP = %s \r\n", cLocalIP);
    fclose(fp);
    return nReadLen;
}

int addMaskToLocalIP(char * cLocalIP, int iLocalIPLen)
{
    char * cLastPoint = strrchr(cLocalIP, '.');
    if(cLastPoint)
    {
        strcpy(cLastPoint + 1, "255");
        printf("cLocalMask = <%s>. \r\n", cLocalIP);
        return cLastPoint - cLocalIP;
    }
    return -1;
}

void BacnetInit()
{
    char cLocalIP[32];
    int nReadLen = getLocalIP(cLocalIP, 32);
    if(nReadLen > 0)
    {
        bip_set_addr(inet_addr(cLocalIP));
        addMaskToLocalIP(cLocalIP, 32);
        bip_set_broadcast_addr(inet_addr(cLocalIP));
    }
    else {
        bip_set_addr(inet_addr("192.168.168.129"));
        bip_set_broadcast_addr(inet_addr("192.168.168.255"));
    }

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
