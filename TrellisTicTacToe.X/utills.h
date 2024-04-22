/*
 * Date: 4/22/2024
 * Main Author(s): Joel Kuehne
 * Course number: EE 2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: List of macros shared between other libraries.
 */

#ifndef UTILS_H
#define	UTILS_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    /**
     * Delays for 'ms' milliseconds.
     * @param ms The time in milliseconds.
     */
    void delay_ms(int ms);
    
    /**
     * Delays for 'us' microseconds
     * @param us The time in microseconds.
     */
    void delay_us(int us);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* UTILS_H */

