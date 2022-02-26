/*
 * iap.c
 *
 *  Created on: Jul 22, 2021
 *      Author: Vojtese
 */

/* Includes --------------------------
 * ----------------------------------------*/
#include "iap.h"

typedef void (*fnc_ptr)(void);
/**
 * @brief   This function erases the memory.
 * @param   address: First address to be erased (the last is the end of the flash).
 * @return  status: Report about the success of the erasing.
 */
flash_status flash_erase(uint32_t address)
{
  HAL_FLASH_Unlock();

  flash_status status = FLASH_ERROR;
  FLASH_EraseInitTypeDef erase_init;
  uint32_t error = 0u;

  erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
  erase_init.PageAddress = address;
  /* Calculate the number of pages from "address" and the end of flash. */
  erase_init.NbPages = (FLASH_APP_END_ADDRESS - address) / PAGE_IN_FLASH_SIZE;
  /* Do the actual erasing. */
  if (HAL_OK == HAL_FLASHEx_Erase(&erase_init, &error))
  {
    status = FLASH_OK;
  }

  HAL_FLASH_Lock();

  return status;
}

/**
 * @brief   This function flashes the memory.
 * @param   address: First address to be written to.
 * @param   *data:   Array of the data that we want to write.
 * @param   *length: Size of the array.
 * @return  status: Report about the success of the writing.
 */
flash_status flash_write(uint32_t address, uint32_t *data, uint32_t length)
{
  flash_status status = FLASH_OK;

  HAL_FLASH_Unlock();

  /* Loop through the array. */
  for (uint32_t i = 0u; (i < length) && (FLASH_OK == status); i++)
  {
    /* If we reached the end of the memory, then report an error and don't do anything else.*/
    if (FLASH <= address)
    {
      status |= FLASH_ERROR_SIZE;
    }
    else
    {
      /* The actual flashing. If there is an error, then report it. */
      if (HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data[i]))
      {
        status |= FLASH_ERROR_WRITE;
      }
      /* Read back the content of the memory. If it is wrong, then report an error. */
      if (((data[i])) != (*(volatile uint32_t*)address))
      {
        status |= FLASH_ERROR_READBACK;
      }

      /* Shift the address by a word. */
      address += 4u;
    }
  }

  HAL_FLASH_Lock();

  return status;
}
/**
  * @brief	Function, that runs an application saved in a flash memory
  * @param	None
  * @retval	None
  */
void flash_jump_to_app(void)
{
	/* Function, that points to user app */
	if (((*(__IO uint32_t*)FLASH_APP_START_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
	{
		fnc_ptr jump_to_app;
		jump_to_app = (fnc_ptr)(*(__IO uint32_t*) (FLASH_APP_START_ADDRESS + 4u));
		//HAL_UART_DeInit(&huart2);


		HAL_DeInit();

		/* Redirecting the main stack pointer */
		__set_MSP(*(__IO uint32_t*)FLASH_APP_START_ADDRESS);
		jump_to_app();
	}
}


