/*
 * Date: 4/22/2024
 * Main Author(s): Debra Johnson
 * Refactored By: Joel Kuehne
 * Course number: EE 2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Lower level library that wraps up a set of
 * commands that can be sent to the lcd.
 */

#ifndef LIBLCD_H
#define	LIBLCD_H

#include <xc.h>

#include "libi2c.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    /**
	 * Sends an lcd command over i2c.
     * @param pkg The payload for the command.
	 */
	void lcd_cmd(uint8_t pkg);

	/**
	 * Initializes the lcd. Must not be called before i2c_init() from libi2c.
	 */
	void lcd_init(void);

	/**
	 * Sends a lcd set cursor command over i2c.
     * @param row The new row of the cursor.
     * @param col The new column of the cursor.
	 */
	void lcd_set_cursor(uint8_t row, uint8_t col);

	/**
	 * Sends a putc command to the lcd (sets a single character)
     * @param c The character to put to the buffer.
	 */
	void lcd_putc(uint8_t c);
    
    /**
     * Sends a puts command to the lcd (sets a full string)
     * @param string The string to be drawn.
     */
    void lcd_puts(uint8_t string[]);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LIBLCD_H */

