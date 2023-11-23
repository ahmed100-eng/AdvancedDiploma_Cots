/*
 * BL_priv.h
 *
 *  Created on: Oct 21, 2023
 *      Author: Ahmed abdelaziz
 */

#ifndef INC_BL_PRIV_H_
#define INC_BL_PRIV_H_

#define CRC_SUCCESS 0u
#define CRC_FAIL    1u

#define BL_VERSION  1u

#define VALID_ADDRESS    0u
#define INVALID_ADDRESS  1u

#define DBGMCU_IDCODE_REG         *((volatile uint32_t *)0xE0042000)

#define RDB_USER_OPTION_WORD      *((volatile uint32_t *)0x1FFFC000)
#define WRP_OPTION_WORD           *((volatile uint32_t *)0x1FFFC008)

#define WRITING_SUCCESS   0u
#define WRITING_ERROR     1u

#define BL_ERASE_OK           0u
#define BL_ERASE_ERR          1u

#define BL_PROGRAMMING_OK     0u
#define BL_PROGRAMMING_ERR    1u

#define READING_SUCCESS       0u
#define READING_ERROR         1u

#define BL_RW_PROTECTION_SUCCESS 0u
#define BL_RW_PROTECTION_ERROR   1u

static uint8_t u8VerifyCRC(uint8_t* Copy_pu8DataArr , uint8_t copy_u8Length, uint32_t Copy_u32HostCRC);

static void voidSendAck(uint8_t Copy_u8ReplyLength);
static void voidSendNack(void);
static uint8_t u8ValidateAddress(uint32_t Copy_u32Address);
static uint8_t u8ExcuteFlashErase(uint8_t Copy_u8SectorNumber, uint8_t Copy_u8NumberOfSector);
static uint8_t u8ExcuteMemoryWrite(uint8_t *Copy_pu8Buffer , uint32_t Copy_u32Address , uint8_t Copy_u8Length);
static uint8_t u8ExcuteMemoryRead(uint32_t Copy_u32Address , uint8_t Copy_u8Length , uint32_t * Copy_pu32Destination);
static uint8_t u8ExcuteRWProtect(uint8_t Copy_u8SectorDetails , uint8_t Copy_u8ProtectionMode);


#endif /* INC_BL_PRIV_H_ */
