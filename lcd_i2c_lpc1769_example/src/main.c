/*
===============================================================================
 Name        : lcd_i2c_lpc1769_example.c
 Author      : ealegremendoza
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "headers.h"

void SysInit(void);
extern char  LCD_Buffer_g[LCD_CANT_LINES][LCD_BUFFER_SIZE];

int main(void) {

	SysInit();

	DEBUGOUT("> Main.");

	LCD_write_str_buffer(0,"Hello World");

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
