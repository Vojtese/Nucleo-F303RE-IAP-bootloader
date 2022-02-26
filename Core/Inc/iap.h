/*
 * iap.h
 *
 *  Created on: Jul 22, 2021
 *      Author: Vojtese
 */

#ifndef INC_IAP_H_
#define INC_IAP_H_

#include "stm32f3xx_hal.h"
#include "main.h"

extern UART_HandleTypeDef huart2;
//#define USER_START_APP_ADDRESS_FLASH 	(uint32_t)0x08003000
//#define USER_END_ADDRESS_FLASH 			(uint32_t)0x0800FFFF	/* 64 KBytes */
//#define USER_LAST_PAGE_ADDRESS_FLASH	(uint32_t)0x0800F800
//#define FLASH_PAGE_SIZE					(uint32_t)0x800			/* 2 KBytes */
//
///* Get the number of pages from where the user program will be
// * loaded, is write protected */
//#define FLASH_PAGE_NUMBER				(uint32_t)((USER_START_APP_ADDRESS_FLASH - 0x08000000) >> 12)
//
///* Compute the mask to test if the FLASH mem., where the user program will be
// * loaded, is write protected */
//#define FLASH_PROTECTED_PAGES	((uint32_t)~((1 << FLASH_PAGE_NUMBER) - 1))
//
///* define the user application size */
//#define USER_FLASH_SIZE		(USER_END_ADDRESS_FLASH - USER_START_APP_ADDRESS_FLASH + 1)
//
///* IAP - flash functions */
//uint32_t IAP_Erase(uint32_t StartSector);
//uint32_t IAP_Write(__IO uint32_t* FlashAddress, uint32_t* Data, uint16_t DataLength);
//uint32_t IAP_DisableWriteProtection(void);
//uint32_t IAP_GetWriteProtectionStatus(void);


/* Start and end addresses of the user application. */
#define FLASH_APP_START_ADDRESS ((uint32_t)0x08020000u)
#define FLASH_APP_END_ADDRESS   ((uint32_t)0x0807FFFF)	/**< Leave a little extra space at the end. */
/* Get the number of pages from where the user program will be loaded */
#define  FLASH_PAGE_NUMBER      (uint32_t)((FLASH_APP_START_ADDRESS - 0x08000000) >> 12)
/* define the user application size */
#define USER_FLASH_SIZE   	(FLASH_APP_END_ADDRESS - FLASH_APP_START_ADDRESS + 1)
#define PAGE_IN_FLASH_SIZE		0x800
/* Status report for the functions. */
typedef enum {
  FLASH_OK              = 0x00u, /**< The action was successful. */
  FLASH_ERROR_SIZE      = 0x01u, /**< The binary is too big. */
  FLASH_ERROR_WRITE     = 0x02u, /**< Writing failed. */
  FLASH_ERROR_READBACK  = 0x04u, /**< Writing was successful, but the content of the memory is wrong. */
  FLASH_ERROR           = 0xFFu  /**< Generic error. */
} flash_status;

flash_status flash_erase(uint32_t address);
flash_status flash_write(uint32_t address, uint32_t *data, uint32_t length);
void flash_jump_to_app(void);


#endif /* INC_IAP_H_ */
