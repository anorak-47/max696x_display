
/************************************************************************/
/*                                                                      */
/*                      Debouncing 8 Keys                               */
/*                      Sampling 4 Times                                */
/*                      With Repeat Function                            */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

#include "device_configuration.h"
#include "key.h"
#include "timer.h"
#include "animation/animation.h"

#if defined(SB_ENABLE_DEBUG_BASE)
#define DEBUG
#define DEBUG_UART
#endif

#include "debug.h"

#define USE_TIMER2_FOR_KEYPRESS_DETECTION

#define KEY_DDR DDRD
#define KEY_PORT PORTD
#define KEY_PIN PIND

#define REPEAT_MASK ALL_KEYS // repeat
#define REPEAT_START 50      // after 500ms
#define REPEAT_NEXT 20       // every 200ms

volatile uint8_t key_state; // debounced and inverted key state: bit = 1: key pressed
volatile uint8_t key_press; // key press detect
volatile uint8_t key_rpt;   // key long press and repeat

uint8_t kbt = -1;

static uint8_t ct0, ct1, rpt;
uint8_t i;

#ifdef USE_TIMER2_FOR_KEYPRESS_DETECTION
ISR(TIMER2_OVF_vect) // every 10ms
{
    TCNT2 = (uint8_t)(int16_t) - (F_CPU / 1024 * 10e-3 + 0.5); // preload for 10ms

    i = key_state ^ ~KEY_PIN;   // key changed ?
    ct0 = ~(ct0 & i);           // reset or count ct0
    ct1 = ct0 ^ (ct1 & i);      // reset or count ct1
    i &= ct0 & ct1;             // count until roll over ?
    key_state ^= i;             // then toggle debounced state
    key_press |= key_state & i; // 0->1: key press detect

    if ((key_state & REPEAT_MASK) == 0) // check repeat function
        rpt = REPEAT_START;             // start delay
    if (--rpt == 0)
    {
        rpt = REPEAT_NEXT; // repeat delay
        key_rpt |= key_state & REPEAT_MASK;
    }
}
#else
void checkKeys()
{
    i = key_state ^ ~KEY_PIN;   // key changed ?
    ct0 = ~(ct0 & i);           // reset or count ct0
    ct1 = ct0 ^ (ct1 & i);      // reset or count ct1
    i &= ct0 & ct1;             // count until roll over ?
    key_state ^= i;             // then toggle debounced state
    key_press |= key_state & i; // 0->1: key press detect

    if ((key_state & REPEAT_MASK) == 0) // check repeat function
        rpt = REPEAT_START;             // start delay
    if (--rpt == 0)
    {
        rpt = REPEAT_NEXT; // repeat delay
        key_rpt |= key_state & REPEAT_MASK;
    }
}
#endif

///////////////////////////////////////////////////////////////////
//
// check if a key has been pressed. Each pressed key is reported
// only once
//
uint8_t get_key_press(uint8_t key_mask)
{
    cli();                 // read and clear atomic !
    key_mask &= key_press; // read key(s)
    key_press ^= key_mask; // clear key(s)
    sei();
    return key_mask;
}

///////////////////////////////////////////////////////////////////
//
// check if a key has been pressed long enough such that the
// key repeat functionality kicks in. After a small setup delay
// the key is reported being pressed in subsequent calls
// to this function. This simulates the user repeatedly
// pressing and releasing the key.
//
uint8_t get_key_rpt(uint8_t key_mask)
{
    cli();               // read and clear atomic !
    key_mask &= key_rpt; // read key(s)
    key_rpt ^= key_mask; // clear key(s)
    sei();
    return key_mask;
}

///////////////////////////////////////////////////////////////////
//
uint8_t get_key_short(uint8_t key_mask)
{
    cli(); // read key state and key press atomic !
    return get_key_press(~key_state & key_mask);
}

///////////////////////////////////////////////////////////////////
//
uint8_t get_key_long(uint8_t key_mask)
{
    return get_key_press(get_key_rpt(key_mask));
}

///////////////////////////////////////////////////////////////////
//
void initKeys(void)
{
    cli();

    // Configure debouncing routines
    KEY_DDR &= ~ALL_KEYS; // configure key port for input
    KEY_PORT |= ALL_KEYS; // and turn on pull up resistors

#ifdef USE_TIMER2_FOR_KEYPRESS_DETECTION
    // ATmega32, Timer 0
    // TCCR0 = (1 << CS02) | (1 << CS00); // divide by 1024
    // TCNT0 = (uint8_t)(int16_t) - (F_CPU / 1024 * 10e-3 + 0.5); // preload for 10ms
    // TIMSK |= 1 << TOIE0; // enable timer interrupt

    // ATmega328, Timer 2 konfigurieren
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);         // Prescaler 1024
    TCNT2 = (uint8_t)(int16_t) - (F_CPU / 1024 * 10e-3 + 0.5); // preload for 10ms
    TIMSK2 |= (1 << TOIE2);                                    // Compare Interrupt erlauben

    LV_("TCNT2=%u\n", (uint8_t)(int16_t) - (F_CPU / 1024 * 10e-3 + 0.5));
#endif

    sei();
}


void keypressMainLoop()
{
    if (get_key_short(KEY_S1))
    {
    	LS_("S1");
    	//animation_next();
    }

    if (get_key_long(KEY_S1))
    {
    	LS_("S1L");
    	//animation_toggle();
    }

    if (get_key_short(KEY_S2))
    {
    	LS_("S2");
    	//animation_previous();
    }

    if (get_key_long(KEY_S2))
    {
    	LS_("S2L");

    }

    /*
    // single press and repeat

    if (get_key_press(1 << KEY2) || get_key_rpt(1 << KEY2))
    {
        uint8_t i = LED_PORT;

        i = (i & 0x07) | ((i << 1) & 0xF0);
        if (i < 0xF0) i |= 0x08;
        LED_PORT = i;
    }
    */
}
