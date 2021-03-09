/*
 * 	display1602_i2c_lpc1769.h
 *
 *  Created on: 9 mar. 2021
 *      Author: ealegremendoza
 */


#ifndef SRC_TASKS_TASK_DISPLAY1602_I2C_LPC1769_H_
#define SRC_TASKS_TASK_DISPLAY1602_I2C_LPC1769_H_


#define LCD_I2C_SDA_PORT	0
#define LCD_I2C_SDA_PIN		27
#define LCD_SDA_MODE		IOCON_MODE_INACT
#define LCD_SDA_FUNC		IOCON_FUNC1
#define LCD_I2C_SCL_PORT	0
#define LCD_I2C_SCL_PIN		28
#define LCD_SCL_MODE		IOCON_MODE_INACT
#define LCD_SCL_FUNC		IOCON_FUNC1

#define I2C_SEL				I2C0	//I2C1 //I2C2
#define	DEFAULT_I2C			I2C0
#define SPEED_100KHZ		100000
#define	SPEED				SPEED_100KHZ

#define LCD_I2C_ADDRESS		0X27//0X27 //0X20 //0X38 //0X3F
#define DEVICE_I2C_ADDRESS	LCD_I2C_ADDRESS


#define LCD_LINE_1_ADDRESS 	0x00
#define LCD_LINE_2_ADDRESS 	0x40

#define LCD_BACKLIGHT_OFF	0
#define LCD_BACKLIGHT_ON	1



/* LCD 1602 i2c INSTRUCTIONS */
#define	LINE1				0X0		// 0 for 1/8 duty cycle -- 1 `line'
#define	LINE2				0X1		// 1 for 1/16 duty cycle -- 2 `lines'
#define	LINE				LINE2

#define	FONT1				0X0		// 0 for 5x8 dot matrix
#define	FONT2				0X1		// 1 for 5x11 dot matrix
#define	FONT				FONT2

#define BIT_OFF				0X0
#define BIT_ON				0X1

#define	CURSOR				BIT_OFF
#define	CBLINK				BIT_OFF
#define	DISP				BIT_ON

#define FUNCTION_RESET_8BIT	0X30						// starts by default in 8 bit
#define FUNCTION_RESET_4BIT	0X20						// change to 4 bit
#define FUNCTION_SET_1		0X20|(LINE<<3)|(FONT<<2)	// Line and font
#define FUNCTION_SET_2		0X08						// Display Off, Cursor Off, Blink Off
#define FUNCTION_SET_3		0X01						// Clear Screen & Returns the Cursor Home
#define FUNCTION_SET_4		0X06						// Inc cursor to the right when writing
#define	FUNCTION_SET_5		0X08|(DISP<<2)|(CURSOR<<1)|(CBLINK<<0)	// Display ON

#define DISPLAY_CTRL		0x0F
#define SHIFT_CURSOR 		0x06
#define DISPLAY_CLR 		0x01

#define testbit(var, bit)   ((var) & (1 <<(bit)))
#define bit_test    		testbit

#define LEN_BUF				(17)	// LCD LINE TXT BUFFER

/* valores para delay crudo*/
#define	DELAY_1MS	4445	//4445	4445
#define DELAY_2MS	9014	//8890	8952
#define DELAY_3MS	13583	//13335	13459
#define DELAY_4MS	18152	//17780	17966
#define	DELAY_5MS	22722	//22225	22473
#define DELAY_6MS	27291	//26670	26980
#define DELAY_7MS	31861	//31115	31488
#define DELAY_8MS	36430	//35560	35995
#define DELAY_9MS	40999	//40005	40502
#define	DELAY_10MS	45569	//44450	45009
#define	DELAY_15MS	68416	//66675	67545
#define DELAY_20MS 	91262	//88900	90081
#define DELAY_500MS	DELAY_10MS*50

/* valores para delay por timer - systick 1ms*/
#define SYSTICKBASEms	1
#define TIME_1SEG		SYSTICKBASEms*1000
#define TIME_500ms		SYSTICKBASEms*500
#define TIME_DELAY		TIME_500ms

//extern unsigned char uServoStartDelayflg; 	//1 delay activado, 0 delay desactivado
//extern unsigned char uServoDelayFlg;		// contador de ticks para el servo
//extern unsigned int  uServoCont; //contador para el ser
/**
 * @brief	Makes a crude delay
 * @param	c		: 	counts
 * @return	Nothing
 */
void LCD_delay (uint32_t delay);

typedef union {
	/* El modulo que genera la interfaz I2C es el PCF8574A */
    struct {
		unsigned char RS:1;			//P0	0:Command, 1:Data
		unsigned char RW:1;			//P1   	0:Write,  1: Read
		unsigned char EN:1;			//P2	Pulse
		unsigned char BL:1;			//P3	Backlight 0:off 1:on
		unsigned char D4:1;			//P4	B4|B0 -> 4 bit mode
		unsigned char D5:1;			//P5	B5|B1 -> 4 bit mode
		unsigned char D6:1;			//P6	B6|B2 -> 4 bit mode
		unsigned char D7:1;			//P7	B7|B3 -> 4 bit mode
    };
    unsigned char data_byte;
} xDisplayI2C;

/**@brief	Initialice I2C peripheral
 * @param	Nothing
 * @return	Nothing
 */
void I2C_Init(void);


/**
 * @brief	Initialice LCD 16x02 Whit i2c backpack PCF8574A
 * @param	Nothing
 * @return	Nothing
 */
void LCD_Init (void);

/**
 * @brief	Send a byte to the LCD
 * @param	address	:	0:command 1:data
 * @param	b		:	byte to send
 * @return	Nothing
 */
void LCD_write_byte(uint8_t address, uint8_t b);

/**
 * @brief	Send upper nibble to the LCD
 * @param	u		: 	data byte to send
 * @return	Nothing
 */
void LCD_write_upper_nibble(uint8_t u);

/**
 * @brief	Send lower nibble to the LCD
 * @param	l		: 	data byte to send
 * @return	Nothing
 */
void LCD_write_lower_nibble(uint8_t l);

/**
 * @brief	Position of the next character on the display.
 * @param	col		: 	col 0...15 (16X2 DISPLAY)
 * @param	row		: 	line 1 or line 2 (16X2 DISPLAY)
 * @return	Nothing
 */
void LCD_Set_Cursor(uint8_t col, uint8_t row);

/**
 * @brief	Write a string text to LCD via I2C.
 * @param	str		: 	pointer to character string
 * @return	Nothing
 */
void LCD_write_string(const char *str);


/**
 * @brief	Set I2C mode to polling/interrupt
 * @param	id		: 	I2C id
 * @param	polling : 	polling mode
 * @return	Nothing
 */
static void i2c_set_mode(I2C_ID_T id, int polling);

/**
 * @brief	State machine handler for I2C0 and I2C1
 * @param	id		: 	I2C id
 * @return	Nothing
 */
static void i2c_state_handling(I2C_ID_T id);

/**
 * @brief	Set backlight status.
 * @param	status		: 	0:OFF 1:ON
 * @return	Nothing
 */
void LCD_BL(uint8_t status);

/**
 * @brief	Clear line.
 * @param	line		:	line to clear
 * @return	Nothing
 */
void LCD_clear_line(uint8_t line);

/**
 * @brief	Clear entire display.
 * @param	Nothing
 * @return	Nothing
 */
void LCD_clear();

#endif /* SRC_TASKS_TASK_DISPLAY1602_I2C_LPC1769_H_ */
