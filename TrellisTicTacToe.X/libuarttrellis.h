/*
 * Date: 4/22/2024
 * Main Author(s): Joel Kuehne
 * Course number: EE 2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Top level library used on the PIC side. Wraps up
 * UART command reception and processing into two different functions:
 * poll_and_update() and process_uart(). Turns the general purpose libtrellis,
 * libi2c, and libuart libraries into something specific for our application.
 */

#ifndef LIBBLUETRELLIS_H
#define	LIBBLUETRELLIS_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    /**
     * Initializes i2c communication with the I2C2 peripheral as well as 38400
     * baud UART communication with peripheral U1 with TX on RB7 and RX on RB8.
     * Also sends i2c initializing commands to the LCD and trellis over i2c.
     * Initializes TMR3 as a frame timer for the trellis.
     */
    void bluetrellis_init(void);
    
    /**
     * Polls for button presses and sends a draw/show command to the display.
     * This function runs at a period of 20ms or greater. If 20ms has not
     * passed since the last call, the function will perform a blocking delay
     * until it has. Sends all button events over UART immediately.
     */
    void poll_and_update(void);
    
    /**
     * Handles commands sent over UART from the bluetooth device.
     */
    void process_uart(void);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LIBUARTTRELLIS_H */

