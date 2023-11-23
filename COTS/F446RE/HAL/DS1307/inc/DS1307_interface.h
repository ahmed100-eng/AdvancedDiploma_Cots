/*
 * DS1037_interface.h
 *
 *  Created on: Aug 24, 2023
 *      Author: Ahmed Abdelaziz
 */

#ifndef DS1037_INTERFACE_H_
#define DS1037_INTERFACE_H_


typedef enum
{
	DISPLAY_24,
	DISPLAY_12
}DS1037_Display_t;

typedef enum
{
	AM,
	PM,
	NO_AM_PM       /*if DISPLAY_24 method is chosen*/
}DS1037_MidDay_t;

typedef enum
{
	SUNDAY=1,
	MONDAY,
	TUESDAY,
	WEDENSDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY
}DS1037_DOW_t;

typedef struct
{
	uint8_t Seconds;
	uint8_t Minuites;
	uint8_t Hours;
	uint8_t Date;
	uint8_t Month;
	uint8_t Year;
	DS1037_Display_t Display_Method;    /*12 Hour  or  24Hour*/
	DS1037_DOW_t Day_of_Week;
	DS1037_MidDay_t MidDay;   /*AM  or  PM*/
}DS1037_Config_t;

uint8_t DS1037_SetTime(DS1037_Config_t *Copy_stTimeVal);
uint8_t DS1037_ReadTime(DS1037_Config_t *Copy_stTimeVal);
#endif /* DS1037_INTERFACE_H_ */
