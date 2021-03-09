/*
 * display1602_i2c_lpc1769.c
 *
 *  Created on: 9 mar. 2021
 *      Author: ealegremendoza
 */
#include "headers.h"


xDisplayI2C LCD;			// To operate the LCD.
static int mode_poll;   	// Poll/Interrupt mode flag
uint8_t  LCD_BL_Status = LCD_BACKLIGHT_ON;     // 1: BACKLIGTH ON, 0 for NEGATIVE control

/**@brief	Initialice I2C peripheral
 * @param	Nothing
 * @return	Nothing
 */
void I2C_Init (void)
{
	Chip_IOCON_PinMux(LPC_IOCON, LCD_I2C_SDA_PORT,LCD_I2C_SDA_PIN,
						LCD_SDA_MODE, LCD_SDA_FUNC);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_I2C_SCL_PORT,LCD_I2C_SCL_PIN,
						LCD_SCL_MODE, LCD_SCL_FUNC);
	Chip_IOCON_EnableOD(LPC_IOCON, LCD_I2C_SDA_PORT,LCD_I2C_SDA_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON, LCD_I2C_SCL_PORT,LCD_I2C_SCL_PIN);

	/* Initialize I2C */
	Chip_I2C_Init(I2C0);
	Chip_I2C_SetClockRate(I2C0, SPEED);

	/* Set default mode to interrupt */
	i2c_set_mode(I2C0, 0);
}

/**
 * @brief	Initialice LCD 16x02 Whit i2c backpack PCF8574A
 * @param	Nothing
 * @return	Nothing
 */
void LCD_Init (void)
{
	LCD_delay(DELAY_20MS);	// crude delay
	LCD.RS	= 0;
    LCD.RW	= 0;
    LCD.EN	= 0;
    LCD.BL	= LCD_BL_Status;

    LCD_write_upper_nibble(FUNCTION_RESET_8BIT);
   	LCD_write_upper_nibble(FUNCTION_RESET_8BIT);
    LCD_write_upper_nibble(FUNCTION_RESET_4BIT);

	LCD_write_byte(0x00, FUNCTION_SET_1);
	LCD_write_byte(0x00, FUNCTION_SET_2);
	LCD_write_byte(0x00, FUNCTION_SET_3);
	LCD_write_byte(0x00, FUNCTION_SET_4);
	LCD_write_byte(0x00, FUNCTION_SET_5);
}

/**
 * @brief	Send a byte to the LCD
 * @param	address	:	0:command 1:data
 * @param	b		:	byte to send
 * @return	Nothing
 */
void LCD_write_byte(uint8_t address, uint8_t b)
{
    if (address)
    {
        LCD.RS	= 1;   // Data
    }
    else
    {
    	LCD.RS	= 0;   // Command
    }

    LCD.RW	= 0;
    LCD.EN	= 0;
    LCD.BL	= LCD_BL_Status;


    // Send upper nibble
    LCD_write_upper_nibble(b);
    // Send lower nibble
    LCD_write_lower_nibble(b);
}

/**
 * @brief	Send upper nibble to the LCD
 * @param	u		: 	data byte to send
 * @return	Nothing
 */
void LCD_write_upper_nibble(uint8_t upper)
{
    if(bit_test(upper,7))
        LCD.D7=1;
    else
        LCD.D7=0;

    if(bit_test(upper,6))
        LCD.D6=1;
    else
    	LCD.D6=0;

    if(bit_test(upper,5))
    	LCD.D5=1;
    else
    	LCD.D5=0;

    if(bit_test(upper,4))
    	LCD.D4=1;
    else
    	LCD.D4=0;
   LCD.EN = 0;

   Chip_I2C_MasterSend(I2C_SEL, DEVICE_I2C_ADDRESS, &LCD.data_byte, 1);
   LCD.EN = 1;

   Chip_I2C_MasterSend(I2C_SEL, DEVICE_I2C_ADDRESS, &LCD.data_byte, 1);
   LCD.EN = 0;
   Chip_I2C_MasterSend(I2C_SEL, DEVICE_I2C_ADDRESS, &LCD.data_byte, 1);

}

/**
 * @brief	Send lower nibble to the LCD
 * @param	l		: 	data byte to send
 * @return	Nothing
 */
void LCD_write_lower_nibble(uint8_t lower)
{
    if(bit_test(lower,3))
    	LCD.D7=1;
    else
    	LCD.D7=0;

    if(bit_test(lower,2))
    	LCD.D6=1;
    else
    	LCD.D6=0;

    if(bit_test(lower,1))
    	LCD.D5=1;
    else
    	LCD.D5=0;

    if(bit_test(lower,0))
    	LCD.D4=1;
    else
    	LCD.D4=0;

    LCD.EN = 0;
    Chip_I2C_MasterSend(I2C_SEL, DEVICE_I2C_ADDRESS, &LCD.data_byte, 1);
    LCD.EN = 1;
    Chip_I2C_MasterSend(I2C_SEL, DEVICE_I2C_ADDRESS, &LCD.data_byte, 1);
    LCD.EN = 0;
    Chip_I2C_MasterSend(I2C_SEL, DEVICE_I2C_ADDRESS, &LCD.data_byte, 1);
}



/**
 * @brief	Set I2C mode to polling/interrupt
 * @param	id		: 	I2C id
 * @param	polling : 	polling mode
 * @return	Nothing
 */
static void i2c_set_mode(I2C_ID_T id, int polling)
{
	if(!polling) {
		mode_poll &= ~(1 << id);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandler);
		NVIC_EnableIRQ(id == I2C0 ? I2C0_IRQn : I2C1_IRQn);
	} else {
		mode_poll |= 1 << id;
		NVIC_DisableIRQ(id == I2C0 ? I2C0_IRQn : I2C1_IRQn);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandlerPolling);
	}
}


void I2C0_IRQHandler(void)
{
	i2c_state_handling(I2C0);
}

/**
 * @brief	State machine handler for I2C0 and I2C1
 * @param	id		: 	I2C id
 * @return	Nothing
 */
static void i2c_state_handling(I2C_ID_T id)
{
	if (Chip_I2C_IsMasterActive(id)) {
		Chip_I2C_MasterStateHandler(id);
	} else {
		Chip_I2C_SlaveStateHandler(id);
	}
}

/**
 * @brief	Position of the next character on the display.
 * @param	col		: 	col 0...15 (16X2 DISPLAY)
 * @param	row		: 	line 1 or line 2 (16X2 DISPLAY)
 * @return	Nothing
 */
void LCD_Set_Cursor(uint8_t col, uint8_t row)
{
	uint8_t address;
	switch(row)
    {
		case 0:
			address = LCD_LINE_1_ADDRESS;
			break;

		case 1:
			address = LCD_LINE_2_ADDRESS;
			break;

		default:
			address = LCD_LINE_1_ADDRESS;
			break;
    }

	address += col;
	LCD_write_byte(0, 0x80 | address);
}

/**
 * @brief	Set backlight status.
 * @param	status		: 	0:OFF 1:ON
 * @return	Nothing
 */
void LCD_BL(uint8_t status)
{
    LCD_BL_Status = status;
    LCD_write_byte(0x00, 0x00);
}
/**
 * @brief	Clear entire display.
 * @param	Nothing
 * @return	Nothing
 */
void LCD_clear()
{
    LCD_write_byte(0x00,0x01);
    LCD_delay(DELAY_5MS);
}

/**
 * @brief	Clear line.
 * @param	line		:	line to clear
 * @return	Nothing
 */
void LCD_clear_line(uint8_t line)
{
	LCD_Set_Cursor(0,line);
    for (int i = 0; i<20; i++)
    {
        LCD_write_string(" ");
    }
    LCD_Set_Cursor(0,line);
}
/**
 * @brief	Write a string text to LCD via I2C.
 * @param	str		: 	pointer to character string
 * @return	Nothing
 */
void LCD_write_string(const char *str)
{
   // Writes a string text[] to LCD via I2C
   LCD.RS  = 1;
   LCD.RW  = 0;
   LCD.EN   = 0;
   LCD.BL  = LCD_BL_Status;

   while (*str)
   {
        // Send upper nibble
        LCD_write_upper_nibble(*str);
        // Send lower nibble
        LCD_write_lower_nibble(*str);

        str++;
   }
}

void LCD_delay (uint32_t delay)
{
	uint32_t i;
	for(i=0;i<delay;i++);
}
