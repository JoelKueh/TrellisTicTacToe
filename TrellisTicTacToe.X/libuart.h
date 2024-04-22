/*
 * Date: 4/22/2024
 * Main Author(s): Joel Kuehne
 * Course number: EE 2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Lower level library that simplifies the process
 * of using UART for asynchronous command transmission and reception.
 * Commands sent consist of a header and a body.
 */

#ifndef LIBUART_H
#define	LIBUART_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    /**
     * Initializes UART communication on pins RB7 and RB8 at 38400 baud.
     */
    void uart_init(void);
    
    /**
     * Checks if the receive buffer is empty.
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
     * Consumes and returns the header byte of the top command in the queue.
     * Blocks if there is no data in the UART buffer.
     * @return The header byte of the current command.
     */
    unsigned char get_command_header();
    
    /**
     * Unpacks 'bytes' bytes of data into a command data array.
     * Blocks until enough bytes are read from the UART buffer.
     * @param com The address of a command where the data should be inserted.
     * @param bytes The number of bytes to take from the buffer.
     */
    void get_command_body(unsigned char *dest, unsigned char bytes);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LIBUART_H */