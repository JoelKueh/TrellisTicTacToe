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
#ifndef LIBBLUETRELLIS_H
#define	LIBBLUETRELLIS_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    void bluetrellis_init(void);
    void poll_buttons(void);
    void process_uart(void);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LIBUARTTRELLIS_H */

