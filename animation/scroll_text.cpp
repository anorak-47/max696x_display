
#include "scroll_text.h"
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

static uint8_t xpos = 1;
static uint8_t ypos = 7;
static uint8_t loopc = 0;
static uint8_t text_width = 0;

void start_scroll_text(animation_queue_data_t const *data)
{
	u8g_SetFont(&global.u8g, select_font(data->font));
    u8g_SetColorIndex(&global.u8g, data->color);

    text_width = u8g_GetStrWidth(&global.u8g, (char *)data->data);
    loopc = text_width;
    xpos = 1;

    LV_("st:%s", (char *)data->data);
    LV_("tw:%u", text_width);
}

void show_scroll_text(animation_queue_data_t const *data)
{
    u8g_FirstPage(&global.u8g);
    do
    {
        u8g_DrawStr(&global.u8g, xpos, ypos, (char *)data->data);
        if (data->direction == MATRIX_ANIMATION_DIRECTION_RIGHT)
            u8g_DrawStr(&global.u8g, xpos - text_width - 1, ypos, (char *)data->data);
        else if (data->direction == MATRIX_ANIMATION_DIRECTION_LEFT)
            u8g_DrawStr(&global.u8g, xpos + text_width + 1, ypos, (char *)data->data);

    } while (u8g_NextPage(&global.u8g));

    if (data->direction == MATRIX_ANIMATION_DIRECTION_RIGHT)
    {
        xpos++;

        if (loopc)
        {
            loopc--;
        }
        else
        {
            loopc = text_width;
            xpos = 1;
        }
    }
    else
    {
        xpos--;

        if (loopc)
        {
            loopc--;
        }
        else
        {
            loopc = text_width;
            xpos = 1;
        }

        // LV_("xpos:%u", xpos);
    }
}

void stop_scroll_text(animation_queue_data_t const *data)
{
    LV_("sst: %u", xpos);
    clean_matrix_display();
}
