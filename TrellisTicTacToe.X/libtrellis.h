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
#ifndef LIBUARTTRELLIS_H
#define	LIBUARTTRELLIS_H

#include <xc.h>

#include "libuart.h"

#define BUTTON_EVENT_HEADER 'A'
#define READ_RESPONSE_HEADER 'B'

#define READ_REQUEST_HEADER 'C'
#define SET_LED_HEADER 'D'
#define SET_LEDS_HEADER 'E'
#define SET_LCD_HEADER 'F'

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    struct button_event {
        unsigned char button_num;
        unsigned int button_states;
    };
    
    struct read_response {
        unsigned int button_states;
    };
    
    struct set_led {
        unsigned char led_num;
        unsigned char color[3]; // RGB values for the led.
    };
    
    struct set_leds {
        unsigned char colors[16][3]; // Array of 16 RGB values.
    };
    
    struct set_lcd {
        unsigned char data[2][8]; // Array of strings for top and bottom row.
    };
    
    inline void send_button_event(struct button_event *command)
    {
        send_command(
                BUTTON_EVENT_HEADER,
                (void *)command,
                sizeof(struct button_event)
        );
    }
    
    inline void send_read_response(struct read_response *command)
    {
        send_command(
                BUTTON_EVENT_HEADER,
                (void *)command,
                sizeof(struct read_response)
        );
    }
    
    inline void unpack_set_led(struct set_led *dest)
    {
        get_command_body((void *)dest, sizeof(struct set_led));
    }
    
    inline void unpack_set_leds(struct set_leds *dest)
    {
        get_command_body((void *)dest, sizeof(struct set_leds));
    }
    
    inline void unpack_set_lcd(struct set_lcd *dest)
    {
        get_command_body((void *)dest, sizeof(struct set_lcd));
    }
    
    void handle_set_led();

    void handle_set_leds();
    
    void handle_set_lcd();
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LIBUARTTRELLIS_H */

