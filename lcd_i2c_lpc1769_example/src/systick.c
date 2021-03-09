/*
 * systick.c
 *
 *  Created on: 9 mar. 2021
 *      Author: ealegremendoza
 */

#include "headers.h"
/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */

extern char  LCD_Buffer_g[LCD_CANT_LINES][LCD_BUFFER_SIZE];

void SysTick_Handler(void)
{
	static uint32_t Divisor_1sec=1000;
	static uint32_t Divisor_1min=60;
	static uint32_t counter = 0;
	char txt[3];
	Divisor_1sec--;
	if(0==Divisor_1sec)	//1 sec
	{
		Divisor_1sec=1000;
		Divisor_1min--;
		/* LCD's LINE0 UPDATE */
		LCD_Set_Cursor(0,0);
		LCD_Shift_Text(LCD_Buffer_g[0],SHIFT_RIGHT);
		LCD_write_string(LCD_Buffer_g[0]);

		/* LCD's LINE1 UPDATE */
		sprintf(txt,"%02d",counter);
		counter++;
		counter%=100;
		LCD_write_str_buffer(1,txt);


		if(0==Divisor_1min)	//1 min
		{
			Divisor_1min=60;
		}
	}
}



void SysTick_Init(uint32_t freq)
{
	if(freq > 0xffffff)
		return;
	/* Enable and setup SysTick Timer at a periodic rate */
	/* Configuro el systick para que de ticks cada 1ms con TICKRATE_HZ1=1000*/
	//SysTick_Config(ticks*100);
	SysTick_Config(SystemCoreClock / freq);
}


