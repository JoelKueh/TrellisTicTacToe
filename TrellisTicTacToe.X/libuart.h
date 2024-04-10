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
#ifndef LIBUART_H
#define	LIBUART_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    /**
     * Initializes UART communication on pins RB7 and RB8.
     */
    void uart_init(void);
    
    /**
     * Checks if the recieve buffer is empty.
     * @return 1 if the buffer has contents.
     */
    int uart_empty(void);
    
    /**
     * Send 'bytes' bytes of data over UART, preceeded by a command header.
     * @param header The header byte of the command.
     * @param data The address of the data to be sent.
     * @param bytes The number of data bytes in the command.
     */
    void send_command(unsigned char header, unsigned char *data, unsigned char bytes);
    
    /**
     * Consumes and returns the header byte of the top command.
     * @return The header byte of the current command.
     */
    unsigned char get_command_header();
    
    /**
     * Unpacks 'bytes' bytes of data into a command data array.
     * @param com The address of a command where the data should be inserted.
     * @param bytes The number of bytes to take from the buffer.
     */
    void get_command_body(unsigned char *dest, unsigned char bytes);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

