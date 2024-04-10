/*
 * File:   libuarttrellis.c
 * Author: joel
 *
 * Created on April 4, 2024, 10:01 AM
 */

#include "xc.h"

#include "libbluetrellis.h"
#include "libuart.h"
#include "libtrellis.h"
#include "liblcd.h"

#define BUTTON_EVENT_HEADER 'A'

#define SET_LED_HEADER 'D'
#define SET_LEDS_HEADER 'E'
#define SET_LCD_HEADER 'F'
    
struct button_event {
    unsigned char button_num : 7;
    unsigned char is_rising : 1;
};
struct button_event b_event;

struct set_led {
    unsigned char led_num;
    unsigned char color[3]; // RGB values for the led.
};
struct set_led set_led_com;

struct set_leds {
    unsigned char colors[16][3]; // Array of 16 RGB values.
};
struct set_leds set_leds_com;

struct set_lcd {
    unsigned char data[2][8]; // Array of strings for top and bottom row.
};
struct set_lcd set_lcd_com;
    
void send_button_event(struct button_event *command)
{
    send_command(
            BUTTON_EVENT_HEADER,
            (unsigned char *)command,
            sizeof(struct button_event)
    );
}

void unpack_set_led(struct set_led *dest)
{
    get_command_body((unsigned char *)dest, sizeof(struct set_led));
}

void unpack_set_leds(struct set_leds *dest)
{
    get_command_body((unsigned char *)dest, sizeof(struct set_leds));
}

void unpack_set_lcd(struct set_lcd *dest)
{
    get_command_body((unsigned char *)dest, sizeof(struct set_lcd));
}

void handle_set_led()
{
    struct set_led command;
    unpack_set_led(&command);
    
    // Code to handle the contents of the command should go here.
}

void handle_set_leds()
{
    struct set_leds command;
    unpack_set_leds(&command);
    
    // Code to handle the contents of the command should go here.
}

void handle_set_lcd()
{
    struct set_lcd command;
    unpack_set_lcd(&command);
    
    // Code to handle the contents of the command should go here.
}

void parse_uart_header(char header)
{
    switch (header) {
        case SET_LED_HEADER:
            handle_set_led();
            break;
        case SET_LEDS_HEADER:
            handle_set_leds();
            break;
        case SET_LCD_HEADER:
            handle_set_lcd();
            break;
        default:
            break;
    }
}

void bluetrellis_init(void)
{
    i2c_init();
    lcd_init();
    trellis_init();
    uart_init();
    
    // DEBUG FUNCTIONS:
    set_led(0, 0x70, 0x70, 0x70);
    display_show();
}

void poll_buttons(void)
{
    
}

void process_uart(void)
{
    while (!uart_empty()) {
        char header = get_command_header();
        parse_uart_header(header);
    }
}