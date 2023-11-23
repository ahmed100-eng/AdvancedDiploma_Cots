/*
 * I2C_private.h
 *
 *  Created on: Aug 21, 2023
 *      Author: Ahmed Abdelaziz
 */

#ifndef I2C_PRIVATE_H_
#define I2C_PRIVATE_H_

#define START_BIT                   8u
#define STOP_BIT                    9u
#define TXE_BIT                     7u
#define RXE_BIT                     6u
#define POS_BIT                     11u
#define BTF_BIT                     2u
#define OAR1_MASK                   0x00004000u
#define OAR1_7BIT_ADDRESS_ACCESS    1u
#define SPEED_MOOD_ACCESS   15u
#define CCR_MASK                    0x0000u
#define CR2_MASK                    0x0000u
#define CR1_MASK                    0x0000u
#define DMA_BIT                     11u
#define LAST_BIT                    12u
#define ITBUFF_BIT                  10u
#define ACK_BIT                     10u
#define TRISE_MASK                  0x0000
#define SM_CCR_MUL                  5u
#define FM_CCR_MUL                  125u
#define FM_CCR_DIV                  100u
#define FM_TRISE_MUL                3u
#define FM_TRISE_DIV                10u
#define TRISE_KEY                   1u
#define ENABLE                      1u
#define ADDR_ACCESS                 1u
#define R_W_ACCESS                  1u
#define TIME_OUT                    50000u
#define RECEIVE_2BYTES              2u
#define LAST_2BYTES                 2u
#endif /* I2C_PRIVATE_H_ */
