
#include "typematrix.h"
#include "../commands.h"
#include "../device_configuration.h"
#include "../global.h"
#include "animation.h"
#include "animation_utils.h"
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#if defined(SB_ENABLE_DEBUG_BASE) && defined(SB_ENABLE_ANIMATION_DEBUG)
#define DEBUG
#define DEBUG_UART
#endif

#include "../debug.h"

#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 6

static uint16_t keys_pressed = 0;
static char buffer[5];
static uint8_t wo = 0;
static uint8_t ho = 0;
static uint8_t tx = 0;
static uint8_t color_index = 0;
static uint8_t typematrix[MATRIX_WIDTH * MATRIX_HEIGHT * sizeof(uint8_t)];

void draw_typematrix(void)
{
    u8g_FirstPage(&global.u8g);
    do
    {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++)
        {
            for (uint8_t y = 0; y < MATRIX_HEIGHT; y++)
            {
                uint8_t color = typematrix[y * MATRIX_WIDTH + x];
                u8g_SetColorIndex(&global.u8g, color);
                u8g_DrawPixel(&global.u8g, wo + x, ho + y);
            }
        }

        u8g_SetColorIndex(&global.u8g, color_index);

        sprintf_P(buffer, PSTR("%u"), keys_pressed);
        tx = u8g_GetWidth(&global.u8g) - u8g_GetStrWidth(&global.u8g, buffer);
        u8g_DrawStr(&global.u8g, tx, 7, buffer);

    } while (u8g_NextPage(&global.u8g));
}

void decrement_typematrix_color_indices(void)
{
    // LS_("decrement_typematrix");

    for (uint8_t x = 0; x < MATRIX_WIDTH; x++)
    {
        for (uint8_t y = 0; y < MATRIX_HEIGHT; y++)
        {
            uint8_t color = typematrix[y * MATRIX_WIDTH + x];

            if (color > 0)
            {
                color--;
                typematrix[y * MATRIX_WIDTH + x] = color;
            }
        }
    }
}

void update_typematrix(uint8_t row_number, matrix_row_t row)
{
    // LV_("update_typematrix r:%u", row_number);
    bool any_key_pressed = false;

    if (row_number >= MATRIX_HEIGHT)
        return;

    // LV_("update_typematrix r:%u", row_number);

    for (uint8_t x = 0; x < MATRIX_WIDTH; x++)
    {
        if (row & (1 << x))
        {
            typematrix[row_number * MATRIX_WIDTH + x] = 3;
            any_key_pressed = true;
        }
    }

    if (any_key_pressed)
        keys_pressed++;

    if (any_key_pressed && animation_is_running() && !animation_is_paused() && get_current_animation() == MATRIX_CMD_ANIMATE_TYPEMATRIX)
        draw_typematrix();
}

void show_typematrix(animation_queue_data_t const *data)
{
    draw_typematrix();
    decrement_typematrix_color_indices();
}

void start_typematrix(animation_queue_data_t const *data)
{
    LS_("typematrix start");
    // typematrix = (uint8_t *)malloc(MATRIX_WIDTH * MATRIX_HEIGHT * sizeof(uint8_t));
    // memset(typematrix, 0, MATRIX_WIDTH * MATRIX_HEIGHT * sizeof(uint8_t));

    color_index = data->color;
    u8g_SetFont(&global.u8g, select_font(data->font));

    wo = (u8g_GetWidth(&global.u8g) - MATRIX_WIDTH) / 2;
    ho = (u8g_GetHeight(&global.u8g) - MATRIX_HEIGHT) / 2;

    draw_typematrix();
}

void stop_typematrix(animation_queue_data_t const *data)
{
    LS_("typematrix stop");
    clean_matrix_display();
    // free(typematrix);
}
