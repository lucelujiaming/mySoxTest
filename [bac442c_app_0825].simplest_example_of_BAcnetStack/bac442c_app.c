#include <stdint.h>
#include <stdio.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include <termios.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#include <sys/time.h>
#include <signal.h>

#include <stdbool.h>

#include "datalink.h"

#include "BipTask.h"
#include "device.h"
#include "client.h"

#include "AddrList.h"

#include "handlers.h"
#include "txbuf.h"

extern uint8_t Handler_Transmit_Buffer[MAX_MPDU];

static void* thread_bacnet_operation(void *arg){
    bool i_am= false;
    uint16_t delay_cnt= 0;
    uint16_t len, err_cnt= 0;
    
    BACnet_IP_Init();
    Device_Init();
    // datalink_set("bip");
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
            //     __FILE__, __FUNCTION__, __LINE__);
            len= bip_task();
            if(len != 0)
            {
            //    printf("[%s:%s:%d] bip_task return %d and bip_socket = %d \n", 
            //          __FILE__, __FUNCTION__, __LINE__, len, bip_socket());
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

    return (void*)NULL;
}

//RTU模式的Slave端程序
int main(int argc, char ** argv)
{
    pthread_t bacnet_control_thread;
    Init_AddrInfoList();
    pthread_create(&bacnet_control_thread, NULL, thread_bacnet_operation, NULL);
    
    //5. 循环接受客户端请求，并且响应客户端
    while (1)
    {
        sleep(10);
    }
    return 0;
}

