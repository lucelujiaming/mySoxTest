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

int main(int argc, char ** argv)
{    
    int fd ;
    int opening        = 700;
    int adaper_command = 0x01;
    int rotate_direction = 0x01;
    char cData[64];
    int iRet = 0, iWriteLen = 0;
    fd = open(CH32V006_PATH, O_RDWR);
    if (fd == -1)
    {
		printf("fopen %s failed\n", CH32V006_PATH);
        return -1;
    }
    
    if(argc == 4)
    {
        opening          = atoi(argv[1]);
        if(opening > 1000) {
            opening = 1000;
        }
        adaper_command   = atoi(argv[2]);
        rotate_direction = atoi(argv[3]);
    }
    iRet = lseek(fd, 0, SEEK_END);
    printf("File size is %d\n", iRet);
    // Setting opening
    iRet = lseek(fd, -9, SEEK_END);
    printf("lseek return  %d with 12\n", iRet);
    memset(cData, 0x00, 64);
    cData[0] = opening / 256;
    cData[1] = opening % 256;
    iWriteLen = write(fd, cData, 2);
    printf("white return  %d\n", iWriteLen);
    
    // Setting command
    iRet = lseek(fd, -3, SEEK_END);
    printf("lseek return  %d with 18\n", iRet);
    memset(cData, 0x00, 64);
    cData[0] = adaper_command;
    cData[1] = rotate_direction;
    iWriteLen = write(fd, cData, 2);
    printf("white return  %d\n", iWriteLen);
    close(fd);
    return 0;
}
