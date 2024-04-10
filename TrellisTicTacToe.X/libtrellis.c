/*
 * File:   libuarttrellis.c
 * Author: joel
 *
 * Created on April 4, 2024, 10:01 AM
 */

#include "xc.h"

#include "libtrellis.h"

void handle_read_request()
{
    // This command has no data.
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