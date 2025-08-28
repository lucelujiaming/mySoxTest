/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : fsmc_nand.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Header for fsmc_nand.c file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FSMC_NAND_H
#define __FSMC_NAND_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define BUFFER_SIZE			STM_FLASH_PAGE_SIZE

/* STM Flash memory address of physics */
#define STM32_FLASH_ADDR		((uint32_t)0x08000000)

/* Define the STM32F10Xxx Flash page size depending on the used STM32 device */
#if defined (STM32F10X_LD) || defined (STM32F10X_MD)
  #define STM32_PAGE_SIZE  (uint32_t)0x400  /* Page size = 1KByte */
#elif defined (STM32F10X_HD) || defined (STM32F10X_XL)
  #define STM32_PAGE_SIZE  (uint32_t)0x800  /* Page size = 2KByte */
#else
  #define STM32_PAGE_SIZE  (uint32_t)0x800  /* Page size = 2KByte */
#endif

#define FLASH_APP_PAGE 			((uint32_t)0x0A)	// 64K for Bootloader,0x08005000
#define FLASH_APP_BK_PAGE 	((uint32_t)0x44)	// 256K,0x08022000

#define FLASH_APP_START_ADDRESS (STM32_FLASH_ADDR + FLASH_APP_PAGE*STM32_PAGE_SIZE)

/* NAND memory status */
#define STM_FLASH_VALID_ADDRESS		((uint32_t)0x00010000)
#define STM_FLASH_INVALID_ADDRESS	((uint32_t)0x00020000)
#define STM_FLASH_ERROR			((uint32_t)0x00040000)
#define STM_FLASH_SUCCESS		((uint32_t)0x00080000)

/* STM32 Flash memory data computation */
#define DATA_1st_CYCLE(DATA)       (uint8_t)((DATA)& 0xFF)			/* 1st data cycle */
#define DATA_2nd_CYCLE(DATA)       (uint8_t)(((DATA)& 0xFF00) >> 8)		/* 2nd data cycle */
#define DATA_3rd_CYCLE(DATA)       (uint8_t)(((DATA)& 0xFF0000) >> 16)	/* 3rd data cycle */
#define DATA_4th_CYCLE(DATA)       (uint8_t)(((DATA)& 0xFF000000) >> 24)	/* 4th data cycle */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t STM_Flash_WritePage(uint8_t *pBuffer, uint32_t ofs);
uint32_t STM_Flash_ReadPage (uint8_t *pBuffer, uint32_t ofs);
uint32_t Flash_EarasePages(uint32_t pg_ofs, uint32_t pgs);

#endif /* __FSMC_NAND_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
