/*
 * File:   utils.c
 * Author: joel
 *
 * Created on April 9, 2024, 10:13 PM
 */

#include "xc.h"

#include "utills.h"

void delay_ms(int ms)
{
	while (ms--) {
		asm("repeat #15998");
		asm("nop");
	}
}

void delay_us(int us)
{
	while (us--) {
		asm("repeat #12");
		asm("nop");
	}
}
