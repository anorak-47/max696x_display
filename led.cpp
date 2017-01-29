
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>
#include "device_configuration.h"
#include "led.h"
#include "timer.h"

//#define DEBUG
//#define ENABLE_DUMP
#include "debug.h"

#define MAXBLINKINGLEDS 1

#if (MAXBLINKINGLEDS > 0)
struct _blinkingled
{
    uint8_t led;
    uint8_t state;
    uint8_t oldstate;
    int8_t timer;
    uint16_t ontime;
    uint16_t offtime;
};

typedef struct _blinkingled blinkingled;

blinkingled blinkenleds[MAXBLINKINGLEDS];
#endif

void initLED(void)
{
#if (MAXBLINKINGLEDS > 0)
    memset(blinkenleds, 0, sizeof(blinkingled) * MAXBLINKINGLEDS);

    for (uint8_t blcnt = 0; blcnt < MAXBLINKINGLEDS; blcnt++)
    {
        blinkenleds[blcnt].timer = -1;
    }
#endif

    //cli();

    LED_DDR  |= ((1 << LED0)); // ports als ausgänge definieren
    LED_PORT |= ((1 << LED0)); // active low -> alle ausgänge setzen -> leds aus

    //sei();
}

#if (MAXBLINKINGLEDS > 0)
void cbblinkLED(void *closure)
{
    blinkingled *bled = (blinkingled*) closure;

    delTimer(bled->timer);

    if (bled->state)
    {
        LS_("O");
        bled->state = 0;
        bled->timer = addTimer(bled->offtime, cbblinkLED, closure);
        LEDOFF(bled->led);
    }
    else
    {
        LS_("o");
        bled->state = 1;
        bled->timer = addTimer(bled->ontime, cbblinkLED, closure);
        LEDON(bled->led);
    }
}

int8_t blinkLED(uint8_t led, uint16_t ontime, uint16_t offtime)
{
    int8_t blcnt;

    for (blcnt = 0; blcnt < MAXBLINKINGLEDS; blcnt++)
    {
        if (blinkenleds[blcnt].led == led)
        {
            unblinkLED(blcnt);
            break;
        }
    }

    if (blcnt >= MAXBLINKINGLEDS)
    {
        for (blcnt = 0; blcnt < MAXBLINKINGLEDS; blcnt++)
        {
            if (blinkenleds[blcnt].ontime == 0 && blinkenleds[blcnt].offtime == 0) break;
        }
    }

    if (blcnt < MAXBLINKINGLEDS)
    {
        blinkenleds[blcnt].led = led;
        blinkenleds[blcnt].state = 1;
        blinkenleds[blcnt].ontime = ontime;
        blinkenleds[blcnt].offtime = offtime;
        blinkenleds[blcnt].oldstate = (bic(LED_PORT, led));

        LEDON(led);
        blinkenleds[blcnt].timer = addTimer(ontime, cbblinkLED, (void*)&blinkenleds[blcnt]);
    }

    return blcnt;
}

int8_t unblinkLED(int8_t handle)
{
    if (handle >= 0 && handle < MAXBLINKINGLEDS)
    {
        delTimer(blinkenleds[handle].timer);

        blinkenleds[handle].ontime = 0;
        blinkenleds[handle].offtime = 0;

        //setLED(blinkenleds[handle].led, blinkenleds[handle].oldstate);
        LEDOFF(blinkenleds[handle].led);
    }

    return -1;
}

#ifdef SB_ENABLE_DUMP
void dumpBlinkingLEDs(void)
{
    printf_P(PSTR("blinking leds\r\n"));
    for (int blcnt = 0; blcnt < MAXBLINKINGLEDS; blcnt++)
    {
        printf_P(PSTR("%d: led: %d, on: %d, off %d, old: %d\r\n"),
                blcnt, blinkenleds[blcnt].led, blinkenleds[blcnt].ontime, blinkenleds[blcnt].offtime, blinkenleds[blcnt].oldstate);
    }
}
#endif
#endif

