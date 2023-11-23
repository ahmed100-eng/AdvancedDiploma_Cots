/*************************************************/
/*@file GPIO_prog.c
 * @author Ahmed Abdelaziz
 * @brief The GPIO main source file, including functions definitions
 *
 */


#include <stdint.h>
#include "stm32f446xx.h"
#include "ErrorTyp.h"

#include "GPIO_interface.h"
#include "GPIO_priv.h"

static GPIO_RegDef_t *GPIOPORT[GPIO_PERIPHERAL_NUM] = {GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOH};

/********************************************/
/* @fn GPIO_u8PinInit
 * @brief the function initializes the GPIO pin according to the input parameters
 * @param[in] PinConfig : the initialization variables of the pins
 * @retval ErrorStatus
 *
 */


uint8_t GPIO_u8PinInit(const GPIO_PinConfig_t *PinConfig)
{
	uint8_t Local_u8ErrorStaus = OK;
    if(PinConfig != NULL)
    {
    	if((PinConfig->Port <= PORTH) && (PinConfig->PinNum <= PIN15))
    	{
            /*Select GPIO mode : Input , Output , Analog , Alternate function */
    		(GPIOPORT[PinConfig->Port]->MODER) &= ~(MODER_MASK << ((PinConfig->PinNum) * MODER_PIN_ACCESS));
    		(GPIOPORT[PinConfig->Port]->MODER) |= ((PinConfig ->Mode) << ((PinConfig->PinNum) * MODER_PIN_ACCESS));

    		/*Select GPIO Pull state : Pull up , Pull down, No Pull*/
    		(GPIOPORT[PinConfig->Port]->PUPDR) &= ~(PUPDR_MASK << ((PinConfig->PinNum)* PUPDR_PIN_ACCESS));
    		(GPIOPORT[PinConfig->Port]->PUPDR) |= ((PinConfig ->PullType) << ((PinConfig->PinNum) * PUPDR_PIN_ACCESS));

    		/*Select Output type & Output speed in case of general purpose output or Alternate function*/
    		if((PinConfig ->Mode == OUTPUT) || (PinConfig->Mode == ALTERNATE_FUNCTION))
    		{
    			/*Select Output type : Push pull , Open drain*/
    			(GPIOPORT[PinConfig->Port]->OTYPER) &= ~(OTYPER_MASK << (PinConfig->PinNum));
    			(GPIOPORT[PinConfig->Port]->OTYPER) |= ((PinConfig->OutputType) << (PinConfig->PinNum));

    			/*Select output speed : Low, Medium, High, Fast*/
    			(GPIOPORT[PinConfig->Port]->OSPEEDR) &= ~(OSPPEEDR_MASK << (PinConfig->PinNum) * OSPPEEDR_PIN_ACCESS);
    			(GPIOPORT[PinConfig->Port]->OSPEEDR) |= ((PinConfig->Speed) << ((PinConfig->PinNum) * OSPPEEDR_PIN_ACCESS));

    			/*Select the pin Alternate function*/
    			if(PinConfig->Mode == ALTERNATE_FUNCTION)
    			{
    				uint8_t Local_u8RegNum = (PinConfig->PinNum) /AFR_ACCESS;
    				uint8_t Local_u8BitNum = (PinConfig->PinNum) %AFR_ACCESS;
    				(GPIOPORT[PinConfig->Port]->AFR[Local_u8RegNum]) &= ~(AFR_MASK << Local_u8BitNum * AFR_PIN_ACCESS);
    				(GPIOPORT[PinConfig->Port]->AFR[Local_u8RegNum]) |= ((PinConfig->AlFunc) << (Local_u8BitNum * AFR_PIN_ACCESS));
    			}
    		}
    	}
    	else
    	{
    		Local_u8ErrorStaus = NOK;
    	}
    }
    else
    {
    	Local_u8ErrorStaus = NULL_PTR_ERR;
    }
	return Local_u8ErrorStaus;
}

/***************************************************************/
/*@fn GPIO_u8SetPinValue
 * @brief the function outputs a certain value on an output pin
 * @param[in] port : the port number, get options @Port_t enum
 * @param[in] PinNum : the pin number, get options @Pin_t enum
 * @param[in] PinVal : the pin value, get options @PinVal_t enum
 * @retval ErrorStatus
 */
uint8_t GPIO_u8SetPinValue(Port_t Port, Pin_t PinNum, PinVal_t PinVal)
{
	uint8_t Local_u8ErrorStatus = OK;
	if((Port <= PORTH) && (PinNum <= PIN15))
	{
		if(PinVal == PIN_LOW)
		{
			GPIOPORT[Port]->ODR &= ~(1<< PinNum);
			/*GPIO[Copy_Port] -> BSRR = 1 << (16 + PinNum);*/
		}
		else if (PinVal == PIN_HIGH)
		{
			GPIOPORT[Port]->ODR |= (1<< PinNum);
			/*GPIO[Copy_Port] -> BSRR = 1 <<  PinNum;*/
		}
		else
		{
			Local_u8ErrorStatus = NOK;
		}
	}
	else
	{
		Local_u8ErrorStatus = NOK;
	}
	return Local_u8ErrorStatus;
}

