#pragma once

#include "../commands.h"
#include "animation_queue.h"
#include <inttypes.h>
#include <stdbool.h>

void animation_toggle(void);
void start_animation(void);
void stop_animation(void);
void pause_animation(void);
void continue_animation(void);
bool set_animation(uint8_t animation_number);
void animation_next(void);
void animation_previous(void);
void animation_increase_speed(void);
void animation_decrease_speed(void);
void animation_set_speed(uint16_t speed);
bool animation_is_running(void);
bool animation_is_paused(void);
uint8_t get_current_animation(void);

void animation_set_typematrix_key(uint8_t row_number, matrix_row_t row);
void animation_set_lock_state(uint8_t locks);
void animation_show_info_text(animation_queue_data_t const *data);

void initAnimation(void);
void animationMainLoop(void);
