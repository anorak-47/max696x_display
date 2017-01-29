#pragma once

#include "../matrix.h"
#include "animation_queue.h"

void start_typematrix(animation_queue_data_t const *data);
void show_typematrix(animation_queue_data_t const *data);
void stop_typematrix(animation_queue_data_t const *data);

void update_typematrix(uint8_t row_number, matrix_row_t row);
