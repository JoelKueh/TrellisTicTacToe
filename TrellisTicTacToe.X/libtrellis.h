/*
 * Date: 4/22/2024
 * Main Author(s): Greta Shields (Write), Alejandro Jimenez (Read)
 * Refactored By: Joel Kuehne
 * Course number: EE 2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Lower level library that wraps up a set of
 * commands that can be sent to the trellis.
 */

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
    
    /**
     * A struct defining the layout of a key_event recieved from a keypad read.
     */
    union key_event {
      struct {
        uint8_t edge : 2;
        uint8_t num : 6;
      };
      uint8_t raw;
    };
    
    /**
     * A struct that defines the layout of a key_state to be sent
     * in a set_keypad_events command.
     */
    union key_state {
      struct {
        uint8_t state : 1;
        uint8_t active : 4;
        uint8_t extra : 3;
      };
      uint8_t raw;
    };
    
    /**
     * Sends the required initialization commands over the trellis. Also,
     * initializes a 20ms frame timer on TMR3.
     * 
     * i2c_init() from libi2c must be called before this function is run.
     */
    void trellis_init(void);
    
    /**
     * Preforms a blocking delay to wait for the 20ms frame timer.
     * Resets the frame timer to zero when the function completes.
     */
    void await_frame(void);
    
    /**
     * Sets a keypad event to be tracked or ignored by the trellis.
     * @param num The number of the key (0-15)
     * @param edges The edge in question
     * @param active Whether or not it should be tracked.
     */
    void set_keypad_event(uint8_t num, uint8_t edge, uint8_t active);
    
    /**
     * Reads at most max_size button events into a buffer.
     * @param buffer The buffer to write into.
     * @param max_size The maximum number of key_events to read.
     * @return The actual number of key_events read.
     */
    int get_button_events(union key_event *buffer, uint8_t max_size);
    
    /**
     * Sends a set led command to the trellis.
     * @param num The led to set.
     * @param g The green value.
     * @param r The red value.
     * @param b The blue value.
     */
    void set_led(uint8_t num, uint8_t g, uint8_t r, uint8_t b);
    
    /**
     * Sends a set display command to the trellis.
     * @param colors An array of colors for each led in GRB format.
     */
    void set_display(uint8_t colors[16][3]);
    
    /**
     * Tells the trellis to update the neopixels with the new values that it has
     * in it's buffer. This function should be called when you want to see the
     * new data written by the set_led and set_display commands.
     */
    void display_show(void);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LIBTRELLIS_H */
