#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/wdt.h>
#include <util/atomic.h>

#include "global.h"
#include "utils.h"
#include "led.h"
#include "uart.h"

#ifdef SB_ENABLE_DEBUG_BASE
#define DEBUG
#define DEBUG_UART
#endif

#include "debug.h"

ISR(WDT_vect)
{
	global.wdt_alarm_occurred = 1;
}

void enter_idle_sleep(void)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_enable();
	}

	sleep_cpu();
	sleep_disable();
}

void enter_power_down_sleep(void)
{
	WITH_WATCHDOG(wdt_disable());
	//PRR |= _BV(PRTIM1);

	while (1)
	{
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);

		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			sleep_enable();
		}

		sleep_cpu();
		sleep_disable();
	}

	WITH_WATCHDOG(wdt_enable(WDTO_4S));

	//PRR &= ~(_BV(PRTIM1));

#ifdef DEBUG
	_delay_ms(250);
#endif
}
