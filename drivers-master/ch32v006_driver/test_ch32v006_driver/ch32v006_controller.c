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
    int file_pos        = 18;    // 自适应指令
    int file_content    = 0x01;  // 1：开始
    int file_length     = 0x01;  // uint8_t
    char cData[4];
    int iRet = 0, iWriteLen = 0;
    fd = open(CH32V006_PATH, O_RDWR);
    if (fd == -1)
    {
		printf("fopen %s failed\n", CH32V006_PATH);
        return -1;
    }
    
    if(argc == 4)
    {
        file_pos          = atoi(argv[1]);
        file_content      = atoi(argv[2]);
        file_length       = atoi(argv[3]);
        if((file_length != 1) && (file_length != 2))
        {
            printf("file_length error with %d.\n", file_length);
            return -1;
        }
        else if((file_length == 1) && (file_content >= 0xFF))
        {
            printf("file_content error with %d when file_length is 1.\n", file_content);
            return -1;
        }
        else if((file_length == 2) && (file_content >= 0xFFFF))
        {
            printf("file_content error with %d when file_length is 2.\n", file_content);
            return -1;
        }
    }
    // Setting opening
    iRet = lseek(fd, file_pos, SEEK_SET);
    printf("lseek return  %d with 12\n", iRet);
    memset(cData, 0x00, 4);
    if(file_length == 2)
    {
        cData[0] = file_content / 256;
        cData[1] = file_content % 256;
        iWriteLen = write(fd, cData, 2);
    }
    else if(file_length == 1)
    {
        cData[0] = file_content;
        iWriteLen = write(fd, cData, 1);
    }
    printf("white return  %d\n", iWriteLen);
    close(fd);
    return 0;
}
