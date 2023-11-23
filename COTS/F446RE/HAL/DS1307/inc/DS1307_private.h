/*
 * DS1037_private.h
 *
 *  Created on: Aug 25, 2023
 *      Author: Ahmed abdelaziz
 */

#ifndef DS1037_PRIVATE_H_
#define DS1037_PRIVATE_H_

#define DS1037_ADDRESS          0b1101000
#define DS1307_REGISTER_ADDRESS 0x00u

#define BIT_12_24_DISPLAY       6u
#define AM_PM_BIT               5u

#define SECOND                  0u
#define MINUIT                  1u
#define HOUR                    2u
#define DAY                     3u
#define DATE                    4u
#define MONTH                   5u
#define YEAR                    6u
#define CH_BIT                  7u
#define DS1037_BUFFER_SIZE      7u
#define HOUR_MASK               0b00011111
#define SECOND_MASK             0b01111111
#endif /* DS1037_PRIVATE_H_ */
