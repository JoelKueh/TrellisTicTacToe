/*
 * Date: 4/22/2024
 * Main Author(s): Debra Johnson, Greta Shields, Alejandro Jimenez
 * Refactored By: Joel Kuehne
 * Course number: EE 2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Lower level library that makes it easier to
 * execute reads and writes over i2c.
 */

#include "xc.h"

#include "libi2c.h"
#include "utills.h"

/**
 * Begins i2c communication with peripheral I2C2 at 100000 baud.
 */
void i2c_init(void)
{
	I2C2CONbits.I2CEN = 0;
	I2C2BRG = 157;
	_MI2C2IF = 0;
	I2C2CONbits.I2CEN = 1;
}
    
/**
 * Executes a blocking read over I2C
 * @param i2c_addr The address of the thing to read.
 * @param prefix The I2C prefix (likely address bytes)
 * @param prefix_len The length of the prefix
 * @param dest The place to store the result of the read
 * @param size The size of the read buffer
 * @param delay The delay between the initialization command and the read.
 */
void i2c_read(uint8_t i2c_addr, const uint8_t *prefix, uint8_t prefix_len,
        uint8_t *dest, uint8_t size, int delay)
{
    // Start Condition
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN);
    IFS3bits.MI2C2IF = 0;

    // Setting Read Location
    I2C2TRN = i2c_addr;
    while (!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    
    // Sending prefix...
    while (prefix_len--) {
        I2C2TRN = *prefix++;
        while (!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF = 0;
    }
    
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN);

    // Delay for slave to prepare its data (this won't work on a multi-master system)
    delay_us(delay);
    
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN);

    // Restart to initiate read
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = i2c_addr | 0b1;
    while (!IFS3bits.MI2C2IF);

    // Reading data until there is only 1 byte left
    I2C2CONbits.ACKDT = 0; // Reply with master ACK
    while (size-- != 1) {
        I2C2CONbits.RCEN = 1;
        while (I2C2CONbits.RCEN);
        *dest++ = I2C2RCV;
        I2C2CONbits.ACKEN = 1; // Request new byte
        while (I2C2CONbits.ACKEN); // Wait for ACKEN SEND
    }
    
    // Receive last byte
    I2C2CONbits.RCEN = 1;
    while (I2C2CONbits.RCEN);
    *dest++ = I2C2RCV;
    I2C2CONbits.ACKDT = 1; // Now reply with master NACK
    I2C2CONbits.ACKEN = 1;
    while (I2C2CONbits.ACKEN); // Wait for ACKEN SEND

    // Stop condition
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN);
    IFS3bits.MI2C2IF = 0;
}

/**
 * Executes a blocking send over I2C
 * @param i2c_addr The address of the thing to send
 * @param prefix The I2C prefix (likely address bytes)
 * @param prefix_len The length of the prefix
 * @param data The data to send
 * @param data_len The length of the data
 */
void i2c_send(uint8_t i2c_addr, const uint8_t *prefix, uint8_t prefix_len,
        const uint8_t *data, uint8_t data_len)
{
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