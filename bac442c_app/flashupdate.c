
#include <string.h>
#include "flash.h"
#include "flashupdate.h"
#include "modbus.h"
#include "mb_var_def.h"
#include "common.h"
#include "eeprom_offchip.h"
#include "variables.h"
// #include "bootflag.h"

#define UPDATE_BUFF_MAX	(STM32_PAGE_SIZE*2)

uint8_t FlashBuff[UPDATE_BUFF_MAX];

uint32_t BytesReceiveTotal, BytesWriteTotal;
uint32_t BytesToWrite;

uint32_t Update_GetFileSize(void)
{
	uint32_t file_size;
	
	file_size= _get_uint32_from_uint16((uint16_t*)&HoldingReg[HR_UPD_FILE_SIZE_L], LIT_ENDIAN_DATA);
	
	return file_size;
}

void Update_SetFileSize(uint32_t size)
{
	EEP_Offchip_Save(HR_UPD_FILE_SIZE_L, (uint16_t*)&HoldingReg[HR_UPD_FILE_SIZE_L], 1);
	EEP_Offchip_Reload(HR_UPD_FILE_SIZE_L, (uint16_t*)&HoldingReg[HR_UPD_FILE_SIZE_L], 1);
	
	EEP_Offchip_Save(HR_UPD_FILE_SIZE_H, (uint16_t*)&HoldingReg[HR_UPD_FILE_SIZE_H], 1);
	EEP_Offchip_Reload(HR_UPD_FILE_SIZE_H, (uint16_t*)&HoldingReg[HR_UPD_FILE_SIZE_H], 1);
}

uint16_t Update_GetStatus(void)
{
	return HoldingReg[HR_UPD_STATUS];
}	

void Update_SetStatus(uint16_t status)
{
	if(HoldingReg[HR_UPD_STATUS]!= UPD_ST_ERROR)
		HoldingReg[HR_UPD_STATUS]= status;
}	

uint16_t Update_GetFIleCRC(void)
{
	return HoldingReg[HR_UPD_FILE_CRC];
}	

uint32_t Update_GetTimeStamp(void)
{
	uint32_t stamp;
	
	stamp= _get_uint32_from_uint16((uint16_t*)&HoldingReg[HR_UPD_TIME_STATMP_L], LIT_ENDIAN_DATA);
	
	return stamp;
}

void Update_ResetCommond(void)
{
	HoldingReg[HR_UPD_CMD]= UPD_CMD_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Copy Data To Buffer
uint32_t USR_Flash_FillBuffer(uint8_t* buff, uint32_t ofs, uint16_t len)
{
	if(ofs== 0)	//First Frame
	{
		BytesReceiveTotal= 0;
		BytesWriteTotal= 0;
	}
	
	if(BytesReceiveTotal + len - BytesWriteTotal<= UPDATE_BUFF_MAX && BytesReceiveTotal== ofs)
	{
		memmove(&FlashBuff[BytesReceiveTotal - BytesWriteTotal], buff, len);
		BytesReceiveTotal+= len;
		
		Update_SetStatus(UPD_ST_NONE);
		BytesToWrite= len;
		return len;
	}

	Update_SetStatus(UPD_ST_ERROR); 
	BytesToWrite= 0;
	return 0;
}

// Write Date To Flash
uint32_t USR_Flash_Write(uint16_t cmd) 
{
	uint32_t flash_status= STM_FLASH_SUCCESS;
	uint32_t page_ofs, pages;
	
	switch(cmd)
	{
		case 	UPD_CMD_NONE:
			if(BytesToWrite)
			{
				if(BytesReceiveTotal - BytesWriteTotal > STM32_PAGE_SIZE)
				{
					Update_SetStatus(UPD_ST_BUSY);
					
					page_ofs= BytesWriteTotal/STM32_PAGE_SIZE; 
				
					flash_status= STM_Flash_WritePage(FlashBuff, FLASH_APP_BK_PAGE + page_ofs);
					if(flash_status== STM_FLASH_SUCCESS)
					{
						BytesWriteTotal+= STM32_PAGE_SIZE;
						
						memmove(&FlashBuff[0], &FlashBuff[STM32_PAGE_SIZE], BytesReceiveTotal - BytesWriteTotal);
						
						Update_SetStatus(UPD_ST_SUCCESS);
					}
					else
					{
						Update_SetStatus(UPD_ST_ERROR);
					}
				}
				else
				{
					Update_SetStatus(UPD_ST_SUCCESS);
				}
				
				BytesToWrite= 0;
			}
			break;
		case UPD_CMD_COMPLETE:
			if(BytesReceiveTotal > BytesWriteTotal)
			{
				Update_SetStatus(UPD_ST_BUSY);
				
				memset(&FlashBuff[BytesReceiveTotal - BytesWriteTotal], 0xFF, STM32_PAGE_SIZE - (BytesReceiveTotal - BytesWriteTotal));	//Fill Buffer
				page_ofs= BytesWriteTotal/STM32_PAGE_SIZE; 
				
				flash_status= STM_Flash_WritePage(FlashBuff, FLASH_APP_BK_PAGE + page_ofs);
				
				if(flash_status== STM_FLASH_SUCCESS)
				{
					BytesWriteTotal= BytesReceiveTotal;
				}
				else
				{
					Update_SetStatus(UPD_ST_ERROR);
					return 0;
				}
			}
			
			if(Update_GetFileSize()== BytesReceiveTotal
				&& Update_GetStatus()!= UPD_ST_ERROR)
			{
				Update_SetFileSize(BytesWriteTotal);
				Update_SetStatus(UPD_ST_SUCCESS);
			}
			else
			{
				Update_SetFileSize(0);
				Update_SetStatus(UPD_ST_ERROR);
				return 0;
			}
			break;
		case UPD_CMD_FORMAT_FLASH:
			Update_SetStatus(UPD_ST_BUSY);
		
			pages= FLASH_APP_BK_PAGE- FLASH_APP_PAGE; 
			
			flash_status= Flash_EarasePages(FLASH_APP_BK_PAGE, pages);
			if(flash_status== STM_FLASH_SUCCESS)
			{
				Update_SetFileSize(0);
				Update_SetStatus(UPD_ST_NONE);
			}
			else
			{
				Update_SetStatus(UPD_ST_ERROR);
			}
			break;
		case UPD_CMD_EARASE_CONFIG:
			EEP_Offchip_WriteWord(EEP_ADDR_CONFIG_SIG, 0xFFFF);
			Update_ResetCommond();
			break;
		case UPD_CMD_EARASE_DEVICE:
			EEP_Offchip_WriteWord(EEP_ADDR_DEVICE_SIG, 0xFFFF);
			Update_ResetCommond();
			break;
		case UPD_CMD_EARASE_BACNET:
			EEP_Offchip_WriteWord(EEP_ADDR_BACNET_SIG, 0xFFFF);
			Update_ResetCommond();
			break;
		case UPD_CMD_EARASE_UPDATE:
			EEP_Offchip_WriteWord(EEP_ADDR_UPDATE_SIG, 0xFFFF);
			Update_ResetCommond();
			break;
	}

	return 0;
}


