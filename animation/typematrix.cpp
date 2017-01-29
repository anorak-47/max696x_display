
#include "typematrix.h"
#include "../commands.h"
#include "../device_configuration.h"
#include "../global.h"
#include "animation_utils.h"
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#if defined(SB_ENABLE_DEBUG_BASE) && defined(SB_ENABLE_ANIMATION_DEBUG)
#define DEBUG
#define DEBUG_UART
#endif

#include "../debug.h"

static uint8_t *typematrix;

#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 6

void start_typematrix(animation_queue_data_t const *data)
{
    LS_("typematrix start");
    typematrix = (uint8_t *)malloc(MATRIX_WIDTH * MATRIX_HEIGHT * sizeof(uint8_t));
    memset(typematrix, 0, MATRIX_WIDTH * MATRIX_HEIGHT * sizeof(uint8_t));
}

void draw_typematrix()
{
    uint8_t wo = (u8g_GetWidth(&global.u8g) - MATRIX_WIDTH) / 2;
    uint8_t ho = (u8g_GetHeight(&global.u8g) - MATRIX_HEIGHT) / 2;

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

    } while (u8g_NextPage(&global.u8g));
}

void decrement_typematrix_color_indices()
{
	//LS_("decrement_typematrix");

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
	//LV_("update_typematrix r:%u", row_number);

    if (row_number >= MATRIX_HEIGHT)
        return;

    //LV_("update_typematrix r:%u", row_number);

    for (uint8_t x = 0; x < MATRIX_WIDTH; x++)
    {
        if (row & (1 << x))
        {
            typematrix[row_number * MATRIX_WIDTH + x] = 3;
        }
    }

    draw_typematrix();
}

void show_typematrix(animation_queue_data_t const *data)
{
    draw_typematrix();
    decrement_typematrix_color_indices();
}

void stop_typematrix(animation_queue_data_t const *data)
{
    LS_("typematrix stop");
    clean_matrix_display();
    free(typematrix);
}
