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
#include <sys/stat.h>

#define CH32V006_PATH "/dev/ch32v006"

typedef struct ch32v006Data_t {
    uint16_t intOpeningFeedback;   // 0-1	开度反馈	0-1000
    uint16_t intAnalogInputOne;    // 2	    模拟输入1	0-4095
    uint16_t intAnalogInputTwo;    // 4	    模拟输入2	0-4095
    uint16_t intAnalogInputThree;  // 6	    模拟输入3	0-4095
    uint16_t intAnalogInputFour;   // 8	    模拟输入4	0-4095
    uint8_t  intStatus;            // 10	状态	    bit 0：逆时针堵转报警
                                   // 		            bit 1：顺时针堵转报警
                                   // 		            bit 2：手动状态
                                   // 		            bit 3：自适应状态
                                   // 	 	            bit 4：CCW
    uint8_t  intPowerDeviation;    // 11	电源偏差	x0.001
    uint16_t intOpeningcommand;    // 12	开度指令	0-1000
    uint16_t intAnalogOutputOne;   // 14	模拟输出1 	0-1000
    uint16_t intAnalogOutputTwo;   // 16	模拟输出2 	0-1000
    uint8_t  intAdaptiveCommand;   // 18	自适应指令	0：完成，1：开始
    uint8_t  intRotationCommand;   // 19	正反转指令	0：CW，1：CCW
    uint8_t  intReservedOne;       // 20	保留
    uint8_t  intReservedTwo;       // 21	保留
}ch32v006Data;


void swap_int16_endian(char * cData)
{
    char cSwap;
    
    cSwap = cData[0];
    cData[0] = cData[1];
    cData[1] = cSwap;
}

int main(int argc, char ** argv)
{    
    int fd ;
    ch32v006Data structData;
    int iReadLen = 0;
    fd = open(CH32V006_PATH, O_RDWR);
    if (fd == -1)
    {
		printf("fopen %s failed\n", CH32V006_PATH);
        return -1;
    }
    memset(&structData, 0x00, sizeof(ch32v006Data));
	iReadLen = read(fd, &structData, sizeof(structData));
    if(iReadLen > 0)
    {
        printf("structData return: \n");
        swap_int16_endian((char *)&structData.intOpeningFeedback);
        printf("\t 开度反馈 = <%d>.\n", structData.intOpeningFeedback);
        swap_int16_endian((char *)&structData.intAnalogInputOne);
        printf("\t 模拟输入1 = <%d>.\n", structData.intAnalogInputOne);
        swap_int16_endian((char *)&structData.intAnalogInputTwo);
        printf("\t 模拟输入2 = <%d>.\n", structData.intAnalogInputTwo);
        swap_int16_endian((char *)&structData.intAnalogInputThree);
        printf("\t 模拟输入3 = <%d>.\n", structData.intAnalogInputThree);
        swap_int16_endian((char *)&structData.intAnalogInputFour);
        printf("\t 模拟输入4 = <%d>.\n", structData.intAnalogInputFour);
        printf("\t 状态       = <%d>.\n", structData.intStatus);
        printf("\t 电源偏差 = <%d>.\n" , structData.intPowerDeviation);
        swap_int16_endian((char *)&structData.intOpeningcommand);
        printf("\t 开度指令 = <%d>.\n",  structData.intOpeningcommand); 
        swap_int16_endian((char *)&structData.intAnalogOutputOne);
        printf("\t 模拟输出1 = <%d>.\n",  structData.intAnalogOutputOne);
        swap_int16_endian((char *)&structData.intAnalogOutputTwo);
        printf("\t 模拟输出2 = <%d>.\n",  structData.intAnalogOutputTwo);
        printf("\t 自适应指令 = <%d>.\n", structData.intAdaptiveCommand);
        printf("\t 正反转指令 = <%d>.\n", structData.intRotationCommand);
        printf("\t 保留 = <%d>.\n", structData.intReservedOne);   
        printf("\t 保留 = <%d>.\n", structData.intReservedTwo);
    }
    close(fd);
    return 0;
}
