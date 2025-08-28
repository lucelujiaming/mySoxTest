/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : fsmc_nand.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : This file provides a set of functions needed to drive the
*                      NAND512W3A2 memory mounted on STM3210E-EVAL board.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "flash.h"
#include "common.h"

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************
* Function Name  : STM_Flash_WritePage
* Description    : This routine is for writing one or several 512 Bytes Page size.
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
uint32_t STM_Flash_WritePage(
	uint8_t *buf,		//Buffer 
	uint32_t ofs 	//Page Offset to Write/Start Page
)
{

	return STM_FLASH_SUCCESS;
}

/******************************************************************************
* Function Name  : STM_Flash_ReadPage
* Description    : This routine is for sequential read from one or several
* Input          : - buf: pointer on the Buffer to fill  
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*******************************************************************************/
uint32_t STM_Flash_ReadPage(
	uint8_t *buf, 		//buffer
	uint32_t ofs 		//page offset 
)
{
	return STM_FLASH_SUCCESS;
}

uint32_t Flash_EarasePages(uint32_t pg_ofs, uint32_t pgs) 
{

		return STM_FLASH_SUCCESS;
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
