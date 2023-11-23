/*
 * I2C_prog.c
 *
 *  Created on: Aug 21, 2023
 *      Author: Ahmed Abdelaziz
 */







#include <stdint.h>
#include "ErrorTyp.h"
#include "stm32f446xx.h"

#include "I2C_interface.h"
#include "I2C_private.h"
#include "I2C_cfg.h"


static I2C_RegDef_t * I2C_ARR[3] = {I2C_1,I2C_2,I2C_3};



uint8_t I2C_MasterInit(I2C_Config_t *Copy_stConfig)
{
   uint8_t Local_u8ErrorStatus = OK;
   if(Copy_stConfig != NULL)
   {
	   //I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR1 |= 1<<15;
	   uint16_t Local_u16CCR_Val = 0,Local_u16TRISE_Val=0;
	   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_OAR1 = OAR1_MASK;   /*Clear Address bits*/
	   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_OAR1 |= ((Copy_stConfig->OwnAddress)<<OAR1_7BIT_ADDRESS_ACCESS) | 1<<14; ; /*Set the Address*/
	   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR2  = CR2_MASK;   /*Clear control register 2*/
	   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR2  |=  APB1_FREQUENCY;  /*Setting the APB1 Bus frequency*/
	   /*Selecting between DMA or Interrupt for transferring the data */
	   switch (Copy_stConfig->Transfer_Method)
	   {
	   case I2C_DMA :
		   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR2 |= 1<<DMA_BIT ;  /*Set DMA Configuration*/
		   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR2 |= 1<<LAST_BIT ;
		   break;
	   case I2C_INTERRUPT :
		   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR2 |= 1<<ITBUFF_BIT;  /*Set Interrupt configuration*/
		   break;
	   }
	   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CCR  =  CCR_MASK;   /*Clear Clock control Register*/
	   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CCR  |= (Copy_stConfig->Speed_Mood)<<SPEED_MOOD_ACCESS; /*Select SM or FM Mood*/
	   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_TRISE = TRISE_MASK;   /*Clear TRISE register*/
	   /*Configure CCR Value and TRISE Value*/
	   switch (Copy_stConfig->Speed_Mood)
	   {
	   case SM_MOOD :
		   Local_u16CCR_Val = (uint16_t)APB1_FREQUENCY * SM_CCR_MUL;
		   Local_u16TRISE_Val = (uint16_t)APB1_FREQUENCY + TRISE_KEY;
		   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CCR |= Local_u16CCR_Val;
		   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_TRISE |= Local_u16TRISE_Val;
		   break;
	   case FM_MOOD :
		   Local_u16CCR_Val = (uint16_t)APB1_FREQUENCY * FM_CCR_MUL / FM_CCR_DIV;
		   Local_u16TRISE_Val = ((uint16_t)APB1_FREQUENCY * FM_TRISE_MUL / FM_TRISE_DIV) + TRISE_KEY;
		   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CCR |= Local_u16CCR_Val;
		   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_TRISE |= Local_u16TRISE_Val;
		   break;
	   }
       I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR1  =  CR1_MASK; /*Clear Control register 1*/
       I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR1  |= 1<<ACK_BIT;   /*Enable Acknowledge Bit*/

       /*Enable I2C peripheral*/
       I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR1 |= ENABLE;
       I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR1  |= 1<<ACK_BIT;   /*Enable Acknowledge Bit*/
   }
   else
   {
	   Local_u8ErrorStatus = NULL_PTR_ERR;
   }
   return Local_u8ErrorStatus;
}
uint8_t I2C_SlaveInit(I2C_Config_t *Copy_stConfig)
{

   uint8_t Local_u8ErrorStatus = OK;
   if(Copy_stConfig != NULL)
   {
	   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_OAR1 = (Copy_stConfig->OwnAddress)<<OAR1_MASK;   /*Clear Address bits*/
	   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_OAR1 = (Copy_stConfig->OwnAddress)<<OAR1_7BIT_ADDRESS_ACCESS;  /*Set the Address*/
       I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR1  =  CR1_MASK; /*Clear Control register 1*/
       I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR1  |= 1<<ACK_BIT;   /*Enable Acknowledge Bit*/
	   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR2  = CR2_MASK;   /*Clear control register 2*/
	   /*Selecting between DMA or Interrupt for transferring the data */
	   switch (Copy_stConfig->Transfer_Method)
	   {
	   case I2C_DMA :
		   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR2 |= 1<<DMA_BIT ;  /*Set DMA Configuration*/
		   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR2 |= 1<<LAST_BIT ;
		   break;
	   case I2C_INTERRUPT :
		   I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR2 |= 1<<ITBUFF_BIT;  /*Set Interrupt configuration*/
		   break;
	   }

       /*Enable I2C peripheral*/
       I2C_ARR[Copy_stConfig->I2C_Num]->I2C_CR1 |= ENABLE;
   }
	else
   {
	   Local_u8ErrorStatus = NULL_PTR_ERR;
   }
   return Local_u8ErrorStatus;
}
I2C_ErrorConfig_t I2C_SendStartCondition(I2C_Number_t Copy_enI2C_Num)
{
	I2C_ErrorConfig_t Local_ErrStatus = I2C_NOERR;  /*Define Error Status*/
	//uint16_t Local_u16SR1_Val = 0; /*Define variable to carry SR1 value register*/
	uint16_t Local_u16Counter = 0; /*Define Counter for Time out detection*/
	/*Set START BIT*/
	I2C_ARR[Copy_enI2C_Num]->I2C_CR1 |= 1<<START_BIT;

	/*Wait till SB Bit is Set*/
	while(((I2C_ARR[Copy_enI2C_Num]->I2C_SR1) & 1u) == 0&& Local_u16Counter != TIME_OUT)
		Local_u16Counter ++;
	if(Local_u16Counter == TIME_OUT)
	{
		Local_ErrStatus = I2C_START_ERR;
	}
	else
	{

	}
    return Local_ErrStatus;
}
I2C_ErrorConfig_t I2C_SendSlaveAddressWithWrite(uint8_t Copy_u8Address,I2C_Number_t Copy_enI2C_Num)
{
	uint8_t Local_u8Address = Copy_u8Address<< R_W_ACCESS; /*Clear LSB for Writing condition*/
	uint16_t Local_u16SR_Val = 0; /*Define variable to carry SR1,SR2 value registers*/
	I2C_ErrorConfig_t Local_enErrStatus = I2C_NOERR; /*Define Error Status*/
	uint16_t Local_u16Counter = 0; /*Define Counter for Time out detection*/

	/*Clearing SB Event by Reading SR1 register*/
	Local_u16SR_Val = I2C_ARR[Copy_enI2C_Num]->I2C_SR1;
    /*Sending Address With Writing Bit*/
	I2C_ARR[Copy_enI2C_Num]->I2C_DR = Local_u8Address;
	/*Wait until ADDR Bit is set*/
	while(((I2C_ARR[Copy_enI2C_Num]->I2C_SR1)>>ADDR_ACCESS & 1) == 0 && Local_u16Counter != TIME_OUT)
		Local_u16Counter ++;
	if(Local_u16Counter == TIME_OUT)
	{
		Local_enErrStatus =I2C_AddressErr;
	}
	else  /*Clearing ADDR Event By Reading SR1, SR2 Registers*/
	{
		Local_u16SR_Val = I2C_ARR[Copy_enI2C_Num]->I2C_SR1;
		Local_u16SR_Val = I2C_ARR[Copy_enI2C_Num]->I2C_SR2;
	}
    return Local_enErrStatus;
}
I2C_ErrorConfig_t I2C_SendSlaveAddressWithRead(uint8_t Copy_u8Address,I2C_Number_t Copy_enI2C_Num)
{
	uint8_t Local_u8Address = (Copy_u8Address<< R_W_ACCESS) | R_W_ACCESS; /*Clear LSB for Writing condition*/
	uint16_t Local_u16SR_Val = 0; /*Define variable to carry SR1,SR2 value registers*/
	I2C_ErrorConfig_t Local_enErrStatus = I2C_NOERR; /*Define Error Status*/
	uint16_t Local_u16Counter = 0; /*Define Counter for Time out detection*/
	/*Clearing SB Event by Reading SR1 register*/
	Local_u16SR_Val = I2C_ARR[Copy_enI2C_Num]->I2C_SR1;
    /*Sending Address With Reading Bit*/
	I2C_ARR[Copy_enI2C_Num]->I2C_DR = Local_u8Address ;
	/*Wait until ADDR Bit is set*/
	while(((I2C_ARR[Copy_enI2C_Num]->I2C_SR1)>>ADDR_ACCESS & 1) == 0 && Local_u16Counter != TIME_OUT)
		Local_u16Counter ++;
	if(Local_u16Counter == TIME_OUT)
	{
		Local_enErrStatus =I2C_AddressErr;
	}

    return Local_enErrStatus;
}
I2C_ErrorConfig_t I2C_SendStopCondition(I2C_Number_t Copy_enI2C_Num)
{
	/*Set Stop Bit*/
    I2C_ARR[Copy_enI2C_Num]->I2C_CR1 = 1<<STOP_BIT;
}
I2C_ErrorConfig_t I2C_MasterSendData(uint8_t Copy_u8Data,I2C_Number_t Copy_enI2C_Num)
{
	I2C_ErrorConfig_t Local_enErrStatus = I2C_NOERR; /*Define Error Status*/
	I2C_ARR[Copy_enI2C_Num]->I2C_DR = Copy_u8Data;  /*Writing data into DR register*/
	uint16_t Local_u16Counter = 0; /*Define Counter for Time out detection*/
	/*wait until the TXE bit is set*/
	while(((I2C_ARR[Copy_enI2C_Num]->I2C_SR1)>>TXE_BIT & 1) == 0 && Local_u16Counter != TIME_OUT)
		Local_u16Counter ++;
	if(Local_u16Counter == TIME_OUT)
	{
		Local_enErrStatus =I2C_TransmittERR;
	}

	return Local_enErrStatus;

}
I2C_ErrorConfig_t I2C_MasterReadDataByte(uint8_t *Copy_u8Data,I2C_Number_t Copy_enI2C_Num)
{
	I2C_ErrorConfig_t Local_enErrStatus = I2C_NOERR; /*Define Error Status*/
	if(Copy_u8Data != NULL)
	{

		uint16_t Local_u16Counter = 0; /*Define Counter for Time out detection*/
		uint16_t Local_u16SR_Val = 0; /*Define variable to carry SR1,SR2 value registers*/

		/*Clear Acknowledge bit*/
		I2C_ARR[Copy_enI2C_Num]->I2C_CR1 &= ~(1<<ACK_BIT);

		/*Clearing ADDR Event By Reading SR1, SR2 Registers*/
		Local_u16SR_Val = I2C_ARR[Copy_enI2C_Num]->I2C_SR1;
		Local_u16SR_Val = I2C_ARR[Copy_enI2C_Num]->I2C_SR2;

        /*Waiting until the RXNe bit is Raised*/
		while(((I2C_ARR[Copy_enI2C_Num]->I2C_SR1)>>RXE_BIT & 1) == 0 && Local_u16Counter != TIME_OUT)
			Local_u16Counter ++;
		if(Local_u16Counter == TIME_OUT)
		{
			Local_enErrStatus =I2C_RECEIVEERR;
		}
		else
		{
			/*Reading Data Register*/
			*Copy_u8Data = I2C_ARR[Copy_enI2C_Num]->I2C_DR;
		}
		/*Send stop condition*/
		I2C_ARR[Copy_enI2C_Num]->I2C_CR1 |= 1<<STOP_BIT;   /*Set Stop Bit*/
	}
	else
	{
		Local_enErrStatus = I2C_NULL_PTR_ERR;
	}

	return Local_enErrStatus;
}
I2C_ErrorConfig_t I2C_MasterReadDataByteARR(uint8_t *Copy_u8DataArr,uint8_t Copy_u8ArrSize,I2C_Number_t Copy_enI2C_Num)
{
	I2C_ErrorConfig_t Local_enErrStatus = I2C_NOERR; /*Define Error Status*/
	uint16_t Local_u16Counter = 0; /*Define Counter for Time out detection*/
	uint16_t Local_u16SR_Val = 0; /*Define variable to carry SR1,SR2 value registers*/
	uint8_t Local_u8RecieverCounter = 0 ; /*Define a counter for the receiving process*/
	if(Copy_u8DataArr != NULL)
	{
        if(Copy_u8ArrSize == RECEIVE_2BYTES)
        {
    		/*Clear Acknowledge bit*/
    		I2C_ARR[Copy_enI2C_Num]->I2C_CR1 &= ~(1<<ACK_BIT);
    		/*Set POS bit*/
    		I2C_ARR[Copy_enI2C_Num]->I2C_CR1 |= 1<<POS_BIT;
    		/*Clearing ADDR Event By Reading SR1, SR2 Registers*/
    		Local_u16SR_Val = I2C_ARR[Copy_enI2C_Num]->I2C_SR1;
    		Local_u16SR_Val = I2C_ARR[Copy_enI2C_Num]->I2C_SR2;
    		/*Wait Until BTF bit is set*/
    		while(((I2C_ARR[Copy_enI2C_Num]->I2C_SR1)>>BTF_BIT & 1) == 0 && Local_u16Counter != TIME_OUT)
    			Local_u16Counter++;
    		if(Local_u16Counter == TIME_OUT)
    		{
    			Local_enErrStatus = I2C_RECEIVEERR;
    		}
    		else
    		{
        		/*Send Stop Condition*/

    		    I2C_ARR[Copy_enI2C_Num]->I2C_CR1 |= 1<<STOP_BIT;   /*Set Stop Bit*/
        		/*Reading Data Register*/
        		Copy_u8DataArr[0] = I2C_ARR[Copy_enI2C_Num]->I2C_DR;
        		Copy_u8DataArr[1] = I2C_ARR[Copy_enI2C_Num]->I2C_DR;
    		}

        }
        else
        {
    		/*Clearing ADDR Event By Reading SR1, SR2 Registers*/
    		Local_u16SR_Val = I2C_ARR[Copy_enI2C_Num]->I2C_SR1;
    		Local_u16SR_Val = I2C_ARR[Copy_enI2C_Num]->I2C_SR2;
        	/*Loop to Receive the data*/
        	for(Local_u8RecieverCounter = 0;Local_u8RecieverCounter <(Copy_u8ArrSize - LAST_2BYTES);Local_u8RecieverCounter ++)
        	{
                /*Waiting until the RXNe bit is Raised*/
        		while(((I2C_ARR[Copy_enI2C_Num]->I2C_SR1)>>RXE_BIT & 1) == 0 && Local_u16Counter != TIME_OUT)
        			Local_u16Counter ++;
        		if(Local_u16Counter == TIME_OUT)
        		{
        			Local_enErrStatus =I2C_RECEIVEERR;
        			break;
        		}
        		else
        		{
        			/*Reading Data Register*/
        			Copy_u8DataArr[Local_u8RecieverCounter] = I2C_ARR[Copy_enI2C_Num]->I2C_DR;
        		}
        	}
        	/*Wait Until BTF bit is set*/
    		while(((I2C_ARR[Copy_enI2C_Num]->I2C_SR1)>>BTF_BIT & 1) == 0 && Local_u16Counter != TIME_OUT)
    			Local_u16Counter++;
    		if(Local_u16Counter == TIME_OUT)
    		{
    			Local_enErrStatus = I2C_RECEIVEERR;
    		}
    		else /*Last 3Bytes Reading procedure*/
    		{
        		/*Clear Acknowledge bit*/
        		I2C_ARR[Copy_enI2C_Num]->I2C_CR1 &= ~(1<<ACK_BIT);
        		Local_u8RecieverCounter ++;
    			/*Reading Data Register*/
    			Copy_u8DataArr[Local_u8RecieverCounter] = I2C_ARR[Copy_enI2C_Num]->I2C_DR;
    			Local_u16Counter=0; /*Reset Time out counter*/
    			/*Wait Until BTF bit is set*/
        		while(((I2C_ARR[Copy_enI2C_Num]->I2C_SR1)>>BTF_BIT & 1) == 0 && Local_u16Counter != TIME_OUT)
        			Local_u16Counter++;
        		if(Local_u16Counter == TIME_OUT)
        		{
        			Local_enErrStatus = I2C_RECEIVEERR;
        		}
        		else
        		{
        			/*Send stop condition*/
        			I2C_ARR[Copy_enI2C_Num]->I2C_CR1 |= 1<<STOP_BIT;   /*Set Stop Bit*/
        			/*Reading Last 2 bytes Register*/
        			Copy_u8DataArr[Local_u8RecieverCounter] = I2C_ARR[Copy_enI2C_Num]->I2C_DR;
        			Local_u8RecieverCounter++;
        			Copy_u8DataArr[Local_u8RecieverCounter] = I2C_ARR[Copy_enI2C_Num]->I2C_DR;
        		}
    		}
        }
	}
	else
	{
		Local_enErrStatus = I2C_NULL_PTR_ERR;
	}
    return Local_enErrStatus;
}
