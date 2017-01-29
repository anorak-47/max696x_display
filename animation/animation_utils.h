#pragma once

#include "animation_queue.h"
#include "../u8glib/u8g.h"
#include <inttypes.h>

struct _animation_interface
{
    bool is_running;
    bool is_paused;

    uint16_t delay_in_ms;
    uint8_t duration_in_s;

    void (*animationStart)(animation_queue_data_t const *data);
    void (*animationStop)(animation_queue_data_t const *data);
    void (*animationLoop)(animation_queue_data_t const *data);

    int8_t loop_timer;
    int8_t duration_timer;

    animation_queue_data_t data;
};

typedef struct _animation_interface animation_interface;

extern animation_interface animation;

void clean_matrix_display(void);
const u8g_fntpgm_uint8_t *select_font(uint8_t font);
