#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#ifdef WIN32
#include <windows.h>
#include <time.h>
#include <process.h>
#else
#include <sys/ioctl.h>
#include <linux/types.h>
#include <sys/time.h>
#include <pthread.h>
#endif
#include "board.h"

#define GPIO_IOC_MAGIC   'G'
#define IOCTL_GPIO_SETPINMUX              _IOW(GPIO_IOC_MAGIC, 0, int)
#define IOCTL_GPIO_REVPINMUX              _IOW(GPIO_IOC_MAGIC, 1, int)
#define IOCTL_GPIO_SETVALUE               _IOW(GPIO_IOC_MAGIC, 2, int)
#define IOCTL_GPIO_GETVALUE               _IOR(GPIO_IOC_MAGIC, 3, int)
#define IOCTL_GPIO_GETVALUE_RT            _IOR(GPIO_IOC_MAGIC, 5, int)

#define GPIO_DRV_NAME "/dev/gpio"

#ifdef WIN32
HANDLE ctx_thread;
#else
pthread_t ctx_thread;
#endif
static int is_thread_init = 0;
static int fd = -1;

struct as9260_gpio_arg {
    int port;
    int pin;
    int data;
    int pinmuxback;
};

#define LED_BLINK_DELAY_S 0
#define LED_BLINK_DELAY_US 1000
struct led_table {
    int port;
    int pin;
    int pinmuxback;
    int state;
    unsigned long last_s;
    unsigned long last_us;
};

#define LED_COUNT 4
static struct led_table table[LED_COUNT] = {
        {5, 0, 0, -1, 0, 0},
        {5, 2, 0, -1, 0, 0},
        {5, 1, 0, -1, 0, 0},
        {5, 3, 0, -1, 0, 0},
};

static void led_init(void)
{
    int i;
    int ret;
    struct timeval tv;
    struct as9260_gpio_arg localArg;

    fd = open(GPIO_DRV_NAME, O_RDWR);
    if(fd < 0){
        printf("led open err !\n");
        return;
    }

    gettimeofday(&tv, NULL);

    for(i=0;i<LED_COUNT;i++){
        localArg.port = table[i].port;
        localArg.pin = table[i].pin;
#ifdef WIN32
		ret = 0;
		memset(&localArg, 0x00, sizeof(localArg));
#else
        ret = ioctl(fd, IOCTL_GPIO_SETPINMUX, &localArg);
#endif
        if(ret < 0){
            printf("ioctl error ! \n");
        }
        table[i].pinmuxback = localArg.pinmuxback;

        localArg.data = 0;
#ifdef WIN32
		;
#else
        ret = ioctl(fd, IOCTL_GPIO_SETVALUE, &localArg);
#endif
        if(ret < 0){
            printf("ioctl error ! \n");
        }
        table[i].last_s = tv.tv_sec + 1;
        table[i].last_us = tv.tv_usec;
    }
    is_thread_init = 1;
}

#ifdef WIN32
static unsigned int __stdcall thread_led(void *arg)
#else
static void* thread_led(void* arg)
#endif
{
#ifdef WIN32
	unsigned __int64 tn = 0;
	unsigned __int64 t = 0;
#else
	unsigned long long tn = 0;
	unsigned long long t = 0;
#endif
    int i;
    struct timeval tv;
    struct as9260_gpio_arg localArg;

    while(1){
        gettimeofday(&tv, NULL);
        tn = tv.tv_sec * 1000000 + tv.tv_usec;
        for(i=0;i<LED_COUNT;i++){
            if(table[i].state != 1){
                t = table[i].last_s * 1000000 + table[i].last_us;
                if(tn > t || t - tn > 1000000){
                    localArg.port = table[i].port;
                    localArg.pin = table[i].pin;
                    localArg.data = 1;
#ifdef WIN32
					;
#else
                    ioctl(fd, IOCTL_GPIO_SETVALUE, &localArg);
#endif // WIN32
                    table[i].state = 1;
                }
            }
        }
#ifdef WIN32
        Sleep(1);
#else
        usleep(1000);
#endif // WIN32
    }

#ifndef WIN32
    pthread_exit(NULL);
    return (void*)NULL;
#else
    return (unsigned int)NULL;
#endif
}

void led_blink(int led_idx)
{
    if(is_thread_init == 0){
        led_init();
#ifdef WIN32
		ctx_thread =
			(HANDLE)_beginthreadex(NULL, 0, thread_led, 0, 0, NULL);
#else
        pthread_create(&ctx_thread, NULL, thread_led, 0);
#endif // WIN32
    } else {
        struct as9260_gpio_arg localArg;
        struct timeval tv;

        gettimeofday(&tv, NULL);

        localArg.port = table[led_idx].port;
        localArg.pin = table[led_idx].pin;
        localArg.data = 0;
#ifdef WIN32
					;
#else
        ioctl(fd, IOCTL_GPIO_SETVALUE, &localArg);
#endif // WIN32

        table[led_idx].last_s = tv.tv_sec + LED_BLINK_DELAY_S;
        table[led_idx].last_us = tv.tv_usec + LED_BLINK_DELAY_US;
        table[led_idx].state = 0;
    }
}

void led_on(int led_idx)
{
    struct as9260_gpio_arg localArg;
    localArg.port = table[led_idx].port;
    localArg.pin = table[led_idx].pin;
    localArg.data = 0;
#ifdef WIN32
					;
#else
    ioctl(fd, IOCTL_GPIO_SETVALUE, &localArg);
#endif // WIN32
}

void led_off(int led_idx)
{
    struct as9260_gpio_arg localArg;
    localArg.port = table[led_idx].port;
    localArg.pin = table[led_idx].pin;
    localArg.data = 1;
#ifdef WIN32
					;
#else
    ioctl(fd, IOCTL_GPIO_SETVALUE, &localArg);
#endif // WIN32
}

