/*
 * File:   libuarttrellis.c
 * Author: joel
 *
 * Created on April 4, 2024, 10:01 AM
 */

#include "xc.h"

#include "libuarttrellis.h"
#include "libuart.h"
#include "libtrellis.h"
#include "liblcd.h"
#include "utills.h"

#define BUTTON_EVENT_HEADER 'A'
#define SHOW_HEADER 'C'
#define SET_LED_HEADER 'D'
#define SET_DISPLAY_HEADER 'E'
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

void handle_show()
{
    display_show();
}

void handle_set_led()
{
    struct set_led command;
    unpack_set_led(&command);
    
    set_led(command.led_num, command.color[0],
        command.color[1], command.color[2]);
}

void handle_set_display()
{
    struct set_leds command;
    unpack_set_leds(&command);
    
    set_display(command.colors);
}

void handle_set_lcd()
{
    struct set_lcd command;
    unpack_set_lcd(&command);
    
    lcd_set_cursor(0, 0);
    lcd_puts(command.data[0]);
    lcd_set_cursor(1, 0);
    lcd_puts(command.data[1]);
}

void parse_uart_header(char header)
{
    switch (header) {
        case SET_LED_HEADER:
            handle_set_led();
            break;
        case SET_DISPLAY_HEADER:
            handle_set_display();
            break;
        case SET_LCD_HEADER:
            handle_set_lcd();
            break;
        case SHOW_HEADER:
            handle_show();
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

    // DEBUG FUNCTIONS
    unsigned char colors[16][3] = {
        { 0x00, 0x70, 0x00},
        { 0x10, 0x60, 0x00},
        { 0x20, 0x50, 0x00},
        { 0x30, 0x40, 0x00},
        { 0x40, 0x30, 0x00},
        { 0x50, 0x20, 0x00},
        { 0x60, 0x10, 0x00},
        { 0x70, 0x00, 0x00},
        { 0x70, 0x00, 0x00},
        { 0x60, 0x10, 0x00},
        { 0x50, 0x20, 0x00},
        { 0x40, 0x30, 0x00},
        { 0x30, 0x40, 0x00},
        { 0x20, 0x50, 0x00},
        { 0x10, 0x60, 0x00},
        { 0x00, 0x70, 0x00}
    };
    
    set_display(colors);
//    set_led(0, 0x00, 0x80, 0x00);
//    set_led(1, 0x20, 0x80, 0x40);
//    set_led(2, 0x80, 0x40, 0x40);
//    set_led(3, 0x00, 0x80, 0x40);
    delay_ms(500);
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