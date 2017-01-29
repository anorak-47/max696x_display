#ifndef _LED_H_
#define _LED_H_

#include "utils.h"

#define LED_DDR DDRC
#define LED_PORT PORTC
#define LED_PIN PINC

#define LED0 0

#define LED_ACTIVITY LED0

#define LEDON(led) cbi(LED_PORT, led)
#define LEDOFF(led) sbi(LED_PORT, led)
#define TOGGLELED(led) toggle(LED_PORT, led)

void initLED(void);

int8_t blinkLED(uint8_t led, uint16_t ontime, uint16_t offtime);
int8_t unblinkLED(int8_t led);

void dumpBlinkingLEDs(void);

#endif
