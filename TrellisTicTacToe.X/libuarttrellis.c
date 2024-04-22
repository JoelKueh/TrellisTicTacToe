/*
 * Date: 4/22/2024
 * Main Author(s): Joel Kuehne
 * Course number: EE 2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Top level library used on the PIC side. Wraps up
 * UART command reception and processing into two different functions:
 * poll_and_update() and process_uart(). Turns the general purpose libtrellis,
 * libi2c, and libuart libraries into something specific for our application.
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
    
/**
 * A struct defining the layout of a button event command sent over UART.
 */
struct button_event {
    uint8_t button_num : 7;
    uint8_t is_rising : 1;
};

/**
 * A struct defining the layout of a set_led command received over UART.
 */
struct set_led {
    uint8_t led_num;
    uint8_t color[3]; // RGB values for the led.
};

/**
 * A struct defining the layout of a set_display command received over UART.
 */
struct set_display {
    uint8_t colors[16][3]; // Array of 16 RGB values.
};

/**
 * A struct defining the layout of a set_lcd command recieved over UART.
 */
struct set_lcd {
    uint8_t data[2][8]; // Array of strings for top and bottom row.
};

/**
 * Queues a button event to be sent over the UART transmit buffer.
 * @param command The struct representing the button event.
 */
void send_button_event(const struct button_event *command)
{
    send_command(
            BUTTON_EVENT_HEADER,
            (unsigned char *)command,
            sizeof(struct button_event)
    );
}

/**
 * Unpacks a set led command from the UART buffer.
 * @param dest The struct to unpack into.
 */
void unpack_set_led(struct set_led *dest)
{
    get_command_body((unsigned char *)dest, sizeof(struct set_led));
}

/**
 * Unpacks a set display command from the UART buffer.
 * @param dest The struct to unpack into.
 */
void unpack_set_display(struct set_display *dest)
{
    get_command_body((unsigned char *)dest, sizeof(struct set_display));
}

/**
 * Unpacks a set_lcd command from the UART buffer.
 * @param dest The struct to unpack into.
 */
void unpack_set_lcd(struct set_lcd *dest)
{
    get_command_body((unsigned char *)dest, sizeof(struct set_lcd));
}

/**
 * Handles a set led command, setting the proper pixel on the trellis to the
 * color that was specified. The colors are sent in GRB format.
 */
void handle_set_led()
{
    struct set_led command;
    unpack_set_led(&command);
    
    set_led(command.led_num, command.color[0],
        command.color[1], command.color[2]);
}

/**
 * Handles a set display command, forwarding the color array to the trellis.
 * The color array is a 16x3 array of bytes in GRB format.
 */
void handle_set_display()
{
    struct set_display command;
    unpack_set_display(&command);
    
    set_display(command.colors);
}

/**
 * Handles a set LCD command, forwarding the buffer to the display.
 * The set LCD command body consists of a 2x8 array of characters that should
 * replace the current contents of the display.
 */
void handle_set_lcd()
{
    struct set_lcd command;
    unpack_set_lcd(&command);
    
    lcd_set_cursor(0, 0);
    lcd_puts(command.data[0]);
    lcd_set_cursor(1, 0);
    lcd_puts(command.data[1]);
}

/**
 * Calls the function that corresponds to a header sent over the UART bridge.
 * @param header The header byte to handle.
 */
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

/**
 * Initializes i2c communication with the I2C2 peripheral as well as 38400
 * baud UART communication with peripheral U1 with TX on RB7 and RX on RB8.
 * Also sends i2c initializing commands to the LCD and trellis over i2c.
 * Initializes TMR3 as a frame timer for the trellis.
 */
void bluetrellis_init(void)
{
    i2c_init();
    lcd_init();
    trellis_init();
    uart_init();
}

/**
 * Polls for button presses and sends a draw/show command to the display.
 * This function runs at a period of 20ms or greater. If 20ms has not
 * passed since the last call, the function will perform a blocking delay
 * until it has. Sends all button events over UART immediately.
 */
void poll_and_update(void)
{
    union key_event events[30];
    union key_event *key = events;
    struct button_event button;
    int num_events;
    
    await_frame();
    display_show();
    num_events = get_button_events(events, 30);
    delay_us(500);
    while (num_events--) {
        button.is_rising = key->edge == EDGE_RISING;
        button.button_num = key->num;
        send_button_event(&button);
    }
}

/**
 * Handles commands sent over UART from the bluetooth device.
 */
void process_uart(void)
{
    while (!uart_empty()) {
        char header = get_command_header();
        parse_uart_header(header);
    }
}