/*
 * debug.h
 *
 *  Created on: 30.06.2011
 *      Author: wenkm
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>
#include <avr/pgmspace.h>

//#define DEBUG
//#define DEBUG_UART

//#define noinline __attribute__((noinline))

/* define macros, if debug is enabled */
#ifdef DEBUG_UART
#define initDebug() debug_init_uart()
#define debug_printf(s, args...) printf_P(PSTR(s), ## args)
#else  /* not DEBUG */
#define initDebug() do {} while(0)
#define debug_printf(s, args...) do {} while(0)
#endif /* not DEBUG */

#ifdef DEBUG
#define LV_(s, args...) debug_printf(s "\n", args)
#define LS_(s) debug_printf(s "\n")
#define L__(s) debug_printf(#s "\n")
#else
#define LV_(s, args...) do {} while(0)
#define LS_(s) do {} while(0)
#define L__(s) do {} while(0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

void debug_init_uart(void);
int __attribute__((noinline)) debug_uart_put (char d, FILE *stream);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H_ */
