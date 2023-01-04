#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "inet_sha1.h"


// extern "C" void sedona_sha1(char* input, int inputOff, int len, char* output, int outputOff);

#define MAX_BUFFER_LEN 1024
#define NAME_BUFFER_LEN 128
#define SHA1OUTPUT_BUFFER_LEN 64


int main()
{
    unsigned int i = 0;
    unsigned char strInputBuf[SHA1OUTPUT_BUFFER_LEN];
    unsigned char m_csha1Output[SHA1OUTPUT_BUFFER_LEN];

    printf(" sedona_checkEndian = %d \r\n", sedona_checkEndian());
	
    memset(strInputBuf, 0x00, SHA1OUTPUT_BUFFER_LEN);
    memset(m_csha1Output, 0x00, SHA1OUTPUT_BUFFER_LEN);

	sprintf((char *)strInputBuf, (char *)"admin:");
    sedona_sha1(strInputBuf, 0, 6, m_csha1Output, 0);
    printf("m_csha1Output = ");
    for (i = 0; i < strlen((char *)m_csha1Output); i++)
    {
        printf("%02X ", m_csha1Output[i]);
    }
    printf(" --- m_csha1Output \r\n");
	
    memset(strInputBuf, 0x00, SHA1OUTPUT_BUFFER_LEN);
    // memset(m_csha1Output, 0x00, SHA1OUTPUT_BUFFER_LEN);
	
    // sprintf((char *)strInputBuf, (char *)"\x84\x4e\x3d\x92\xc4\xe1\x80\x07\x8b\x91\x60\x77\x35\x45\x35\x35\x67\x83\x3b\x9e");
    strncpy((char *)strInputBuf, (char *)m_csha1Output, strlen((char *)m_csha1Output));
    // sprintf((char *)strInputBuf + 20, (char *)"\x64\x3c\x98\x69\x66\x33\x48\x73");
    sprintf((char *)strInputBuf + 20, (char *)"\x10\x9c\xf9\x2e\x0d\xed\x72\x63");
    printf("strInputBuf = ");
    for (i = 0; i < strlen((char *)strInputBuf); i++)
    {
        printf("%02X ", strInputBuf[i]);
    }
    printf(" --- strInputBuf \r\n");

        
    sedona_sha1(strInputBuf, 0, 28, m_csha1Output, 0);
    printf("m_csha1Output = ");
    for (i = 0; i < strlen((char *)m_csha1Output); i++)
    {
        printf("%02X ", m_csha1Output[i]);
    }
    printf(" --- m_csha1Output \r\n");
	return 1;
}


