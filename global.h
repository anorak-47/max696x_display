/*
 * gobal.h
 *
 *  Created on: 29.09.2012
 *      Author: wenk
 */

#ifndef GOBAL_H_
#define GOBAL_H_

#include "device_configuration.h"
#include "u8glib/u8g.h"
#include "version.h"
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#ifdef SB_WITH_WATCHDOG
#define WITH_WATCHDOG(command) command
#else
#define WITH_WATCHDOG(command)
#endif

struct _state_t
{
    uint8_t brightness;
    uint8_t animation;
    uint8_t animation_state;
    uint8_t animation_speed;
    uint8_t fault;
    uint8_t width;
    uint8_t height;
    uint8_t major_version;
    uint8_t minor_version;
};

struct _global_t
{
    uint8_t initializing;
    uint8_t activity_led_blink_enabled;
    uint8_t wants_to_sleep;
    uint8_t wdt_alarm_occurred;

    u8g_t u8g;

    struct _state_t state;
};

extern struct _global_t global;

#endif /* GOBAL_H_ */
