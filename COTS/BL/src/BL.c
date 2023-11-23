/*
 * BL.c
 *
 *  Created on: Oct 21, 2023
 *      Author: Ahmed abdelaziz
 */

#include<stdint.h>
#include "ErrorTyp.h"
#include"BL.h"
#include"BL_priv.h"
#include "CRC_interface.h"
#include "USART_interface.h"
#include "GPIO_interface.h"
#include "FLASH_interface.h"


USART_Config_t BL_USART2_Config={
		.USART_Baud = USART_BAUD_115200,
		.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE,
		.USART_Mode = USART_MODE_TXRX,
		.USART_NoOfStopBits = USART_STOPBITS_1,
		.USART_Oversampling = USART_OVERSAMPLING_16,
		.USART_ParityControl = USART_PARITY_DISABLE,
		.USART_SynchMode = ASYNCH,
		.USART_WordLength = USART_WORD_WIDTH_8BITS
};
USART_Handle_t USART2_Handle;





static uint8_t u8VerifyCRC(uint8_t* Copy_pu8DataArr , uint8_t copy_u8Length, uint32_t Copy_u32HostCRC)
{
    USART2_Handle.USARTx = USART_2;
    USART2_Handle.USART_Config = BL_USART2_Config;

	uint8_t Local_u8Iterator , Local_u8CRCStatus;

	uint32_t Local_u32AccCRC, Local_u32Temp;
	for(Local_u8Iterator=0; Local_u8Iterator<copy_u8Length;Local_u8Iterator++)
	{
		Local_u32Temp = Copy_pu8DataArr[Local_u8Iterator];
		Local_u32AccCRC = CRC_u32Accumlator(&Local_u32Temp, 1);

	}
	/*Reset CRC Calculation unit*/
	CRC_Reset();


	if(Local_u32AccCRC == Copy_u32HostCRC)
	{
		Local_u8CRCStatus = CRC_SUCCESS;
	}
	else
	{
		Local_u8CRCStatus = CRC_FAIL;
	}
	return Local_u8CRCStatus;
}

static void voidSendAck(uint8_t Copy_u8ReplyLength)
{
	uint8_t Local_u8AckBuffer[2] = {BL_ACK,Copy_u8ReplyLength};


	USART_u8SendData(&USART2_Handle, Local_u8AckBuffer, 2);
}
static void voidSendNack(void)
{
	uint8_t Local_u8Nack = (uint8_t)BL_NACK;


	USART_u8SendData(&USART2_Handle, &Local_u8Nack, 1);
}

void BL_voidHandleGetVerCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t Local_u8BLVersion , Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{



		voidSendAck(1u);  /*Boot loader version*/
		Local_u8BLVersion = BL_VERSION;

        USART_u8SendData(&USART2_Handle, &Local_u8BLVersion, 1);


	}
	else
	{
		voidSendNack();
	}

}
void BL_voidHandleGetHelpCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t   Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint8_t Local_u8BLCommandsArr[] =
		{
				BL_GET_VER          ,
				BL_GET_HELP   		,
				BL_GET_CID    		,
				BL_GET_RDP_STATUS   ,
				BL_GO_TO_ADDR    	,
				BL_FLASH_ERASE    	,
				BL_MEM_WRITE    	,
				BL_EN_RW_PROTECT    ,
				BL_MEM_READ    		,
				BL_READ_SECTOR_STAUS,
				BL_OTP_READ    		,
				BL_DIS_RW_PROTECT
		};

		voidSendAck(sizeof(Local_u8BLCommandsArr));  /*Boot loader Commands*/


		USART_u8SendData(&USART2_Handle, Local_u8BLCommandsArr, sizeof(Local_u8BLCommandsArr));
	}
	else
	{
		voidSendNack();
	}
}
void BL_voidHandleGetCIDCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t   Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint16_t BL_u16DeviceID = (DBGMCU_IDCODE_REG & 0x0fff);  /*Masking the rest 12 bits*/
		voidSendAck(2u);  /*Boot loader version*/


		USART_u8SendData(&USART2_Handle, (uint8_t *)&BL_u16DeviceID, 2);
	}
	else
	{
		voidSendNack();
	}
}
void BL_voidHandleGetRDPStatusCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t   Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint8_t Local_u8RDPStatus = (uint8_t)((RDB_USER_OPTION_WORD >> 8) & 0xff);

		voidSendAck(1u);  /*Boot loader version*/


		USART_u8SendData(&USART2_Handle, &Local_u8RDPStatus, 1);
	}
	else
	{
		voidSendNack();
	}
}
void BL_voidHandleGoToAddressCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t   Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint32_t Local_u32Address;
		uint8_t Local_u8AddressValidStatus;

		voidSendAck(1u);  /*Boot loader version*/

		/*Extract the address*/

		Local_u32Address = *((uint32_t *)&Copy_pu8CmdPacket[2]);
		Local_u8AddressValidStatus = u8ValidateAddress(Local_u32Address);

		USART_u8SendData(&USART2_Handle, &Local_u8AddressValidStatus, 1);
		if(Local_u8AddressValidStatus == VALID_ADDRESS)
		{

			/*Define pointer to function*/
			void (*Local_pvFunctionPtr)(void) = NULL;

			Local_u32Address |= 1; /*increment address by 1 to make T-bit equal 1*/
			Local_pvFunctionPtr = (void*)Local_u32Address;
			Local_pvFunctionPtr();
		}
		else
		{
		}
	}

}
static uint8_t u8ValidateAddress(uint32_t Copy_u32Address)
{
	/*Address is at flash or SRAM*/

	uint8_t Local_u8AddressStatus;

	if((Copy_u32Address >= FLASH_BASE) && (Copy_u32Address <= FLASH_END))
	{
		Local_u8AddressStatus = VALID_ADDRESS;
	}
	else if ((Copy_u32Address >= SRAM1_BASE) && (Copy_u32Address <= (SRAM1_BASE + (128*4))))
	{
		Local_u8AddressStatus = VALID_ADDRESS;
	}
	else
	{
		Local_u8AddressStatus = INVALID_ADDRESS;
	}
	return Local_u8AddressStatus;
}
void BL_voidHandleFlashEraseVerCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t   Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint8_t Local_u8EraseStatus;
		voidSendAck(1u);  /*Boot loader version*/



		Local_u8EraseStatus = u8ExcuteFlashErase(Copy_pu8CmdPacket[2], Copy_pu8CmdPacket[3]);



		USART_u8SendData(&USART2_Handle, &Local_u8EraseStatus, 1);
	}
	else
	{
		voidSendNack();
	}
}
static uint8_t u8ExcuteFlashErase(uint8_t Copy_u8SectorNumber, uint8_t Copy_u8NumberOfSector)
{
	uint8_t  Local_ErrorStatus = BL_ERASE_OK;

	if((Copy_u8NumberOfSector > 8) && (Copy_u8SectorNumber != 0xff))
	{
		Local_ErrorStatus = BL_ERASE_ERR;
	}
	else if ((Copy_u8SectorNumber > 7) && (Copy_u8SectorNumber != 0xff))
	{
		Local_ErrorStatus = BL_ERASE_ERR;
	}
	else
	{

		uint32_t Local_u32SectorErr;

		if(Copy_u8SectorNumber == 0xff)
		{
			/*Mass erase operation is required*/
			/*unlock the flash before erasing*/
	        Flash_Unlock();

			Local_ErrorStatus = Flash_u8Erase(Copy_u8SectorNumber, Copy_u8NumberOfSector);

			Flash_Lock();
		}
		else
		{
			/*Sector erase operation is required*/
			uint8_t Local_u8RemainigSectors = 8 - Copy_u8SectorNumber;

			if(Copy_u8NumberOfSector > Local_u8RemainigSectors)
			{
				/*if number of sectors is bigger than maximum make it equal to maximum*/
				Copy_u8NumberOfSector = Local_u8RemainigSectors;
				/*unlock the flash before erasing*/

		        Flash_Unlock();
				//Local_ErrorStatus = HAL_FLASHEx_Erase(&Local_MyErase, &Local_u32SectorErr);
				Local_ErrorStatus = Flash_u8Erase(Copy_u8SectorNumber, Copy_u8NumberOfSector);
				/*Lock the flash again*/

				Flash_Lock();
			}
			else
			{
				/*Do no thing*/

		        Flash_Unlock();

				Local_ErrorStatus = Flash_u8Erase(Copy_u8SectorNumber, Copy_u8NumberOfSector);
				/*Lock the flash again*/

				Flash_Lock();
			}

		}

		return Local_ErrorStatus;
	}
	return Local_ErrorStatus;
}
void BL_voidHandleMemWriteCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t   Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint8_t Local_u8WritingStatus;
		uint32_t Local_u32Address = *((uint32_t*)&Copy_pu8CmdPacket[2]); /*Extract Base memory address*/

		uint8_t Local_u8Address_Status = u8ValidateAddress(Local_u32Address);
		voidSendAck(1u);  /*Send the acknowledge*/

		if(Local_u8Address_Status == VALID_ADDRESS)
		{
			uint8_t Local_u8PayLoadLength = Copy_pu8CmdPacket[6];
			Local_u8WritingStatus = u8ExcuteMemoryWrite(&Copy_pu8CmdPacket[7],Local_u32Address,Local_u8PayLoadLength);

		}
		else
		{
			Local_u8WritingStatus = WRITING_ERROR;
		}


		USART_u8SendData(&USART2_Handle, &Local_u8WritingStatus, 1);
	}
	else
	{
		voidSendNack();
	}
}
static uint8_t u8ExcuteMemoryWrite(uint8_t *Copy_pu8Buffer , uint32_t Copy_u32Address , uint8_t Copy_u8Length)
{
	uint8_t Local_u8ErrorStatus = BL_PROGRAMMING_OK;

	if((Copy_u32Address >= FLASH_BASE) && (Copy_u32Address <= FLASH_END))
	{
		uint8_t Local_u8Iterator;

		/*Unlock the flash before writing */

		Flash_Unlock();
		for(Local_u8Iterator = 0 ; Local_u8Iterator < Copy_u8Length ; Local_u8Iterator++)
		{
			Local_u8ErrorStatus = FLASH_u8Programe(Copy_u32Address+ Local_u8Iterator, FLASH_PROGRAM_BYTE, Copy_pu8Buffer[Local_u8Iterator]);
		}

		Flash_Lock();
	}
	else
	{
		/* Writing in SRAM case*/
		uint8_t Local_u8Iterator;
		uint8_t * Local_pu8Dest = (uint8_t*)Copy_u32Address;

		for(Local_u8Iterator = 0 ; Local_u8Iterator < Copy_u32Address ; Local_u8Iterator++)
		{
			Local_pu8Dest[Local_u8Iterator] = Copy_pu8Buffer[Local_u8Iterator];
		}
	}

	return Local_u8ErrorStatus;
}
void BL_voidHandleMemReadCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t   Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint8_t Local_u8ReadStatus,Local_u8AddressValidate;
		uint32_t Local_u8Destination[255];
		uint32_t Local_u32Address = *((uint32_t*)&Copy_pu8CmdPacket[2]);/*Extract memory address*/
		uint8_t Local_u8Length = Copy_pu8CmdPacket[6];
		voidSendAck(1u);  /*Boot loader version*/
		Local_u8AddressValidate = u8ValidateAddress(Local_u32Address);
		if(Local_u8AddressValidate == VALID_ADDRESS)
		{
			Local_u8ReadStatus = u8ExcuteMemoryRead(Local_u32Address , Local_u8Length,Local_u8Destination);
		}
		else
		{
			Local_u8ReadStatus = READING_ERROR;
		}

		USART_u8SendData(&USART2_Handle, &Local_u8ReadStatus, 1);
	}
	else
	{
		voidSendNack();
	}
}
static uint8_t u8ExcuteMemoryRead(uint32_t Copy_u32Address , uint8_t Copy_u8Length , uint32_t * Copy_pu32Destination)
{

}
void BL_voidHandleEnRWProtectCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t   Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint8_t Local_u8SectorDetails;  /*to hold Sectors*/
		uint8_t Local_u8ProtectionMode; /*to hold Protection mode*/
		uint8_t Local_u8Status;
	    /*Extract Sector Details*/
		Local_u8SectorDetails =  Copy_pu8CmdPacket[2];
		/*Extract Protection Details*/
		Local_u8ProtectionMode = Copy_pu8CmdPacket[3];
		voidSendAck(1u);  /*Send the acknowledge*/
		Local_u8Status = u8ExcuteRWProtect(Local_u8SectorDetails ,Local_u8ProtectionMode);
		USART_u8SendData(&USART2_Handle, &Local_u8Status, 1);
	}
	else
	{
		voidSendNack();
	}
}
static uint8_t u8ExcuteRWProtect(uint8_t Copy_u8SectorDetails , uint8_t Copy_u8ProtectionMode)
{
	uint8_t Local_u8ErrorStatus = BL_RW_PROTECTION_SUCCESS;
	uint8_t Local_u8Iterator;
	for(Local_u8Iterator = 0 ; Local_u8Iterator < 8 ; Local_u8Iterator++)
	{
		if(((Copy_u8SectorDetails >> Local_u8Iterator ) & 1 )== 1)
		{
			if(((Copy_u8ProtectionMode >> Local_u8Iterator ) & 1 )== 0)
			{
				/*Write protection Nedded*/
				FLASH_OPT_Unlock();
				Local_u8ErrorStatus = FLASH_u8SectorProtection_En(Local_u8Iterator , WRITE_PROTECTION);
				FLASH_OPT_Lock();
			}
			else
			{
				/*Read Write Protection Nedded*/
				FLASH_OPT_Unlock();
				Local_u8ErrorStatus = FLASH_u8SectorProtection_En(Local_u8Iterator , READ_WRITE_PROTECTION);
				FLASH_OPT_Lock();
			}
		}
	}
	return Local_u8ErrorStatus;
}
void BL_voidHandleReadSectorCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t   Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint16_t Local_u16SectorStatus = (WRP_OPTION_WORD & 0xffff);
		voidSendAck(1u);  /*Boot loader version*/
		USART_u8SendData(&USART2_Handle, &Local_u16SectorStatus, 1);
	}
	else
	{
		voidSendNack();
	}
}
void BL_voidHandleOTPReadCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t   Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		voidSendAck(1u);  /*Boot loader version*/
	}
	else
	{
		voidSendNack();
	}
}
void BL_voidHandleDisableRWProtectCmd(uint8_t* Copy_pu8CmdPacket)
{
	uint8_t   Local_u8CRCStatus , Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength = Copy_pu8CmdPacket[0] + 1 ; /*The first byte include the length to follow*/

	Local_u32HostCRC = *((uint32_t*)(Copy_pu8CmdPacket + Local_u8CmdLength - 4));

	Local_u8CRCStatus = u8VerifyCRC(Copy_pu8CmdPacket, (Local_u8CmdLength - 4), Local_u32HostCRC);

	if(Local_u8CRCStatus == CRC_SUCCESS)
	{
		uint8_t Local_u8Status;
		voidSendAck(1u);  /*Boot loader version*/
		FLASH_OPT_Unlock();
		Local_u8Status = FLASH_u8SectorProtection_Dis();
		FLASH_OPT_Lock();
		USART_u8SendData(&USART2_Handle, &Local_u8Status, 1);
	}
	else
	{
		voidSendNack();
	}
}

