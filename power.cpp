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

#define MAX_WDRF_COUNT 3

uint8_t ee_wdrf_count EEMEM = 0;
extern uint8_t mcusr_mirror;

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
	wdt_disable();
	PRR |= _BV(PRTIM1);

	while (1)
	{
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);

		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			WDTCSR |= (1<<WDIE); // WDT Interrupt enable
			sleep_enable();
		}

		sleep_cpu();
		sleep_disable();
	}

	WITH_WATCHDOG(wdt_enable(WDTO_4S));

	PRR &= ~(_BV(PRTIM1));

#ifdef DEBUG
	_delay_ms(250);
#endif
}

void enter_power_down_sleep_forever(void)
{
/*
	uint8_t counter = 0;
    uint8_t wdt_timeout = WDTO_4S;

    global.rfm_alarm_occurred = 0;
    global.wdt_alarm_occurred = 0;

    LS_("off");

#ifdef DEBUG
    while (!uart_tx_buffer_empty());
    _delay_ms(250);
#endif

    PRR |= _BV(PRTWI) | _BV(PRTIM0) | _BV(PRTIM1) | _BV(PRTIM2);
    rfm12_set_wakeup_timer(0x1FFF);

    wdt_enable(wdt_timeout);

	while (1)
	{
		global.wdt_alarm_occurred = 0;
		global.rfm_alarm_occurred = 0;

		set_sleep_mode(SLEEP_MODE_PWR_DOWN);

		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			WDTCSR |= (1<<WDIE); // WDT Interrupt enable
			sleep_enable();
		}

		sleep_cpu();
		sleep_disable();

		wdt_reset();

		counter++;

		if (counter % 3 == 0)
			wdt_timeout = WDTO_4S;
		else
			wdt_timeout = WDTO_250MS;

		wdt_enable(wdt_timeout);

		LEDON(LED_ACTIVITY);
		_delay_ms(3);
		LEDOFF(LED_ACTIVITY);
	}
*/
}

void enter_power_down_on_repearted_watchdog_reset(void)
{
	WITH_WATCHDOG(wdt_reset());
	uint8_t wdrf_count = eeprom_read_byte(&ee_wdrf_count);
	LV_("wdrf %u", wdrf_count);

	if (mcusr_mirror & (1<<WDRF))
	{
		LS_("wdrf");

		if (wdrf_count >= MAX_WDRF_COUNT)
		{
			enter_power_down_sleep_forever();
		}
		else
		{
			wdrf_count++;
			eeprom_write_byte(&ee_wdrf_count, wdrf_count);
		}
	}
	else
	{
		if (wdrf_count > 0)
		{
			eeprom_write_byte(&ee_wdrf_count, 0);
		}
	}
}
