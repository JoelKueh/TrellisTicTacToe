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
#ifndef LIBI2C_H
#define	LIBI2C_H

#include <xc.h>

#define MAX_I2C_LEN 100

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    /**
     * Begins i2c communication.
     */
    void i2c_init(void);
    
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
        uint8_t *dest, uint8_t size, int delay);
    
    /**
     * Executes a blocking send over I2C
     * @param i2c_addr The address of the thing to send
     * @param prefix The I2C prefix (likely address bytes)
     * @param prefix_len The length of the prefix
     * @param data The data to send
     * @param data_len The length of the data
     */
    void i2c_send(uint8_t i2c_addr, const uint8_t *prefix, uint8_t prefix_len,
        const uint8_t *data, uint8_t data_len);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

