/*
 * Date: 4/22/2024
 * Main Author(s): Debra Johnson, Greta Shields, Alejandro Jimenez
 * Refactored By: Joel Kuehne
 * Course number: EE 2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Lower level library that wraps up a set of
 * commands that can be sent to the lcd.
 */

#include "liblcd.h"
#include "utills.h"

#define LCD_ADDR 0b01111100
#define CNTRL_CMD 0b00000000
#define CNTRL_PUTC 0b01000000
#define CNTRL_PUTS 0b11000000

#define LCD_CONTRAST 0b011100

/**
 * Sends an lcd command over i2c.
 * @param pkg The payload for the command.
 */
void lcd_cmd(uint8_t pkg)
{
    const uint8_t prefix = CNTRL_CMD;
    i2c_send(LCD_ADDR, &prefix, 1, &pkg, 1);
}

/**
 * Sends a lcd set cursor command over i2c.
 * @param row The new row of the cursor.
 * @param col The new column of the cursor.
 */
void lcd_set_cursor(uint8_t row, uint8_t col)
{
	char pkg = 0x80 | (row << 6) | col;
	lcd_cmd(pkg);
}

/**
 * Sends a putc command to the lcd (sets a single character)
 * @param c The character to put to the buffer.
 */
void lcd_putc(uint8_t c)
{
    const uint8_t prefix = CNTRL_PUTC;
    i2c_send(LCD_ADDR, &prefix, 1, &c, 1);
}

/**
 * Sends a puts command to the lcd (sets a full string)
 * @param string The string to be drawn.
 */
void lcd_puts(uint8_t string[])
{
    int i, len;
    uint8_t data[16];

    i = 0;
    while (string[i] != '\0' && i < 8) {
        data[i * 2] = CNTRL_PUTS;
        data[i * 2 + 1] = string[i];
        i++;
    }
    data[i * 2 - 2] = CNTRL_PUTC;
    len = i * 2;
    
    i2c_send(LCD_ADDR, 0, 0, data, len);
}

/**
 * Initializes the lcd. Must not be called before i2c_init() from libi2c.
 */
void lcd_init()
{
    // Send initialization commands...
	delay_ms(40);

	lcd_cmd(0b00111000);
	delay_us(27);
	lcd_cmd(0b00111001);
	delay_us(27);
	lcd_cmd(0b00010100);
	delay_us(27);
	lcd_cmd(0b01110000 | (LCD_CONTRAST & 0b1111));
	delay_us(27);
	lcd_cmd(0b01010100 | ((LCD_CONTRAST & 0b110000) >> 4));
	delay_us(27);
    lcd_cmd(0b01101100);

	delay_ms(200);

	lcd_cmd(0b00111000);
	delay_us(27);
	lcd_cmd(0b00001100);
	delay_us(27);
	lcd_cmd(0b00000001);

	delay_ms(2);

	_MI2C2IF = 0;
}
