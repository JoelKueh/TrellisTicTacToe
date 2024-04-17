/*
 * File:   liblcd.c
 * Author: joel
 *
 * Created on April 9, 2024, 10:02 PM
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

#define NEO_KHZ800 0x00
#define NEO_TRELLIS_NEOPIX_PIN 3
#define NUM_LEDS 16

// Cannot read more than 32 bytes.
void read_block(unsigned char i2c_addr, unsigned char addr_upper,
        unsigned char addr_lower, unsigned char *dest, unsigned char len) {
    // Start Condition
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN);
    IFS3bits.MI2C2IF = 0;

    // Setting Read Location
    I2C2TRN = i2c_addr;
    while (!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = addr_upper;
    while (!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = addr_lower;
    while (!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;

    // Stop Condition
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN);
    IFS3bits.MI2C2IF = 0;

    // Start Condition
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN);
    IFS3bits.MI2C2IF = 0;

    delay_us(30);

    // Restart to initiate read
    I2C2TRN = i2c_addr | 0b1;
    while (!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;

    // Reading Data
    while (len--) {
        I2C2CONbits.RCEN = 1;
        while (I2C2CONbits.RCEN);
        *dest++ = I2C2RCV;
        I2C2CONbits.ACKEN = 1;
    }

    // Stop condition
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN);
    IFS3bits.MI2C2IF = 0;
}
//
//void send_block(unsigned char i2c_addr, unsigned char addr_upper,
//        unsigned char addr_lower, unsigned char *data, unsigned char len) {
//    // Start Condition
//    IFS3bits.MI2C2IF = 0;
//    I2C2CONbits.SEN = 1;
//    while (I2C2CONbits.SEN);
//    IFS3bits.MI2C2IF = 0;
//
//    // Addressing...
//    I2C2TRN = i2c_addr;
//    while (!IFS3bits.MI2C2IF);
//    IFS3bits.MI2C2IF = 0;
//    I2C2TRN = addr_upper;
//    while (!IFS3bits.MI2C2IF);
//    IFS3bits.MI2C2IF = 0;
//    I2C2TRN = addr_lower;
//    while (!IFS3bits.MI2C2IF);
//    IFS3bits.MI2C2IF = 0;
//
//    // Sending data...
//    while (len--) {
//        I2C2TRN = *data++;
//        while (!IFS3bits.MI2C2IF);
//        IFS3bits.MI2C2IF = 0;
//    }
//
//    // Stop condition
//    I2C2CONbits.PEN = 1;
//    while (I2C2CONbits.PEN);
//    IFS3bits.MI2C2IF = 0;
//}

void send_block(unsigned char i2c_addr, unsigned char *prefix, int prefix_len,
        unsigned char *data, int data_len) {
    // Start Condition
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN);
    IFS3bits.MI2C2IF = 0;

    // Addressing...
    I2C2TRN = i2c_addr;
    while (!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    
    // Sending prefix...
    while (prefix_len--) {
        I2C2TRN = *prefix++;
        while (!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF = 0;
    }

    // Sending data...
    while (data_len--) {
        I2C2TRN = *data++;
        while (!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF = 0;
    }

    // Stop condition
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN);
    IFS3bits.MI2C2IF = 0;
}

void set_keypad_events(char num, char edges)
{
    
}

void get_button_events(unsigned char *buffer, int max_size)
{
    
}

void set_led(char num, unsigned char g, unsigned char r, unsigned char b)
{
    unsigned char prefix[4];
    unsigned char data[3];
    
    prefix[0] = SEESAW_NEOPIXEL_BASE;
    prefix[1] = SEESAW_NEOPIXEL_BUF;
	prefix[2] = 0;
    prefix[3] = num * 3;
    data[0] = g;
    data[1] = r;
    data[2] = b;
    send_block(TRELLIS_ADDR, prefix, 4, data, 3);
//	volatile struct i2c_command *cmd = i2c_queue_get_top();
//
//	while (i2c_queue_full());
//
//	// Fill in the data for the command
//	cmd->data[0] = TRELLIS_ADDR;
//	cmd->data[1] = SEESAW_NEOPIXEL_BASE;
//	cmd->data[2] = SEESAW_NEOPIXEL_BUF;
//	cmd->data[3] = 0;
//    cmd->data[4] = num * 3;
//    cmd->data[5] = g;
//    cmd->data[6] = r;
//    cmd->data[7] = b;
//    cmd->len = 8;
//
//	// Complete the push
//	i2c_queue_push();
}

void set_display(unsigned char colors[16][3])
{
    const unsigned char prefixes[2][4] = {
        { SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_BUF, 0, 0 },
        { SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_BUF, 0, 24 },
    };
    
    send_block(TRELLIS_ADDR, prefixes[0], 4, colors, 24);
    send_block(TRELLIS_ADDR, prefixes[1], 4, colors[8], 24);
    
    // The Adafruit Seesaw can only handle writing 30 bytes to any buffer in
    // a single command, so we split up this command into two parts.
    
//    volatile struct i2c_command *cmd = i2c_queue_get_top();
//	while (i2c_queue_full());
//
//	cmd->data[0] = TRELLIS_ADDR;
//	cmd->data[1] = SEESAW_NEOPIXEL_BASE;
//	cmd->data[2] = SEESAW_NEOPIXEL_BUF;
//	cmd->data[3] = 0;
//    cmd->data[4] = 0;
//    for (int i = 0; i < 8; ++i) {
//        cmd->data[3 * i + 5] = colors[i][0];
//        cmd->data[3 * i + 6] = colors[i][1];
//        cmd->data[3 * i + 7] = colors[i][2];
//    }
//    cmd->len = 29;
//
//	// Complete the first push
//	i2c_queue_push();
//    
//    cmd = i2c_queue_get_top();
//	while (i2c_queue_full());
//
//	cmd->data[0] = TRELLIS_ADDR;
//	cmd->data[1] = SEESAW_NEOPIXEL_BASE;
//	cmd->data[2] = SEESAW_NEOPIXEL_BUF;
//	cmd->data[3] = 0;
//    cmd->data[4] = 8 * 3;
//    for (int i = 0; i < 8; ++i) {
//        cmd->data[3 * i + 5] = colors[i + 8][0];
//        cmd->data[3 * i + 6] = colors[i + 8][1];
//        cmd->data[3 * i + 7] = colors[i + 8][2];
//    }
//    cmd->len = 29;
//
//	// Complete the second push
//	i2c_queue_push();
}

void display_show(void)
{
    const unsigned char prefix[2] = {
        SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_SHOW
    };
    
    send_block(TRELLIS_ADDR, prefix, 2, 0, 0);
    
//	volatile struct i2c_command *cmd = i2c_queue_get_top();
//
//	while (i2c_queue_full());
//
//	// Fill in the data for the command
//	cmd->data[0] = TRELLIS_ADDR;
//	cmd->data[1] = SEESAW_NEOPIXEL_BASE;
//	cmd->data[2] = SEESAW_NEOPIXEL_SHOW;
//    cmd->len = 3;
//
//	// Complete the push
//	i2c_queue_push();
}

void trellis_init(void)
{
    _MI2C2IE = 0;
    unsigned char prefix[5];
    unsigned char data[5];
    
    // Software Reset Command
    prefix[0] = SEESAW_STATUS_BASE;
    prefix[1] = SEESAW_STATUS_SWRST;
    data[0] = 0xFF;
    send_block(TRELLIS_ADDR, prefix, 2, data, 1);
    delay_ms(100); // Delay 100 ms for Trellis to come back from reset.

    // Pixel Speed Command
    prefix[0] = SEESAW_NEOPIXEL_BASE;
    prefix[1] = SEESAW_NEOPIXEL_SPEED;
    data[0] = 0x01;
    send_block(TRELLIS_ADDR, prefix, 2, data, 1);

    // Buffer Length Command
    prefix[0] = SEESAW_NEOPIXEL_BASE;
    prefix[1] = SEESAW_NEOPIXEL_BUF_LEN;
    data[1] = (NUM_LEDS * 3) & 0xFF;
    data[0] = (NUM_LEDS * 3) >> 8;
    send_block(TRELLIS_ADDR, prefix, 2, data, 2);

    // Pin Set Command
    prefix[0] = SEESAW_NEOPIXEL_BASE;
    prefix[1] = SEESAW_NEOPIXEL_PIN;
    data[0] = NEO_TRELLIS_NEOPIX_PIN;
    send_block(TRELLIS_ADDR, prefix, 2, data, 1);
    
    _MI2C2IF = 0;
//    _MI2C2IE = 1;
}

//void trellis_init(void)
//{
//    _MI2C2IE = 0;
//    unsigned char prefix[5];
//    unsigned char data[5];
//    
//    // Software Reset Command
//    buffer[0] = 0xFF;
//    send_block(TRELLIS_ADDR, SEESAW_STATUS_BASE, SEESAW_STATUS_SWRST, buffer, 1);
//    delay_ms(100); // Delay 100 ms for Trellis to come back from reset.
//
//    // Pixel Speed Command
//    buffer[0] = 0x01;
//    send_block(TRELLIS_ADDR, SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_SPEED, buffer, 1);
//
//    // Buffer Length Command
//    unsigned int buff_len = NUM_LEDS * 3;
//    buffer[1] = buff_len & 0xFF;
//    buffer[0] = buff_len >> 8;
//    send_block(TRELLIS_ADDR, SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_BUF_LEN, buffer, 2);
//
//    // Pin Set Command
//    buffer[0] = NEO_TRELLIS_NEOPIX_PIN;
//    send_block(TRELLIS_ADDR, SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_PIN, buffer, 1);
//    
//    _MI2C2IF = 0;
////    _MI2C2IE = 1;
//}