/*
 * File:   kuehn348_i2clib.c
 * Author: joel
 *
 * Created on March 20, 2024, 12:23 PM
 */

#include "xc.h"

#include "libi2c.h"

#define ARRAY_SIZE 10

/**
 * Describes a queue that holds a series of commands to be sent over the I2C
 * bus.
 */
struct command_queue {
	struct i2c_command data[ARRAY_SIZE];
	int w_idx;
	int r_idx;
	int cnt;
};
volatile struct command_queue i2c_queue;
volatile int i2c_is_waiting = 1;

int i2c_queue_full(void)
{
    return i2c_queue.cnt == ARRAY_SIZE;
}

/**
 * Returns the address of the top of the queue so that an item
 * can be pushed to it.
 */
volatile struct i2c_command *i2c_queue_get_top()
{
	return &i2c_queue.data[i2c_queue.w_idx];
}

/**
 * Returns the address of the bottom of the queue so that it can be read.
 */
volatile struct i2c_command *i2c_queue_peek(void)
{
	return &i2c_queue.data[i2c_queue.r_idx];
}

/**
 * Removes an item from the queue, incrementing the read index.
 */
void i2c_queue_rmv(void)
{
	i2c_queue.r_idx++;
	i2c_queue.r_idx = i2c_queue.r_idx == ARRAY_SIZE ? 0 : i2c_queue.r_idx;
	i2c_queue.cnt--;
}

/**
 * Increments the write index of the command queue.
 * Also tells to the ISR to begin sending again if it was waiting.
 */
void i2c_queue_push(void)
{
	// Complete the push
	i2c_queue.w_idx++;
	i2c_queue.w_idx = i2c_queue.w_idx == ARRAY_SIZE ? 0 : i2c_queue.w_idx;
	i2c_queue.cnt++;

	// Set the interrupt flag to enter the ISR if it has exited
	// without starting a new command.
	if (i2c_is_waiting) {
		_MI2C2IF = 1;
	}
}

/**
 * Initializes the queue.
 */
void i2c_queue_init(void)
{
	i2c_queue.w_idx = 0;
	i2c_queue.r_idx = 0;
	i2c_queue.cnt = 0;
	for (int i = 0; i < ARRAY_SIZE; ++i)
	{
		i2c_queue.data[i].len = 0;
	}
}

void i2c_init(void)
{
	I2C2CONbits.I2CEN = 0;
	I2C2BRG = 157;
	_MI2C2IF = 0;
	_MI2C2IE = 1;
	I2C2CONbits.I2CEN = 1;
    
    i2c_queue_init();
}

void __attribute__((__interrupt__, __auto_psv__)) _MI2C2Interrupt(void)
{
	static int cmd_idx = -1;
	volatile struct i2c_command *cmd;

	_MI2C2IF = 0;

	cmd = i2c_queue_peek();

	// If we exit this ISR without queuing another write (e.g. when cmd_queue
	// is empty) we need to set the interrupt flag upon the next enqueue.
	if (i2c_queue.cnt == 0) {
		i2c_is_waiting = 1;
		return;
	}
	// Otherwise we do not want to set the interrupt flag upon enqueue.
	i2c_is_waiting = 0;

	if (cmd_idx == -1) {
		I2C2CONbits.SEN = 1;
		cmd_idx++;
		return;
	}

	if (cmd_idx == cmd->len) {
		cmd_idx = -1;
		I2C2CONbits.PEN = 1;
		i2c_queue_rmv();
		return;
	}

	I2C2TRN = cmd->data[cmd_idx++];
}
