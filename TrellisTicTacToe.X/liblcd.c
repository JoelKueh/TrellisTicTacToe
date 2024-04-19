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
void lcd_cmd(uint8_t pkg)
{
    const uint8_t prefix = CNTRL_CMD;
    i2c_send(LCD_ADDR, &prefix, 1, &pkg, 1);
}

/**
 * Pushes a set_cursor command to the buffer.
 */
void lcd_set_cursor(uint8_t row, uint8_t col)
{
	char pkg = 0x80 | (row << 6) | col;
	lcd_cmd(pkg);
}

/**
 * Pushes a print_character command to the buffer.
 */
void lcd_putc(uint8_t c)
{
    const uint8_t prefix = CNTRL_PUTC;
    i2c_send(LCD_ADDR, &prefix, 1, &c, 1);
}

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
 * Initializes the LCD display
 */
void lcd_init()
{
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
    
    // DEBUG
    lcd_set_cursor(0, 0);
    lcd_putc('a');
}
