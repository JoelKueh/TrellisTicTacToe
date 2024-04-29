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

#include "xc.h"

#include "libtrellis.h"
#include "utills.h"

#define TRELLIS_ADDR (0x2E << 1)
#define SEESAW_STATUS_BASE 0x00
#define SEESAW_STATUS_SWRST 0x7F
#define SEESAW_STATUS_HW_ID 0x01

#define SEESAW_NEOPIXEL_BASE 0x0E
#define SEESAW_NEOPIXEL_PIN 0x01
#define SEESAW_NEOPIXEL_SPEED 0x02
#define SEESAW_NEOPIXEL_BUF_LEN 0x03
#define SEESAW_NEOPIXEL_BUF 0x04
#define SEESAW_NEOPIXEL_SHOW 0x05

#define SEESAW_KEYPAD_BASE 0x10
#define SEESAW_KEYPAD_EVENT 0x01
#define SEESAW_KEYPAD_COUNT 0x04
#define SEESAW_KEYPAD_FIFO 0x10

#define NEO_KHZ800 0x00
#define NEO_TRELLIS_NEOPIX_PIN 3
#define NUM_LEDS 16

#define NEO_TRELLIS_KEY(x) (((x) / 4) * 8 + ((x) % 4))
#define NEO_TRELLIS_SEESAW_KEY(x) (((x) / 8) * 4 + ((x) % 8))

//#define NEO_TRELLIS_KEY(x) (((x) >> 2) << 3 + ((x) & 3))
//#define NEO_TRELLIS_SEESAW_KEY(x) (((x) >> 3) << 2 + ((x) & 0b1111))

/**
 * Preforms a blocking delay to wait for the 20ms frame timer.
 * Resets the frame timer to zero when the function completes.
 */
void await_frame(void)
{
    while (!_T3IF);
    TMR3 = 0;
    _T3IF = 0;
}

/**
 * Sets a keypad event to be tracked or ignored by the trellis.
 * @param num The number of the key (0-15)
 * @param edges The edge in question
 * @param active Whether or not it should be tracked.
 */
void set_keypad_event(uint8_t num, uint8_t edge, uint8_t active)
{
    const uint8_t prefix[2] = {
        SEESAW_KEYPAD_BASE, SEESAW_KEYPAD_EVENT
    };
    uint8_t data[2];
    union key_state ks;
    
    num = NEO_TRELLIS_KEY(num);
    data[0] = num;
    ks.raw = 0;
    ks.state = active;
    ks.active = 1 << edge;
    data[1] = ks.raw;
    i2c_send(TRELLIS_ADDR, prefix, 2, data, 2);
}

/**
  * Reads at most max_size button events into a buffer.
  * @param buffer The buffer to write into.
  * @param max_size The maximum number of key_events to read.
  * @return The actual number of key_events read.
  */
int get_button_events(union key_event *buffer, uint8_t max_size)
{
    const uint8_t prefixes[2][2] = {
        { SEESAW_KEYPAD_BASE, SEESAW_KEYPAD_COUNT },
        { SEESAW_KEYPAD_BASE, SEESAW_KEYPAD_FIFO }
    };
    uint8_t len;
    delay_us(500);
    i2c_read(TRELLIS_ADDR, prefixes[0], 2, &len, 1, 500);
    delay_us(500);
    len = len > max_size ? max_size : len;
    if (len != 0) {
        i2c_read(TRELLIS_ADDR, prefixes[1], 2, (uint8_t *)buffer, len, 1000);
    }
    
    for (int i = 0; i < len; ++i) {
        buffer[i].num = NEO_TRELLIS_SEESAW_KEY(buffer[i].num);
    }
    
    return len;
}

/**
 * Sends a set led command to the trellis.
 * @param num The led to set.
 * @param g The green value.
 * @param r The red value.
 * @param b The blue value.
 */
void set_led(uint8_t num, uint8_t g, uint8_t r, uint8_t b)
{
    uint8_t prefix[4];
    uint8_t data[3];
    
    prefix[0] = SEESAW_NEOPIXEL_BASE;
    prefix[1] = SEESAW_NEOPIXEL_BUF;
    prefix[2] = 0;
    prefix[3] = num * 3;
    data[0] = g;
    data[1] = r;
    data[2] = b;
    i2c_send(TRELLIS_ADDR, prefix, 4, data, 3);
}

/**
 * Sends a set display command to the trellis.
 * @param colors An array of colors for each led in GRB format.
 */
void set_display(uint8_t colors[16][3])
{
    const uint8_t prefixes[2][4] = {
        { SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_BUF, 0, 0 },
        { SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_BUF, 0, 24 },
    };
    
    // The Adafruit Seesaw can only handle writing 30 bytes to any buffer in
    // a single command, so we split up this command into two parts.
    i2c_send(TRELLIS_ADDR, prefixes[0], 4, &(colors[0][0]), 24);
    i2c_send(TRELLIS_ADDR, prefixes[1], 4, &(colors[8][0]), 24);
}

/**
 * Tells the trellis to update the neopixels with the new values that it has
 * in it's buffer. This function should be called when you want to see the
 * new data written by the set_led and set_display commands.
 */
void display_show(void)
{
    const uint8_t prefix[2] = {
        SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_SHOW
    };
    
    delay_us(500);
    i2c_send(TRELLIS_ADDR, prefix, 2, 0, 0);
}

/**
 * Sends the required initialization commands over the trellis. Also,
 * initializes a 20ms frame timer on TMR3. By default, tracks every rising
 * and falling edge.
 * 
 * i2c_init() from libi2c must be called before this function is run.
 */
void trellis_init(void)
{
    uint8_t prefix[5];
    uint8_t data[5];
    
    // Set up frame timer
    PR3 = 39999;
    T3CONbits.TCKPS = 0b01;
    TMR3 = 0;
    _T3IF = 1;
    T3CONbits.TON = 1;
    
    // Software Reset Command
    prefix[0] = SEESAW_STATUS_BASE;
    prefix[1] = SEESAW_STATUS_SWRST;
    data[0] = 0xFF;
    i2c_send(TRELLIS_ADDR, prefix, 2, data, 1);
    delay_ms(100); // Delay 100 ms for Trellis to come back from reset.

    // Pixel Speed Command
    prefix[0] = SEESAW_NEOPIXEL_BASE;
    prefix[1] = SEESAW_NEOPIXEL_SPEED;
    data[0] = 0x01;
    i2c_send(TRELLIS_ADDR, prefix, 2, data, 1);

    // Buffer Length Command
    prefix[0] = SEESAW_NEOPIXEL_BASE;
    prefix[1] = SEESAW_NEOPIXEL_BUF_LEN;
    data[1] = (NUM_LEDS * 3) & 0xFF;
    data[0] = (NUM_LEDS * 3) >> 8;
    i2c_send(TRELLIS_ADDR, prefix, 2, data, 2);

    // Pin Set Command
    prefix[0] = SEESAW_NEOPIXEL_BASE;
    prefix[1] = SEESAW_NEOPIXEL_PIN;
    data[0] = NEO_TRELLIS_NEOPIX_PIN;
    i2c_send(TRELLIS_ADDR, prefix, 2, data, 1);
    
    // Initialize keypad tracking for every button
    prefix[0] = SEESAW_KEYPAD_BASE;
    prefix[1] = SEESAW_KEYPAD_BASE;
    for (unsigned int i = 0; i < 16; ++i) {
        set_keypad_event(i, EDGE_FALLING, 1);
        set_keypad_event(i, EDGE_RISING, 1);
    }
    
    _MI2C2IF = 0;
}
