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
#define SET_LED_HEADER 'D'
#define SET_DISPLAY_HEADER 'E'
#define SET_LCD_HEADER 'F'
    
struct button_event {
    uint8_t button_num : 7;
    uint8_t is_rising : 1;
};

struct set_led {
    uint8_t led_num;
    uint8_t color[3]; // RGB values for the led.
};

struct set_leds {
    uint8_t colors[16][3]; // Array of 16 RGB values.
};

struct set_lcd {
    uint8_t data[2][8]; // Array of strings for top and bottom row.
};
    
void send_button_event(const struct button_event *command)
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
}

void poll_buttons(void)
{
    union key_event events[30];
    union key_event *key = events;
    struct button_event button;
    int num_events;
    
    await_frame();
    display_show();
    num_events = get_button_events(events, 30);
    delay_ms(30);
    while (num_events--) {
        button.is_rising = key->edge == EDGE_RISING;
        button.button_num = key->num;
        send_button_event(&button);
    }
}

void process_uart(void)
{
    while (!uart_empty()) {
        char header = get_command_header();
        parse_uart_header(header);
    }
}