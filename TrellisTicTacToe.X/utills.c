/*
 * Date: 4/22/2024
 * Main Author(s): Joel Kuehne
 * Course number: EE 2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: List of macros shared between other libraries.
 */

#include "xc.h"

#include "utills.h"

/**
 * Delays for 'ms' milliseconds.
 * @param ms The time in milliseconds.
 */
void delay_ms(int ms)
{
	while (ms--) {
		asm("repeat #15998");
		asm("nop");
	}
}

/**
 * Delays for 'us' microseconds
 * @param us The time in microseconds.
 */
void delay_us(int us)
{
	while (us--) {
		asm("repeat #12");
		asm("nop");
	}
}
