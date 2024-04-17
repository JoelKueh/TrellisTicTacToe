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

#define EDGE_HIGH       0b00
#define EDGE_LOW        0b01
#define EDGE_RISING     0b10
#define EDGE_FALLING    0b11

#include <xc.h>

#include "libi2c.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    union key_event {
      struct {
        uint8_t edge : 2;
        uint8_t num : 6;
      };
      uint8_t raw;
    };
    
    union key_state {
      struct {
        uint8_t state : 1;
        uint8_t active : 4;
        uint8_t extra : 3;
      };
      uint8_t raw;
    };
    
    void trellis_init(void);
    void await_frame(void);
    
    void set_keypad_events(uint8_t num, uint8_t edges);
    int get_button_events(union key_event *buffer, uint8_t max_size);
    
    void set_led(uint8_t num, uint8_t g, uint8_t r, uint8_t b);
    void set_display(uint8_t colors[16][3]);
    void display_show(void);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LIBTRELLIS_H */
