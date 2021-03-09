/*
===============================================================================
 Name        : lcd_i2c_lpc1769_example.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "headers.h"

void SysInit(void);


int main(void) {

	SysInit();

	DEBUGOUT("> Main.");

	LCD_Set_Cursor(0,0);
	LCD_write_string("Hello World!");

    while(1) {
    	/* check systick handler */
    	/* do something */
    }
    return 0 ;
}

void SysInit(void)
{
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();

	SysTick_Init(TICKRATE_HZ1);

	I2C_Init();
	LCD_Init();

}
