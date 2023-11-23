/*
 * I2C_interface.h
 *
 *  Created on: Aug 21, 2023
 *      Author: Ahmed Abdelaziz
 */

#ifndef I2C_INTERFACE_H_
#define I2C_INTERFACE_H_


typedef enum
{
	I2C1,
	I2C2,
	I2C3
}I2C_Number_t;

typedef enum
{
	SM_MOOD,             /*Standard mood 100Kbps*/
	FM_MOOD              /*Fast mood 400Kbps*/
}I2C_SpeedMode_t;

typedef enum
{
	I2C_POLLING,         /*wait for transmit and receive the data*/
	I2C_DMA,             /*Using DMA for transmit and receive the data*/
	I2C_INTERRUPT        /*using Interrupt for transmit and receive the data*/
}I2C_Transfer_Data_Method_t;

typedef struct
{
	uint16_t OwnAddress;    /*Master/Slave Own Address*/
	I2C_SpeedMode_t  Speed_Mood;     /*set speed mood*/
	I2C_Number_t I2C_Num;       /*Select I2C number to configure*/
	I2C_Transfer_Data_Method_t  Transfer_Method;    /*Select method of transferring the data*/
}I2C_Config_t;

typedef enum
{
	I2C_NULL_PTR_ERR,
	I2C_NOERR,
	I2C_START_ERR,
	I2C_AddressErr,
	I2C_TransmittERR,
	I2C_RECEIVEERR,
}I2C_ErrorConfig_t;

I2C_ErrorConfig_t I2C_SendStartCondition(I2C_Number_t Copy_enI2C_Num);
I2C_ErrorConfig_t I2C_SendStopCondition(I2C_Number_t Copy_enI2C_Num);
I2C_ErrorConfig_t I2C_SendSlaveAddressWithWrite(uint8_t Copy_u8Address,I2C_Number_t Copy_enI2C_Num);
I2C_ErrorConfig_t I2C_SendSlaveAddressWithRead(uint8_t Copy_u8Address,I2C_Number_t Copy_enI2C_Num);
I2C_ErrorConfig_t I2C_MasterSendData(uint8_t Copy_u8Data,I2C_Number_t Copy_enI2C_Num);
I2C_ErrorConfig_t I2C_MasterReadDataByte(uint8_t *Copy_u8Data,I2C_Number_t Copy_enI2C_Num);
I2C_ErrorConfig_t I2C_MasterReadDataByteARR(uint8_t *Copy_u8DataArr,uint8_t Copy_u8ArrSize,I2C_Number_t Copy_enI2C_Num);
uint8_t I2C_MasterInit(I2C_Config_t *Copy_stConfig);
uint8_t I2C_SlaveInit(I2C_Config_t *Copy_stConfig);

#endif /* I2C_INTERFACE_H_ */
