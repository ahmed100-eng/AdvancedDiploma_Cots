/*
 * SysTick_prog.c
 *
 *  Created on: May 17, 2023
 *      Author: Ahmed Abdelaziz
 */

#include <stdint.h>
#include "stm32f446xx.h"
#include "ErrorTyp.h"

#include "SysTick_interface.h"
#include "SysTick_priv.h"


/*this function is used to make a delay time with melli second*/
void Delay_ms(uint16_t Copy_u16Value)
{
   uint32_t Local_NumOfTicks=0;
   Local_NumOfTicks = (uint32_t)Copy_u16Value * 22502u;
   SYSTICK->RVR = Local_NumOfTicks; /*Load the desired no.of ticks*/
   SYSTICK->CSR &= ~(1 << SYSTICK_CSR_CLKSRC); /*Enable AHB divided by 8 clock*/
   SYSTICK->CSR |= 1 << SYSTICK_CSR_ENABLE; /*Enable counting from load value*/
   while(((SYSTICK->CSR) >> SYSTICK_CSR_COUNTFLAG) == 0);
}
/*this function is used to make a delay time with micro second*/
void Delay_us(uint16_t Copy_u16Value)
{
	   uint32_t Local_NumOfTicks=0;
	   Local_NumOfTicks = (uint32_t)Copy_u16Value * 23u;
	   SYSTICK->RVR = Local_NumOfTicks; /*Load the desired no.of ticks*/
	   SYSTICK->CSR &= ~(1 << SYSTICK_CSR_CLKSRC); /*Enable AHB divided by 8 clock*/
	   SYSTICK->CSR |= 1 << SYSTICK_CSR_ENABLE; /*Enable counting from load value*/
	   while(((SYSTICK->CSR) >> SYSTICK_CSR_COUNTFLAG) == 0);
}
