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
#ifndef LIBTRELLIS_H
#define	LIBTRELLIS_H

#define MAX_READ_SIZE 32

#define EDGE_NONE       0b00
#define EDGE_RISING     0b01
#define EDGE_FALLING    0b10
#define EDGE_BOTH       0b11

#include <xc.h>

#include "libi2c.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    void trellis_init(void);
    
    void set_recorded_edges(char num, char edges);
    void get_button_events(unsigned char *buffer, int max_size);
    
    void set_led(char num, unsigned char g, unsigned char r, unsigned char b);
    void set_display(char num, unsigned char colors[16][3]);
    void display_show(void);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LIBTRELLIS_H */
