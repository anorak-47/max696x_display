
#include "interrupt.h"
#include "global.h"
#include "animation/animation.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

#ifdef SB_ENABLE_DEBUG_IRQ
#define DEBUG
#define DEBUG_UART
#endif

#include "debug.h"

void init_interrupts()
{
    /*
     * INT0: PD2 MAX696x
     * INT1: PD3 External Shutdown
     *
     */

    cli();
    EIMSK &= ~((1 << INT1) | (1 << INT0)); // INT 0/1 sperren

    // Analogcomparator ausschalten

    ACSR = 0x80;

    // Interrupt INT1 (PD3) konfigurieren

    DDRD &= ~(1 << PD3);                  // Ports als Eingägne
    PORTD |= (1 << PD3);                  // Interne PullUps Setzen

    EICRA &= ~0x0F;                       // löschen

    if ((PIND & (1 << PD3)) == 0)
	{
		// PD3 is LOW
		//LS_("ii: PD3 is LOW");
		//u8g_SleepOff(&global.u8g);
		EICRA |= (0 << ISC11) | (1 << ISC10); // INT1 auf Wechsel der Flanke
	}
	else
	{
		// PD3 high HIGH
		//LS_("ii: PD3 is HIGH");
		//u8g_SleepOn(&global.u8g);
		EICRA |= (0 << ISC11) | (0 << ISC10); // INT1 auf Low Level
	}

    //EICRA |= (0 << ISC11) | (1 << ISC10); // INT1 auf Wechsel der Flanke
    //EICRA |= (0 << ISC11) | (0 << ISC10); // INT1 auf Low Level

    //EIMSK |= (1 << INT1);                 // Interrupts aktivieren
    sei();

    LS_("init_interrupts");
}

void release_interrupts()
{
    // Interrupts aktivieren

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        global.external_shutdown_event = 0;
        EIMSK |= (1 << INT1);
    }
}

void release_interrupt_external_shutdown()
{
    // Interrupt 1 aktivieren

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        global.external_shutdown_event = 0;
        EIMSK |= (1 << INT1);
    }
}

ISR(INT1_vect)
{
    EIMSK &= ~(1 << INT1); // INT 1 sperren
    global.external_shutdown_event = 1;
}

void handleInterrupts(void)
{
	if (global.external_shutdown_event)
	{
		if ((PIND & (1 << PD3)) == 0)
		{
			// PD3 is LOW
			LS_("PD3 is LOW -> sleep off");
			u8g_SleepOff(&global.u8g);
			continue_animation();
			EICRA |= (0 << ISC11) | (1 << ISC10); // INT1 auf Wechsel der Flanke
		}
		else
		{
			// PD3 high HIGH
			LS_("PD3 is HIGH -> sleep on");
			pause_animation();
			u8g_SleepOn(&global.u8g);
			EICRA |= (0 << ISC11) | (0 << ISC10); // INT1 auf Low Level
		}

		release_interrupt_external_shutdown();
	}
}
