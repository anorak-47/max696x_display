/*
 * timer.c
 *
 *  Created on: 28.06.2011
 *      Author: wenkm
 */

#include "global.h"
#include "utils.h"
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>

//#define DEBUG
//#define DEBUG_UART
#include "debug.h"

#define MAXTIMERS 5

enum timer_state
{
    invalid = 0,
    running,
    stopped,
    paused
};

struct _Timer
{
    enum timer_state state;
    bool looping;
    uint16_t secs;
    uint16_t msecs;
    volatile uint16_t ticksleft;
    void (*f)(void *closure);
    void *closure;
};

typedef struct _Timer Timer;

Timer timerlist[MAXTIMERS];
unsigned long timercount = 0;

#ifndef SB_USE_KEYTIMER2
volatile uint8_t keycount = 10;
#endif

/*
 Der Compare Interrupt Handler
 wird aufgerufen, wenn
 TCNT0 = OCR0A = 125-1
 ist (125 Schritte), d.h. genau alle 1 ms
 */
#ifdef SB_TIMER_TIMER0
ISR(TIMER0_COMPA_vect)
{
    // tick timers

    for (uint8_t i = 0; i < MAXTIMERS; i++)
    {
        enum timer_state state = timerlist[i].state;
        uint16_t ticksleft = timerlist[i].ticksleft;
        if ((state == running) && ticksleft > 0)
        {
            ticksleft--;
            timerlist[i].ticksleft = ticksleft;
        }
    }
}
#endif

#ifdef SB_TIMER_TIMER1
ISR(TIMER1_COMPA_vect)
{
    // tick timers

    for (uint8_t i = 0; i < MAXTIMERS; i++)
    {
        enum timer_state state = timerlist[i].state;
        uint16_t ticksleft = timerlist[i].ticksleft;
        if ((state == running || state == running_looping) && ticksleft > 0)
        {
            ticksleft--;
            timerlist[i].ticksleft = ticksleft;
        }
    }
}
#endif

int8_t getNextFreeTimer()
{
    for (uint8_t i = 0; i < MAXTIMERS; i++)
    {
        if (timerlist[i].state == invalid)
            return i;
    }

    return -1;
}

int8_t addLongTimer(uint16_t secs, uint16_t msecs, void (*f)(void *closure), void *closure)
{
    if ((msecs == 0 && secs == 0) || f == 0)
        return -1;

    int8_t ntc = getNextFreeTimer();

    // LV_("addLongTimer: ntc=%d", ntc);

    if (ntc >= 0)
    {
        uint16_t ticks = msecs;

        if (msecs == 0 && secs > 0)
        {
            ticks = 1000;
        }

        // LV_("addLongTimer: %d %d", secs, msecs);

        timerlist[ntc].state = running;
        timerlist[ntc].looping = false;
        timerlist[ntc].secs = secs;
        timerlist[ntc].msecs = msecs;
        timerlist[ntc].f = f;
        timerlist[ntc].closure = closure;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            timerlist[ntc].ticksleft = ticks;
        }

        timercount++;
    }

    return ntc;
}

int8_t addTimer(uint16_t msecs, void (*f)(void *closure), void *closure)
{
    if (msecs == 0 || f == 0)
        return -1;

    int8_t ntc = getNextFreeTimer();

    LV_("addTimer: ntc=%d %d", ntc, msecs);

    if (ntc >= 0)
    {
        timerlist[ntc].state = running;
        timerlist[ntc].looping = false;
        timerlist[ntc].secs = 0;
        timerlist[ntc].msecs = msecs;
        timerlist[ntc].f = f;
        timerlist[ntc].closure = closure;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            timerlist[ntc].ticksleft = msecs;
        }

        timercount++;
    }

    return ntc;
}

int8_t addLoopTimer(uint16_t msecs, void (*f)(void *closure), void *closure)
{
    if (msecs == 0 || f == 0)
        return -1;

    int8_t ntc = getNextFreeTimer();

    // LV_("addTimer: ntc=%d", ntc);

    if (ntc >= 0)
    {
        timerlist[ntc].state = running;
        timerlist[ntc].looping = true;
        timerlist[ntc].secs = 0;
        timerlist[ntc].msecs = msecs;
        timerlist[ntc].f = f;
        timerlist[ntc].closure = closure;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            timerlist[ntc].ticksleft = msecs;
        }

        timercount++;
    }

    return ntc;
}

int8_t createTimer(uint16_t msecs, void (*f)(void *closure), void *closure)
{
    if (msecs == 0 || f == 0)
        return -1;

    int8_t ntc = getNextFreeTimer();

    LV_("createTimer: ntc=%d %d", ntc, msecs);

    if (ntc >= 0)
    {
        timerlist[ntc].state = stopped;
        timerlist[ntc].looping = false;
        timerlist[ntc].msecs = msecs;
        timerlist[ntc].secs = 0;
        timerlist[ntc].ticksleft = 0;
        timerlist[ntc].f = f;
        timerlist[ntc].closure = closure;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            timerlist[ntc].ticksleft = 0;
        }

        timercount++;
    }

    return ntc;
}

int8_t restartTimer(int8_t t)
{
    if (t < 0 || t >= MAXTIMERS || (timerlist[t].state != stopped && timerlist[t].state != paused && timerlist[t].state != running))
        return -1;

    LV_("restartTimer: %d", t);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        timerlist[t].ticksleft = timerlist[t].msecs;
    }

    timerlist[t].state = running;

    return t;
}

int8_t stopTimer(int8_t t)
{
    if (t < 0 || t >= MAXTIMERS || (timerlist[t].state != running && timerlist[t].state != paused))
        return -1;

    LV_("stopTimer: %d", t);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        timerlist[t].ticksleft = 0;
    }

    timerlist[t].state = stopped;

    return t;
}

int8_t pauseTimer(int8_t t)
{
    if (t < 0 || t >= MAXTIMERS || (timerlist[t].state != running))
        return -1;

    LV_("pauseTimer: %d", t);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        timerlist[t].state = paused;
    }

    return t;
}

int8_t continueTimer(int8_t t)
{
    if (t < 0 || t >= MAXTIMERS || (timerlist[t].state != paused))
        return -1;

    LV_("continueTimer: %d", t);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        timerlist[t].state = running;
    }

    return t;
}

void delTimer(int8_t t)
{
    if (t < 0 || t >= MAXTIMERS)
        return;

    LV_("delTimer: %d", t);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        timerlist[t].ticksleft = 0;
    }

    timerlist[t].state = invalid;
    timerlist[t].looping = false;
    timerlist[t].msecs = 0;
    timerlist[t].secs = 0;
    timerlist[t].f = 0;

    timercount--;
}

// call this in your mainloop
void checkTimer()
{
    Timer *t;
    uint16_t ticksleft;

    for (uint8_t i = 0; i < MAXTIMERS; i++)
    {
        t = &timerlist[i];

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            ticksleft = t->ticksleft;
        }

        if (ticksleft == 0 && (t->state == running))
        {
            if (t->secs > 0)
            {
                t->secs--;

                LV_("timer %d secs: %d", i, t->secs);

                if (t->secs > 0)
                {
                    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                    {
                        t->ticksleft = 1000;
                    }
                }
                else
                {
                    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                    {
                        t->ticksleft = t->msecs;
                    }
                }
            }
            else
            {
                if (t->state == running)
                {
                    if (t->looping)
                    {
                        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                        {
                            t->ticksleft = t->msecs;
                        }
                    }
                    else
                    {
                        t->state = stopped;
                        LV_("timer %d stopped", i);
                    }

                    t->f(t->closure);
                }
            }
        }
    }
}

void resetTimer(void)
{
    for (uint8_t i = 0; i < MAXTIMERS; i++)
    {
        timerlist[i].state = invalid;
        timerlist[i].f = 0;
        timerlist[i].secs = 0;
        timerlist[i].msecs = 0;
        timerlist[i].closure = 0;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            timerlist[i].ticksleft = 0;
        }
    }
}

uint16_t ticksLeftTimer(int8_t t)
{
    uint16_t ticksleft;

    if (t < 0 || t >= MAXTIMERS)
        return 0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ticksleft = timerlist[t].ticksleft;
    }

    return ticksleft;
}

uint16_t secondsLeftTimer(int8_t t)
{
    uint16_t seconds;

    if (t < 0 || t >= MAXTIMERS)
        return 0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        seconds = (timerlist[t].ticksleft / 1000) + timerlist[t].secs;
    }

    return seconds;
}

#ifdef SB_ENABLE_DUMP
void dumpTimer(void)
{
    printf_P(PSTR("timer: %d\r\n"), timercount);
    for (uint8_t i = 0; i < MAXTIMERS; i++)
    {
        printf_P(PSTR(" %d: %d %d.%06d, %06d\r\n"), i, timerlist[i].state, timerlist[i].secs, timerlist[i].msecs, timerlist[i].ticksleft);
    }
}
#endif

void initTimer(void)
{
    resetTimer();

    // Global Interrupts deaktivieren
    cli();

#ifdef SB_TIMER_TIMER1
    PRR &= ~_BV(PRTIM1);

    // Timer 0 konfigurieren
    TCCR1A = (1 << WGM01);               // CTC Modus, Top: OCR0A
    TCCR1B |= (1 << CS01) | (1 << CS00); // Prescaler 64

    // OCR0 = 125 - 1;  // ((8000000/64)/1000) = 125
    OCR1A = 250 - 1; // ((16000000/64)/1000) = 250

    // Compare Interrupt erlauben
    TIMSK1 |= (1 << OCIE1A);
#endif

#ifdef SB_TIMER_TIMER0
    PRR &= ~_BV(PRTIM0);

    // Timer 0 konfigurieren
    TCCR0A = (1 << WGM01);               // CTC Modus, Top: OCR0A
    TCCR0B |= (1 << CS01) | (1 << CS00); // Prescaler 64

    // OCR0 = 125 - 1;  // ((8000000/64)/1000) = 125
    OCR0A = 250 - 1; // ((16000000/64)/1000) = 250

    // Compare Interrupt erlauben
    TIMSK0 |= (1 << OCIE0A);
#endif

    // Global Interrupts aktivieren
    sei();
}
