//peak.c
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "protocol.h"
#include "usart.h"
#include "usart_type.h"
#include "peak.h"

static uint16_t req_addr;

void PEAK_Init(int fd, uint16_t addr)
{
//	USART3_SetRcvMode(USART_RCV_CHAR,0x02,0x03);
	Instrument_USART_SetRcvMode(USART_RCV_CHAR,0x02,0x03);
//	USART3_Configuration(USART_BAUD(9600), PARITY_NONE, STB_1);
    set_speed(fd, 9600);
	if (set_parity(fd, 8, 1, 'N') == SERIAL_FALSE)  {
		printf("Set Parity Error\n");
		exit (0);
	}
	req_addr= addr;
}

uint16_t PEAK_Request(void)
{
	uint8_t len;
	
	len= 0;
	if(req_addr)
	{
		serial_buff[0]= req_addr/100+'0';
		serial_buff[1]= (req_addr%100)/10+'0';
		serial_buff[2]= req_addr%10+'0';
		
		len= 3;
	}
	
	return len;
}

//Concentration Unit:PPB
uint8_t PEAK_Analysis(uint16_t len)
{
	uint8_t data_pos;
	uint8_t i,j,str_size;
	uint16_t data;
	uint8_t err;
	float f_value;
	uint32_t u32_value;
	uint8_t* str_ptr;
	
	err= 1;
	i= 0;
	j= 0;
	data_pos= 0;
	
	//Find position after 3 ','
	while(data_pos < len)
	{
		if(*(serial_buff + data_pos++)==',')
		{
			if(++j>= 3)
			{
				break;
			}
		}
	}
	
	//Get General Error
	str_size= 0;
	for(j= 0;j< len- data_pos;j++)
	{
		if(*(serial_buff + data_pos + j)== ',')
		{
			break;
		}
		str_size++;
	}

	if(str_size)
	{
		memset(data_temp,0,20);
		strncpy((char*)data_temp,(const char*)(serial_buff+data_pos),str_size);
		data= atoi(data_temp);
		
		pthread_rwlock_wrlock(&ireg_rwlock); // 获取IReg的写锁
		protocol_buff[1]= data;
    	pthread_rwlock_unlock(&ireg_rwlock); // 释放IReg的写锁
		data_pos+= str_size + 1;
	}
	else
	{
		data_pos++;
	}
	
	//Get Stream Number
	str_size= 0;
	for(j= 0;j< len-data_pos;j++)
	{
		if(*(serial_buff+data_pos+j)== ',')
		{
			break;
		}
		str_size++;
	}

	if(str_size)
	{
		memset(data_temp,0,20);
		strncpy((char*)data_temp,(const char*)(serial_buff+data_pos),str_size);
		data= atoi(data_temp);
		
		pthread_rwlock_wrlock(&ireg_rwlock); // 获取IReg的写锁
		protocol_buff[2]= data;
    	pthread_rwlock_unlock(&ireg_rwlock); // 释放IReg的写锁
		data_pos+= str_size + 1;
	}
	else
	{
		data_pos++;
	}
	
	//Get Name , Area and Concent
	for(i= 0;i< 4;i++)
	{
		//Get Name
		str_size= 0;
		for(j= 0;j< len- data_pos;j++)
		{
			if(*(serial_buff+data_pos+j)== ',')
			{
				break;
			}
			str_size++;
		}
		
		if(str_size)
		{
    		pthread_rwlock_rdlock(&ireg_rwlock); // 获取IReg的读锁
			str_ptr= (uint8_t*)(&protocol_buff[3 + i* 8]);
    		pthread_rwlock_unlock(&ireg_rwlock); // 释放IReg的读锁
			for(j= 0;j< str_size;j++)
			{
				str_ptr[j]=  *(serial_buff + data_pos + j);
			}
			data_pos+= str_size + 1;
		}
		else
		{
			data_pos++;
		}
		//Get Area
		str_size= 0;
		for(j= 0;j< len- data_pos;j++)
		{
			if(*(serial_buff + data_pos + j)== ',')
			{
				break;
			}
			str_size++;
		}
		
		if(str_size)
		{
			for(j= 0;j< str_size;j++)
			{
				memset(data_temp,0,20);
				strncpy((char*)data_temp,(const char*)(serial_buff+data_pos),str_size);
				
				f_value= atof(data_temp);
				u32_value= real_to_u32(f_value);
				
				pthread_rwlock_wrlock(&ireg_rwlock); // 获取IReg的写锁
				if(little_endian)
				{
					protocol_buff[7 + 8 * i]= _low_word_int32(u32_value);
					protocol_buff[8 + 8 * i]= _high_word_int32(u32_value);
				}
				else
				{
					protocol_buff[7 + 8 * i]= _high_word_int32(u32_value);
					protocol_buff[8 + 8 * i]= _low_word_int32(u32_value);
				}
   				pthread_rwlock_unlock(&ireg_rwlock); // 释放IReg的写锁
			}
			data_pos+= str_size + 1;
		}
		else
		{
			data_pos++;
		}
		
		//Get Concent
		str_size= 0;
		for(j= 0;j< len-data_pos;j++)
		{
			if(*(serial_buff + data_pos + j)== ',')
			{
				break;
			}
			str_size++;
		}
		
		if(str_size)
		{
			for(j= 0;j< str_size;j++)
			{
				memset(data_temp,0,20);
				strncpy((char*)data_temp,(const char*)(serial_buff + data_pos),str_size);
				
				f_value= atof(data_temp);
				u32_value= real_to_u32(f_value);
				
				pthread_rwlock_wrlock(&ireg_rwlock); // 获取IReg的写锁
				if(little_endian)
				{
					protocol_buff[9 + 8 * i]= _low_word_int32(u32_value);
					protocol_buff[10 + 8 * i]= _high_word_int32(u32_value);
				}
				else
				{
					protocol_buff[9 + 8 * i]= _high_word_int32(u32_value);
					protocol_buff[10 + 8 * i]= _low_word_int32(u32_value);
				}
   				pthread_rwlock_unlock(&ireg_rwlock); // 释放IReg的写锁
			}
			data_pos+= str_size + 1;
		}
		else
		{
			data_pos++;
		}
	}
	
	err= 0;

	return err;
}

/*
    PEAKS/P9000: RCP, FID, HDID
    寄存器     类型   功能描述
    30001       Int	    Communication Fault
    30002       Int	    Alarm	
    30003       Int	    Stream Number / GAS1	
    30004       Int	    Name / GAS1	
    30008       Real    Area / GAS1	
    30010       Real    Concent / GAS1	
    30012       Int	    Name / GAS2	
    30016       Real    Area / GAS2	
    30018       Real    Concent / GAS2	
    30020       Int	    Name / GAS3	
    30024       Real    Area / GAS3	
    30026       Real    Concent / GAS3	
    30028       Int	    Name / GAS4	
    30032       Real    Area / GAS4	
    30034       Real    Concent / GAS4	
 */
uint16_t PEAK_DataOutput(char* strOutput)
{
    char cAreaBuffer[4] = {0};
    char cConcentBuffer[4] = {0};
    
	float * floatAreaBufferPtr      = (float *)(cAreaBuffer);
	float * floatConcentBufferPtr   = (float *)(cConcentBuffer);
    
    float  floatArea[4], floatConcent[4];
    
    pthread_rwlock_rdlock(&ireg_rwlock); // 获取IReg的读锁
	if(little_endian)
	{
        // Area / GAS1	
		cAreaBuffer[3] = protocol_buff[8]>>8;
		cAreaBuffer[2] = protocol_buff[8]&0x00FF;
		cAreaBuffer[1] = protocol_buff[7]>>8;
		cAreaBuffer[0] = protocol_buff[7]&0x00FF;
        floatArea[0] = *floatAreaBufferPtr;
        // Concent / GAS1
		cConcentBuffer[3] = protocol_buff[10]>>8;
		cConcentBuffer[2] = protocol_buff[10]&0x00FF;
		cConcentBuffer[1] = protocol_buff[9]>>8;
		cConcentBuffer[0] = protocol_buff[9]&0x00FF;
        floatConcent[0] = *floatConcentBufferPtr;
        
        // Area / GAS2
		cAreaBuffer[3] = protocol_buff[16]>>8;
		cAreaBuffer[2] = protocol_buff[16]&0x00FF;
		cAreaBuffer[1] = protocol_buff[15]>>8;
		cAreaBuffer[0] = protocol_buff[15]&0x00FF;
        floatArea[1] = *floatAreaBufferPtr;
        // Concent / GAS2
		cConcentBuffer[3] = protocol_buff[18]>>8;
		cConcentBuffer[2] = protocol_buff[18]&0x00FF;
		cConcentBuffer[1] = protocol_buff[17]>>8;
		cConcentBuffer[0] = protocol_buff[17]&0x00FF;
        floatConcent[1] = *floatConcentBufferPtr;
        
        // Area / GAS3
		cAreaBuffer[3] = protocol_buff[24]>>8;
		cAreaBuffer[2] = protocol_buff[24]&0x00FF;
		cAreaBuffer[1] = protocol_buff[23]>>8;
		cAreaBuffer[0] = protocol_buff[23]&0x00FF;
        floatArea[2] = *floatAreaBufferPtr;
        // Concent / GAS3
		cConcentBuffer[3] = protocol_buff[26]>>8;
		cConcentBuffer[2] = protocol_buff[26]&0x00FF;
		cConcentBuffer[1] = protocol_buff[25]>>8;
		cConcentBuffer[0] = protocol_buff[25]&0x00FF;
        floatConcent[2] = *floatConcentBufferPtr;
        
        // Area / GAS4
		cAreaBuffer[3] = protocol_buff[32]>>8;
		cAreaBuffer[2] = protocol_buff[32]&0x00FF;
		cAreaBuffer[1] = protocol_buff[31]>>8;
		cAreaBuffer[0] = protocol_buff[31]&0x00FF;
        floatArea[3] = *floatAreaBufferPtr;
        // Concent / GAS4
		cConcentBuffer[3] = protocol_buff[34]>>8;
		cConcentBuffer[2] = protocol_buff[34]&0x00FF;
		cConcentBuffer[1] = protocol_buff[33]>>8;
		cConcentBuffer[0] = protocol_buff[33]&0x00FF;
        floatConcent[3] = *floatConcentBufferPtr;
    }
    else
	{
        // Area / GAS1	
		cAreaBuffer[3] = protocol_buff[7]>>8;
		cAreaBuffer[2] = protocol_buff[7]&0x00FF;
		cAreaBuffer[1] = protocol_buff[8]>>8;
		cAreaBuffer[0] = protocol_buff[8]&0x00FF;
        floatArea[0] = *floatAreaBufferPtr;
        // Concent / GAS1
		cConcentBuffer[3] = protocol_buff[9]>>8;
		cConcentBuffer[2] = protocol_buff[9]&0x00FF;
		cConcentBuffer[1] = protocol_buff[10]>>8;
		cConcentBuffer[0] = protocol_buff[10]&0x00FF;
        floatConcent[0] = *floatConcentBufferPtr;
        
        // Area / GAS2
		cAreaBuffer[3] = protocol_buff[15]>>8;
		cAreaBuffer[2] = protocol_buff[15]&0x00FF;
		cAreaBuffer[1] = protocol_buff[16]>>8;
		cAreaBuffer[0] = protocol_buff[16]&0x00FF;
        floatArea[1] = *floatAreaBufferPtr;
        // Concent / GAS2
		cConcentBuffer[3] = protocol_buff[17]>>8;
		cConcentBuffer[2] = protocol_buff[17]&0x00FF;
		cConcentBuffer[1] = protocol_buff[18]>>8;
		cConcentBuffer[0] = protocol_buff[18]&0x00FF;
        floatConcent[1] = *floatConcentBufferPtr;
        
        // Area / GAS3
		cAreaBuffer[3] = protocol_buff[23]>>8;
		cAreaBuffer[2] = protocol_buff[23]&0x00FF;
		cAreaBuffer[1] = protocol_buff[24]>>8;
		cAreaBuffer[0] = protocol_buff[24]&0x00FF;
        floatArea[2] = *floatAreaBufferPtr;
        // Concent / GAS3
		cConcentBuffer[3] = protocol_buff[25]>>8;
		cConcentBuffer[2] = protocol_buff[25]&0x00FF;
		cConcentBuffer[1] = protocol_buff[26]>>8;
		cConcentBuffer[0] = protocol_buff[26]&0x00FF;
        floatConcent[2] = *floatConcentBufferPtr;
        
        // Area / GAS4
		cAreaBuffer[3] = protocol_buff[31]>>8;
		cAreaBuffer[2] = protocol_buff[31]&0x00FF;
		cAreaBuffer[1] = protocol_buff[32]>>8;
		cAreaBuffer[0] = protocol_buff[32]&0x00FF;
        floatArea[3] = *floatAreaBufferPtr;
        // Concent / GAS4
		cConcentBuffer[3] = protocol_buff[33]>>8;
		cConcentBuffer[2] = protocol_buff[33]&0x00FF;
		cConcentBuffer[1] = protocol_buff[34]>>8;
		cConcentBuffer[0] = protocol_buff[34]&0x00FF;
        floatConcent[3] = *floatConcentBufferPtr;
    }
    pthread_rwlock_unlock(&ireg_rwlock); // 释放IReg的读锁
    
    sprintf(strOutput, "%f,%f,%f,%f,%f,%f,%f,%f,%.1f,%.1f",
          floatArea[0],       // Real    Area / GAS1	
          floatConcent[0],    // Real    Concent / GAS1	
          floatArea[1],       // Real    Area / GAS2	
          floatConcent[1],    // Real    Concent / GAS2	
          floatArea[2],       // Real    Area / GAS3	
          floatConcent[2],    // Real    Concent / GAS3	
          floatArea[3],       // Real    Area / GAS4	
          floatConcent[3],    // Real    Concent / GAS4	
          0.0, 0.0);
        
	return 0;
}

uint16_t PEAK_DataColumnsNumber()
{
	return 8;
}

