/*
 * BL.h
 *
 *  Created on: Oct 21, 2023
 *      Author: Ahmed abdelaziz
 */

#ifndef INC_BL_H_
#define INC_BL_H_

#define BL_ACK                  0xA5
#define BL_NACK                 0x7FA

#define BL_GET_VER    			0x51
#define BL_GET_HELP   			0x52
#define BL_GET_CID    		 	0x53
#define BL_GET_RDP_STATUS       0x54
#define BL_GO_TO_ADDR    		0x55
#define BL_FLASH_ERASE    		0x56
#define BL_MEM_WRITE    		0x57
#define BL_EN_RW_PROTECT    	0x58
#define BL_MEM_READ    			0x59
#define BL_READ_SECTOR_STAUS    0x5A
#define BL_OTP_READ    			0x5B
#define BL_DIS_RW_PROTECT   	0x5C


void BL_voidHandleGetVerCmd(uint8_t* Copy_pu8CmdPacket);
void BL_voidHandleGetHelpCmd(uint8_t* Copy_pu8CmdPacket);
void BL_voidHandleGetCIDCmd(uint8_t* Copy_pu8CmdPacket);
void BL_voidHandleGetRDPStatusCmd(uint8_t* Copy_pu8CmdPacket);
void BL_voidHandleGoToAddressCmd(uint8_t* Copy_pu8CmdPacket);
void BL_voidHandleFlashEraseVerCmd(uint8_t* Copy_pu8CmdPacket);
void BL_voidHandleEnRWProtectCmd(uint8_t* Copy_pu8CmdPacket);
void BL_voidHandleMemReadCmd(uint8_t* Copy_pu8CmdPacket);
void BL_voidHandleReadSectorCmd(uint8_t* Copy_pu8CmdPacket);
void BL_voidHandleOTPReadCmd(uint8_t* Copy_pu8CmdPacket);
void BL_voidHandleDisableRWProtectCmd(uint8_t* Copy_pu8CmdPacket);
void BL_voidHandleMemWriteCmd(uint8_t* Copy_pu8CmdPacket);

#define FLASH_BASE            0x08000000UL
#define FLASH_END             0x0807ffffUL
#define SRAM1_BASE            0x20000000UL


#endif /* INC_BL_H_ */
