/*********************************************************************
 *
 *  Show fuses:
 *  avrdude -pm328p -cstk500v2 -P/dev/ttySTK500 -v
 *
 *  Fuses: 258CK/14CK + 65ms
 *  avrdude -pm328p -cstk500v2 -P/dev/ttySTK500 -B20 -U lfuse:w:0xde:m -U hfuse:w:0xd7:m -U efuse:w:0xf9:m
 *
 *  Fuses: 16CK/14CK + 4.1ms
 *  avrdude -pm328p -cstk500v2 -P/dev/ttySTK500 -B20 -U lfuse:w:0xef:m
 *
 *  Program with EEPROM
 *  avrdude -pm328p -cstk500v2 -P/dev/ttySTK500 -B.1 -Uflash:w:max696x_cpu.hex:a -Ueeprom:w:max696x_cpu.eep:a
 *
 */

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <util/atomic.h>
#include <util/delay.h>

#include "cmdparser.h"
#include "communication_twi.h"
#include "global.h"
#include "led.h"
#include "key.h"
#include "max696x/max696x.h"
#include "power.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"
#include "command.h"
#include "animation/animation.h"
#include "max696x_device.h"
#include "interrupt.h"

#ifndef F_CPU
#warning------ F_CPU not defined! ------
#endif

#define BAUD 38400 // 9600 14400 19200 38400 57600 115200

#define UBRR_VAL ((F_CPU + BAUD * 8) / (BAUD * 16) - 1) // clever runden
#define BAUD_REAL (F_CPU / (16 * (UBRR_VAL + 1)))       // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL * 1000) / BAUD)          // Fehler in Promille, 1000 = kein Fehler.

#if ((BAUD_ERROR < 990) || (BAUD_ERROR > 1010))
#error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch!
#endif

#ifdef SB_ENABLE_DEBUG_BASE
#define DEBUG
#define DEBUG_UART
#endif

#include "debug.h"

/*
 *  EEMEM
 */

uint16_t ee_dummy EEMEM = 0xadda;
uint8_t ee_activity_led_blink_enabled EEMEM = 1;
uint8_t ee_matrix_brightness EEMEM = 16;
uint8_t ee_matrix_config EEMEM = 0;

/*
 * globals
 */

struct _global_t global;

/*
 * MCUSR stuff
 */

void get_mcusr(void) __attribute__((naked)) __attribute__((section(".init3")));

uint8_t mcusr_mirror __attribute__((section(".noinit")));

void get_mcusr(void)
{
    mcusr_mirror = MCUSR;
    MCUSR = 0;
    wdt_disable();
}

/*
 * initialization
 */

void restore_saved_state(void)
{
    memset(&global, 0, sizeof(struct _global_t));

    global.initializing = 1;
    global.activity_led_blink_enabled = eeprom_read_byte(&ee_activity_led_blink_enabled);
    global.state.brightness = eeprom_read_byte(&ee_matrix_brightness);
    global.state.animation = 0;
    global.state.animation_state = 0;
    global.state.animation_speed = 5;
    global.state.fault = 0;
    global.state.width = (NUMBER_OF_DRIVERS_PER_ROW) * 16;
    global.state.height = (NUMBER_OF_DRIVER_DEVICES / NUMBER_OF_DRIVERS_PER_ROW) * 8;
    global.state.major_version = VERSION_MAJOR;
    global.state.minor_version = VERSION_MINOR;
    global.state.minimal_animation_id = MATRIX_FIRST_ANIMATION;
    global.state.maximal_animation_id = MATRIX_LAST_ANIMATION;
    global.state.config.raw = eeprom_read_byte(&ee_matrix_config);
}

void blink_init_start()
{
    LEDON(LED_ACTIVITY);

    if (mcusr_mirror & (1 << WDRF))
    {
        _delay_ms(50);
    }
    else
    {
        _delay_ms(100);
    }

    LEDOFF(LED_ACTIVITY);
}


void initialize(void)
{
#if defined(DEBUG_UART) || defined(SB_WITH_CMDPARSER)
    uart_init(UART_BAUD_SELECT(BAUD, F_CPU));
    initDebug();
#else
    PRR |= _BV(PRUSART0);
#endif

    PRR |= _BV(PRTIM1);

    LS_("max696x_cpu v" VERSION " " COMPILED_TS);
    //LV_("MCUSR=%x", mcusr_mirror);

    initCommand();
    initAnimation();
    init_interrupts();
    initLED();
    blink_init_start();
    initTimer();
    initKeys();
    initializeMatrixDisplay();
    release_interrupts();
    initCommunicationTwi(SB_LOCAL_I2C_SLAVE_ADDRESS);

#ifdef SB_WITH_CMDPARSER
    parserInit();
#endif

    //blinkLED(LED_ACTIVITY, 1000, 1000);

    global.wants_to_sleep = 0;
}

//####################### Hauptprogramm

int main(void)
{
    WITH_WATCHDOG(wdt_enable(WDTO_4S));

    restore_saved_state();
    initialize();

    while (1)
    {
        WITH_WATCHDOG(wdt_reset());

#ifdef SB_WITH_CMDPARSER
        parserUARTHandleInput();
#endif

        checkTimer();
        twiMainLoop();
        keypressMainLoop();
        commandMainLoop();
        animationMainLoop();
        handleInterrupts();

        // LV_("sleep? %d %d", global.wants_to_sleep, global.rfm_wants_to_sleep);

        if (global.wants_to_sleep)
        {
        	pause_animation();
            enter_power_down_sleep();
            continue_animation();
        }
        else
        {
            enter_idle_sleep();
        }
    }

} // end.main
