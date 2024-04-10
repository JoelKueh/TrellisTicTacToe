/*
 * File:   libuart.c
 * Author: joel
 *
 * Created on March 29, 2024, 10:28 AM
 */

#include "xc.h"

#include "libuart.h"

#define IDX_BITS 8
#define ARRAY_SIZE (1 << IDX_BITS)
#define MAX_IDX (ARRAY_SIZE - 1)

/**
 * Describes a queue that holds a series of commands to be sent over the I2C
 * bus.
 */
struct uart_buffer {
	unsigned char data[ARRAY_SIZE];
	unsigned int w_idx;
	unsigned int r_idx;
	unsigned int cnt;
};
volatile struct uart_buffer tr_queue;
volatile struct uart_buffer rc_buff;

inline void buffer_push(struct uart_buffer &buff, char data)
{
    buff.data[buff.w_idx++] = data;
    buff.w_idx &= MAX_IDX;
    buff.cnt++;
}

inline char buffer_pop(struct uart_buffer &buff)
{
    char data = buff.data[buff.r_idx++] = data;
    buff.r_idx &= MAX_IDX;
    buff.cnt--;
    return data;
}

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void)
{
    _U1RXIF = 0;
    buffer_push(rc_buff, U1RXREG);
}

volatile int transmit_waiting = 1;
void __attribute__((__interrupt__, __auto_psv__)) _U1TXInterrupt(void)
{
    char data;
    _U1TXIF = 0;
    
    if (uart_buffer.cnt == 0) {
        transmit_waiting = 1;
        return;
    }
    
    data = buffer_pop(tr_queue);
    U1TXREG = data;
}

void uart_init(void) {
    // I think the following two lines are irrelevant. The UART doc
    // Page 3, when describing the UARTEN bit in UxMODE says that when UARTEN
    // is set, TRISx are ignored and instead UEN and UTXEN control pins.
    _TRISB6 = 0; // U1TX output
    _TRISB10 = 1; // U1RX input

    U1MODE = 0; // UEN<1:0> bits control the pins
    // U1BRG = 34; // 115200 baud,
    // U1MODEbits.BRGH = 1;
    U1MODEbits.BRGH = 0;
    U1BRG = 25; // 38400 baud (check the calculation with the datasheet)
    U1MODEbits.UEN = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;

    // Peripheral Pin Select 
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _RP6R = 0x0003; //RB6->UART1:U1TX; See Table 10-3 on P109 of the datasheet
    _U1RXR = 10; //RB10->UART1:U1RX;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    // Enable receive interrupt flag.
    _U1RXIF = 0;
    _U1RXIE = 1;
    
    // Enable transmit interrupt flag
    _U1TXIF = 0;
    _U1TXIE = 1;
}

/**
 * Send 'bytes' bytes of data over UART, preceeded by a command header.
 * @param header The header byte for the command.
 * @param data A pointer to the body of the command.
 * @param bytes The number of data bytes in the command.
 */
void send_command(unsigned char header, void *data, unsigned char bytes)
{
    // Push the command to the transmit queue.
    buffer_push(tr_queue, data);
    while (bytes--) {
        buffer_push(tr_queue, *data++);
    }
    
    // If the transmit interrupt is sleeping, jump into it to send a byte.
    if (transmit_waiting) {
        _U1TXIF = 1;
    }
}

/**
 * Consumes and returns the header byte of the top command.
 * @return The header byte of the current command.
 */
unsigned char get_command_header()
{
    return buffer_pop(rc_buff);
}

/**
 * Unpacks 'bytes' bytes of data into a command data array.
 * @param dest The where the data should be inserted.
 * @param bytes The number of bytes to take from the buffer.
 */
void get_command_body(void *dest, unsigned char bytes)
{
    // Read the data section of the command in the receive buffer.
    while (bytes--) {
        *dest++ = buffer_pop(rc_buff);
    }
}