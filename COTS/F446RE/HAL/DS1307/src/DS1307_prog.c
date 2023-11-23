/*
 * DS1037_prog.c
 *
 *  Created on: Aug 25, 2023
 *      Author: Ahmed abdelaziz
 */


#include <stdint.h>
#include "stm32f446xx.h"
#include "ErrorTyp.h"

#include "I2C_interface.h"
#include "I2C_cfg.h"
#include "I2C_private.h"

#include "DS1037_interface.h"
#include "DS1037_private.h"












static uint8_t ConvetTo_BCD(uint8_t Copy_u8DecimalVal);

static uint8_t ConvertTo_Decimal(uint8_t Copy_u8BCDVal);


uint8_t DS1037_SetTime(DS1037_Config_t *Copy_stTimeVal)
{
     uint8_t Local_u8ErrorStatus = OK;
     uint8_t Local_u8TimeArr[DS1037_BUFFER_SIZE] = {0};
     if(Copy_stTimeVal != NULL)
     {
    	 /*Prepare data to send to DS1037 */
    	 uint8_t Local_u8Seconds = (ConvetTo_BCD(Copy_stTimeVal->Seconds)) | 0<<CH_BIT;
    	 uint8_t Local_u8Minuites = ConvetTo_BCD(Copy_stTimeVal->Minuites);
    	 uint8_t Local_u8DayOfWeek = Copy_stTimeVal->Day_of_Week;
    	 uint8_t Local_u8Date = ConvetTo_BCD(Copy_stTimeVal->Date);
    	 uint8_t Local_u8Month = ConvetTo_BCD(Copy_stTimeVal->Month);
    	 uint8_t Local_u8Year = ConvetTo_BCD(Copy_stTimeVal->Year);
    	 uint8_t Local_u8Hours = ConvetTo_BCD(Copy_stTimeVal->Hours);
    	 switch(Copy_stTimeVal->Display_Method)
    	 {
    	 case DISPLAY_24 : Local_u8Hours |= DISPLAY_24 << BIT_12_24_DISPLAY;break;
    	 case DISPLAY_12 : Local_u8Hours |= (DISPLAY_12 << BIT_12_24_DISPLAY | (Copy_stTimeVal->MidDay)<<AM_PM_BIT);break;
    	 }
    	 /*Assign the data in array to send*/
    	 Local_u8TimeArr[SECOND] = Local_u8Seconds ;
    	 Local_u8TimeArr[MINUIT] = Local_u8Minuites;
    	 Local_u8TimeArr[HOUR] = Local_u8Hours;
    	 Local_u8TimeArr[DAY] = Local_u8DayOfWeek;
    	 Local_u8TimeArr[DATE] = Local_u8Date;
    	 Local_u8TimeArr[MONTH] = Local_u8Month;
    	 Local_u8TimeArr[YEAR] = Local_u8Year;
    	 /*Send start condition*/
    	 I2C_SendStartCondition(I2C1);
         /*Send DS1037 Address with write*/
    	 I2C_SendSlaveAddressWithWrite(DS1037_ADDRESS, I2C1);
    	 /*intialise DS1307 Pointer*/
    	 I2C_MasterSendData(DS1307_REGISTER_ADDRESS,I2C1);
    	 /*Send Data*/
         for(uint8_t Counter = SECOND ; Counter <= YEAR ; Counter++)
         {
        	 I2C_MasterSendData(Local_u8TimeArr[Counter], I2C1);
         }
         /*Send Stop condition*/
         I2C_SendStopCondition(I2C1);
     }
     else
     {
    	 Local_u8ErrorStatus = NULL_PTR_ERR;
     }
     return Local_u8ErrorStatus;
}
uint8_t DS1037_ReadTime(DS1037_Config_t *Copy_stTimeVal)
{
    uint8_t Local_u8ErrorStatus = OK;
    if(Copy_stTimeVal != NULL)
    {
    	uint8_t Local_u8TimeArr[DS1037_BUFFER_SIZE] = {0};
   	     /*Send start condition*/
   	     I2C_SendStartCondition(I2C1);
         /*Send DS1037 Address with write*/
    	 I2C_SendSlaveAddressWithWrite(DS1037_ADDRESS, I2C1);
    	 /*intialise DS1307 Pointer*/
    	 I2C_MasterSendData(DS1307_REGISTER_ADDRESS,I2C1);
  	     /*Send Repeated start condition*/
  	     I2C_SendStartCondition(I2C1);
    	/*Send DS1037 Address with Read*/
    	I2C_SendSlaveAddressWithRead(DS1037_ADDRESS, I2C1);

    	/*Read data from DS1037*/
    	I2C_MasterReadDataByteARR(&Local_u8TimeArr, DS1037_BUFFER_SIZE, I2C1);
    	if((Local_u8TimeArr[HOUR]>>BIT_12_24_DISPLAY) == DISPLAY_24)
    	{
    		Copy_stTimeVal->Display_Method = DISPLAY_24;
    		Copy_stTimeVal->MidDay = NO_AM_PM;
    		Copy_stTimeVal->Hours = ConvertTo_Decimal(Local_u8TimeArr[HOUR]);
    	}
    	else
    	{
    		Copy_stTimeVal->Display_Method = DISPLAY_12;
    		Copy_stTimeVal->MidDay = 1 & (Local_u8TimeArr[HOUR]>>AM_PM_BIT);
            Local_u8TimeArr[HOUR] &= HOUR_MASK; /*Clear 12-24 Bit and am-pm Bit*/
            Copy_stTimeVal->Hours = ConvertTo_Decimal(Local_u8TimeArr[HOUR]);
    	}
        Local_u8TimeArr[SECOND] &= SECOND_MASK; /*Clear CH Bit*/
        Copy_stTimeVal->Seconds = ConvertTo_Decimal(Local_u8TimeArr[SECOND]);
        Copy_stTimeVal->Minuites = ConvertTo_Decimal(Local_u8TimeArr[MINUIT]);
        Copy_stTimeVal->Day_of_Week = Local_u8TimeArr[DAY];
        Copy_stTimeVal->Date = ConvertTo_Decimal(Local_u8TimeArr[DATE]);
        Copy_stTimeVal->Month = ConvertTo_Decimal(Local_u8TimeArr[MONTH]);
        Copy_stTimeVal->Year = ConvertTo_Decimal(Local_u8TimeArr[YEAR]);
    }
    else
    {
   	 Local_u8ErrorStatus = NULL_PTR_ERR;
    }
    return Local_u8ErrorStatus;
}

uint8_t ConvetTo_BCD(uint8_t Copy_u8DecimalVal)
{
	uint8_t Local_u8RightDigit = Copy_u8DecimalVal % 10 ; /*Getting Right Digit*/
	uint8_t Local_u8LeftDigit  = Copy_u8DecimalVal /10 ;  /*Getting left Digit*/
	uint8_t Local_u8BCDVal=0;
    Local_u8BCDVal = Local_u8RightDigit | (Local_u8LeftDigit << 4) ; /*Converting to BCD*/

    return Local_u8BCDVal;
}
uint8_t ConvertTo_Decimal(uint8_t Copy_u8BCDVal)
{
	uint8_t Local_u8RightDigit = Copy_u8BCDVal & 0b00001111 ; /*Getting Right Digit*/
	uint8_t Local_u8LeftDigit  = (Copy_u8BCDVal >> 4) & 0b00001111 ;  /*Getting left Digit*/
	uint8_t Local_u8DecimalVal=0;
	Local_u8DecimalVal = Local_u8LeftDigit * 10 + Local_u8RightDigit; /*Converting to Decimal*/
	return Local_u8DecimalVal;
}
