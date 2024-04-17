/*
 * Date: 3/21/2024
 * Name: Joel Kuehne
 * Student ID number: 5751841
 * Course number: EE 2361
 * Term: Spring 2024
 * Lab/assignment number: Lab 5
 * Short Program Description: Header for a library that handles writes to an
 * I2C bus. Allows for buffered, non-blocking transmission by using interrupts.
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LIBLCD_H
#define	LIBLCD_H

#include <xc.h>

#include "libi2c.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    /**
	 * Writes an lcd command to a circular buffer to be sent over I2C.
     * @param pkg The payload for the command.
	 */
	void lcd_cmd_async(char pkg);

	/**
	 * Initializes the lcd. Should only be run once at the beginning of the
     * initialization of the I2C bus.
	 */
	void lcd_init(void);

	/**
	 * Writes a set_cursor command to the buffer to be sent over I2C.
     * @param row The new row of the cursor.
     * @param col The new column of the cursor.
	 */
	void lcd_set_cursor(char row, char col);

	/**
	 * Writes a putc command to the buffer to be sent over I2C
     * @param c The character to put to the buffer.
	 */
	void lcd_putc(unsigned char c);
    
    /**
     * Draws a string of 8 characters to the LCD.
     * @param string The string to be drawn.
     */
    void lcd_puts(unsigned char string[]);
    
    /**
     * Draws a buffer of bytes to the lcd display.
     * @param string The new set of characters to be drawn.
     */
    void lcd_set_display(char buffer[2][8]);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LIBUARTTRELLIS_H */

