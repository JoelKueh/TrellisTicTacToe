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

	/*
	 * Describes a command (series of bytes)
	 * that should be sent over the bus.
	 */
	struct i2c_command {
		int len;
		unsigned char data[MAX_I2C_LEN];
	};
    
    /**
     * Begins i2c communication.
     */
    void i2c_init(void);
    
    /**
     * Checks if the i2c queue is full.
     * @return True if the queue is full.
     */
    int i2c_queue_full(void);

	/**
	 * Returns the address of the top of the queue so that an item
	 * can be pushed to it.
	 */
	volatile struct i2c_command *i2c_queue_get_top();

	/**
	 * Increments the write index of the command queue.
	 * Also tells to the ISR to begin sending again if it was waiting.
	 */
	void i2c_queue_push(void);
    
    /**
     * Initializes the command queue.
     */
    void i2c_queue_init(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

