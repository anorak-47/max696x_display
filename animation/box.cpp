
#include "box.h"
#include "../commands.h"
#include "../device_configuration.h"
#include "../global.h"
#include "animation_utils.h"
#include <avr/pgmspace.h>
#include <stdlib.h>

#if defined(SB_ENABLE_DEBUG_BASE) && defined(SB_ENABLE_ANIMATION_DEBUG)
#define DEBUG
#define DEBUG_UART
#endif

#include "../debug.h"

static uint8_t xpos = 0;
static uint8_t ypos = 0;
static uint8_t width = 0;
static uint8_t loopc = 0;

void reset_box()
{
    xpos = u8g_GetWidth(&global.u8g) / 2;
    ypos = u8g_GetHeight(&global.u8g) / 2;

    loopc = u8g_GetWidth(&global.u8g) / 2;

    width = 0;
}

void start_box(animation_queue_data_t const *data)
{
    reset_box();
}

void show_box(animation_queue_data_t const *data)
{
    u8g_FirstPage(&global.u8g);
    do
    {
    	u8g_SetColorIndex(&global.u8g, data->color);
    	u8g_DrawFrame(&global.u8g, xpos, ypos, width, width);
    } while (u8g_NextPage(&global.u8g));

    if (data->direction == MATRIX_ANIMATION_DIRECTION_RIGHT)
    {
        xpos--;

        if (loopc)
        {
            loopc--;
        }
        else
        {
			reset_box();
        }
    }
    else
    {
        xpos--;
        ypos--;
        width += 2;

        if (loopc)
        {
            loopc--;
        }
        else
        {
        	reset_box();
        }

        // LV_("xpos:%u", xpos);
    }
}

void stop_box(animation_queue_data_t const *data)
{
    clean_matrix_display();
}
