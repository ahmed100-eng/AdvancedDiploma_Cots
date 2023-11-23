/*
 * SysTick_interface.h
 *
 *  Created on: May 17, 2023
 *      Author: Ahmed Abdelaziz
 */

#ifndef SYSTICK_INTERFACE_H_
#define SYSTICK_INTERFACE_H_


#define SYSTICK_CSR_COUNTFLAG        16u
#define SYSTICK_CSR_CLKSRC           2u
#define SYSTICK_CSR_TICKINT          1u
#define SYSTICK_CSR_ENABLE           0u



void Delay_ms(uint16_t Copy_u16Value);

void Delay_us(uint16_t Copy_u16Value);

#endif /* SYSTICK_INTERFACE_H_ */
