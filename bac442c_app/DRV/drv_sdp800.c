#include <stdint.h>
#include <stdio.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "drv_sdp800.h"
#include "common.h"

#include "hal_i2c1.h"


typedef struct sdp8xx_data_t {
    int16_t lc_press;
    int16_t lc_temp;
    int16_t scaleFactor;
} sdp8xx_data;


void swap_int16_endian(char * cData)
{
    char cSwap;
    
    cSwap = cData[0];
    cData[0] = cData[1];
    cData[1] = cSwap;
}

void DRV_SDP800_Init(uint8_t i2cAddress){
}


int g_SDP800_fd = 0 ;

#define SDP8XX_PATH    "/dev/sdp8xx"
Error DRV_SDP800_StartMeasurement(
        Sdp800TempComp   tempComp,  
        Sdp800Averaging  averaging)
{
    int ioctl_code = 0;
    g_SDP800_fd = open(SDP8XX_PATH, O_RDONLY);
    if (g_SDP800_fd == -1)
    {
        printf("fopen %s failed\n", SDP8XX_PATH);
        return ERROR_IVALID_PARAMETER;
    }
    printf("[%s:%s:%d] DRV_SDP800_StartMeasurement  \n", 
                __FILE__, __FUNCTION__, __LINE__);
    switch(tempComp) 
    {
     case SDP800_TEMPCOMP_MASS_FLOW:
        switch(averaging) 
        {
        case SDP800_AVERAGING_TILL_READ:
          ioctl_code = 12;
          break;
        case SDP800_AVERAGING_NONE:
          ioctl_code = 11;
          break;
        }
        break;
     case SDP800_TEMPCOMP_DIFFERNTIAL_PRESSURE:
        switch(averaging) 
        {
        case SDP800_AVERAGING_TILL_READ:
          ioctl_code = 22;
          break;
        case SDP800_AVERAGING_NONE:
          ioctl_code = 21;
          break;
        }
        break;
    }

    if(ioctl_code != 0)
    { 
        printf("\n ioctl_code = %d\n", ioctl_code);
        if (ioctl(g_SDP800_fd, ioctl_code, NULL) == -1) {
            perror("ioctl failed");
            return -1;
        }
    }
    else {
        printf("tempComp and averaging error\n");
        close(g_SDP800_fd);
        return ERROR_IVALID_PARAMETER;
    }
    return ERROR_NONE;
}

Error DRV_SDP800_StopMeasurement(void)
{
    if(g_SDP800_fd)
    {
        close(g_SDP800_fd);
        g_SDP800_fd = 0;
    }
    return ERROR_NONE;
}

static const float scaleFactorTemperature = 200;
Error DRV_SDP800_ReadResults(float* diffPressure, float* temperature)
{
    Error error= ERROR_NONE;
    
    char cData[12];
    sdp8xx_data objSDP8xx ;
    
    int iLen = read(g_SDP800_fd, cData, 6);
    // printf("fread return  %d\n", iLen);
    // for(int i = 0 ; i < iLen; i++)
    // {
    //     printf("<%02X> ", cData[i]);
    // }
    // printf("\nEnd of fread and len is %d\n", iLen);
    memcpy(&objSDP8xx, cData, iLen);
    *diffPressure = (float)(objSDP8xx.lc_press) / (float)(objSDP8xx.scaleFactor);
    *temperature  = (float)(objSDP8xx.lc_temp) / scaleFactorTemperature;
    // printf("lc_press = %f, lc_temp = %f. \r\n", 
    //                    *diffPressure, *temperature);

    return error;
}

Error DRV_SDP800_SoftReset(void)
{
  Error error= ERROR_NONE;
  return error;
}

