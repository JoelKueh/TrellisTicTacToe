/*
 * File:   liblcd.c
 * Author: joel
 *
 * Created on April 9, 2024, 10:02 PM
 */

#include "liblcd.h"
#include "utills.h"

#define LCD_ADDR 0b01111100
#define CNTRL_CMD 0b00000000
#define CNTRL_PUTC 0b01000000
#define CNTRL_PUTS 0b11000000

#define LCD_CONTRAST 0b011100

/**
 * Pushes an LCD command to the buffer.
 */
void lcd_cmd_async(char pkg)
{
	volatile struct i2c_command *cmd = i2c_queue_get_top();

	// Wait until there is room in the buffer
	while (i2c_queue_full());

	// Fill in the data for the command
	cmd->data[0] = LCD_ADDR;
	cmd->data[1] = CNTRL_CMD;
	cmd->data[2] = pkg;
	cmd->len = 3;

	// Push the command
	i2c_queue_push();
}

/**
 * Pushes a set_cursor command to the buffer.
 */
void lcd_set_cursor(char row, char col)
{
	char pkg = 0x80 | (row << 6) | col;
	lcd_cmd_async(pkg);
}

/**
 * Pushes a print_character command to the buffer.
 */
void lcd_putc(char c)
{
	volatile struct i2c_command *cmd = i2c_queue_get_top();

	while (i2c_queue_full());

	// Fill in the data for the command
	cmd->data[0] = LCD_ADDR;
	cmd->data[1] = CNTRL_PUTC;
	cmd->data[2] = c;
	cmd->len = 3;

	// Complete the push
	i2c_queue_push();
}

void lcd_puts(char string[])
{
    volatile struct i2c_command *cmd = i2c_queue_get_top();
    int i;

	while (i2c_queue_full());

	// Fill in the data for the command
	cmd->data[0] = LCD_ADDR;
    
    i = 0;
    while (string[i] != '\0' && i < 9) {
        cmd->data[(i << 1) + 1] = CNTRL_PUTS;
        cmd->data[(i << 1) + 2] = string[i];
        i++;
    }
    cmd->data[(i << 1) - 1] = CNTRL_PUTC;
    cmd->len = (i << 1) + 1;

	// Complete the push
	i2c_queue_push();
}

/**
 * Sends a command without the ISR. Should only be used in initialization.
 */
void lcd_cmd_block(char pkg)
{
	IFS3bits.MI2C2IF = 0;
	I2C2CONbits.SEN = 1;
	while (I2C2CONbits.SEN);
	IFS3bits.MI2C2IF = 0;
	I2C2TRN = 0b01111100;
	while (!IFS3bits.MI2C2IF);
	IFS3bits.MI2C2IF = 0;
	I2C2TRN = 0b00000000;
	while (!IFS3bits.MI2C2IF);
	IFS3bits.MI2C2IF = 0;
	I2C2TRN = pkg;
	while (!IFS3bits.MI2C2IF);
	I2C2CONbits.PEN = 1;
	while (I2C2CONbits.PEN);
	IFS3bits.MI2C2IF = 0;
}

/**
 * Initializes the LCD display
 */
void lcd_init()
{
	_MI2C2IE = 0;

	delay_ms(40);

	lcd_cmd_block(0b00111000);
	delay_us(27);
	lcd_cmd_block(0b00111001);
	delay_us(27);
	lcd_cmd_block(0b00010100);
	delay_us(27);
	lcd_cmd_block(0b01110000 | (LCD_CONTRAST & 0b1111));
	delay_us(27);
	lcd_cmd_block(0b01010100 | ((LCD_CONTRAST & 0b110000) >> 4));
	delay_us(27);
    lcd_cmd_block(0b01101100);

	delay_ms(200);

	lcd_cmd_block(0b00111000);
	delay_us(27);
	lcd_cmd_block(0b00001100);
	delay_us(27);
	lcd_cmd_block(0b00000001);

	delay_ms(2);

	_MI2C2IF = 0;
	_MI2C2IE = 1;
}
