#include <stdint.h>
#include <stdio.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define SDP8XX_PATH    "/dev/sdp8xx"

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

static const float scaleFactorTemperature = 200;
int main(int argc, char ** argv)
{
    FILE* fpr ;
    char cData[12];
    float diffPressure = 0.0;
    float temperature = 0.0;
    
    memset(cData, 0x00, 12);
    sdp8xx_data objSDP8xx ;
    if (NULL == (fpr = fopen(SDP8XX_PATH, "rb")))
    {
		printf("fopen %s failed\n", SDP8XX_PATH);
        return -1;
    }
    int iLen = fread(cData, 1, 6, fpr);
    printf("fread return  %d\n", iLen);
    for(int i = 0 ; i < iLen; i++)
    {
     	printf("<%02X> ", cData[i]);
    }
    printf("\nEnd of fread and len is %d\n", iLen);
    memcpy(&objSDP8xx, cData, iLen);
    diffPressure = (float)(objSDP8xx.lc_press) / (float)(objSDP8xx.scaleFactor);
    temperature  = (float)(objSDP8xx.lc_temp) / scaleFactorTemperature;
    printf("lc_press = %f, lc_temp = %f. \r\n", 
            diffPressure, temperature);
    
    // swap_int16_endian(cData);
    // swap_int16_endian(cData + 2);
    // swap_int16_endian(cData + 3);
    // memcpy(&objSDP8xx, cData, iLen);
    // diffPressure = (float)(objSDP8xx.lc_press) / (float)(objSDP8xx.scaleFactor);
    // temperature  = (float)(objSDP8xx.lc_temp) / scaleFactorTemperature;
    // printf("lc_press = %f, lc_temp = %f. \r\n", 
    //         diffPressure, temperature);
    fclose(fpr);
    return 0;
}

