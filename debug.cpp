/*
 * debug.c
 *
 *  Created on: 30.06.2011
 *      Author: wenkm
 */

#include <avr/interrupt.h>
#include "debug.h"
#include "uart.h"

void debug_init_uart(void)
{
	cli();
	/* open stdout/stderr */
	fdevopen(debug_uart_put, NULL);
	sei();
}

int __attribute__((noinline))
debug_uart_put(char d, FILE *stream)
{
	//if (d == '\n') debug_uart_put('\r', stream);

	if (d == 0x1b) d = '^'; /* replace escape sequences. */

	uart_putc(d);

	return 0;
}
