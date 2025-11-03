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
#include <time.h>

#include <stdbool.h>

#include "modbus_def.h"
#include "vav.h"

#include "datalink.h"

#include "device.h"

#include "hal_usart1_dma.h"

#include "task_config.h"
#include "hal_usart1_dma.h"
#include "hal_usart3_dma.h"

#include "bacnetproc.h"

#include "handlers.h"
#include "modbus.h"
#include "mb_var_def.h"
#include "actuator.h"

// extern struct mstp_port_struct_t MSTP_Port;


static void* taskControl(void *arg){
    while (1)
    {
        Task_Control();
    }

    return (void*)NULL;
}

static void* taskActuator(void *arg){
    while (1)
    {
        Task_Actuator();
    }

    return (void*)NULL;
}


/***********************************************************************
 * 函数名：open_and_new_rtu_slave
 *   功能：打开液晶屏的端口。
 * 入口参数： 无。
 *   返回值： 返回端口文件句柄。
 ***********************************************************************/
int open_lcd_port()
{
    // char cDevicePath[20];
    
    int   lcd_fd; // , send_res;

    lcd_fd = open("/dev/ttyS2", O_RDWR|O_NOCTTY/*|O_NDELAY*/);
    printf("lcd open...\n");
    printf("lcd open return %d\n", lcd_fd);
    if (lcd_fd == -1) {
        printf("INSTRUMENT_UART_DEVICE open error");
        exit(1);
    }
    return lcd_fd;
}


static void* taskModbus(void *arg){
    int   lcd_fd = 0; // , send_res;
    lcd_fd = open_lcd_port();
    HAL_USART3_DMA_Configuration(lcd_fd, 38400, 0, 0, 256);
    while (1)
    {
        Task_Modbus(lcd_fd);
    }

    return (void*)NULL;
}



//   static void* taskMSTPRcv(void *arg){
//       while (1)
//       {
//           if (MSTP_Port.ReceivedValidFrame == false
//               && MSTP_Port.ReceivedInvalidFrame == false)
//           {
//                   MSTP_Receive_Frame_FSM(&MSTP_Port);
//           }
//
//       }
//
//       return (void*)NULL;
//   }
//
//   static void* taskMSTP(void *arg){
//       // BACnet_IP_Init();
//       // Device_Init();
//       // datalink_set("bip");
//       while (1)
//       {
//           Task_MSTP();
//       }
//
//       return (void*)NULL;
//   }

static void* taskBacnet(void *arg){
    Device_Init();
    // datalink_set("bip");
    while (1)
    {
        BacnetTask();
        // sleep(2);
    }

    return (void*)NULL;
}


//RTU模式的Slave端程序
int main(int argc, char ** argv)
{
    // pthread_t task_mstp_thread;
    // pthread_t task_mstp_recieve_thread;
    pthread_t task_bacnet_thread;
    pthread_t task_modbus_thread;
    pthread_t task_actuator_thread;
    pthread_t task_control_thread;
    
    BSP_Init();
    // pthread_create(&task_mstp_thread, NULL, taskMSTP, NULL);
    // pthread_create(&task_mstp_recieve_thread, NULL, taskMSTPRcv, NULL);
    pthread_create(&task_bacnet_thread, NULL, taskBacnet, NULL);
    pthread_create(&task_modbus_thread, NULL, taskModbus, NULL);
    pthread_create(&task_actuator_thread, NULL, taskActuator, NULL);
    pthread_create(&task_control_thread, NULL, taskControl, NULL);
    
    //5. 循环接受客户端请求，并且响应客户端
    while (1)
    {
        sleep(10);
    }
    return 0;
}

