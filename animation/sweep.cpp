
#include "sweep.h"
#include "animation_utils.h"
#include "../device_configuration.h"
#include "../global.h"
#include "../commands.h"
#include <avr/pgmspace.h>
#include <stdlib.h>

#if defined(SB_ENABLE_DEBUG_BASE) && defined(SB_ENABLE_ANIMATION_DEBUG)
#define DEBUG
#define DEBUG_UART
#endif

#include "../debug.h"

// The lookup table to make the brightness changes be more visible
uint8_t const sweep[] PROGMEM = {0, 1, 2, 3, 3, 2, 1, 0};
static uint8_t incr;

void start_sweep(animation_queue_data_t const *data)
{
	LS_("sweep start");
	incr = 0;
}

void show_sweep(animation_queue_data_t const *data)
{
	uint8_t w = u8g_GetWidth(&global.u8g);
	uint8_t h = u8g_GetHeight(&global.u8g);

    u8g_FirstPage(&global.u8g);
    do
    {
    	if (data->direction == MATRIX_ANIMATION_DIRECTION_RIGHT)
    	{
			if (incr == 0)
				incr = 8;
			incr--;
    	}
    	else
    	{
			incr++;
			if (incr >= 8)
				incr = 0;
    	}

    	for (uint8_t x = 0; x < w; x++)
    	{
    		for (uint8_t y = 0; y < h; y++)
    		{
    			u8g_SetColorIndex(&global.u8g, pgm_read_byte(&sweep[(x + y + incr) % 8]));
    			u8g_DrawPixel(&global.u8g, x, y);
    		}
    	}

    } while (u8g_NextPage(&global.u8g));
}

void stop_sweep(animation_queue_data_t const *data)
{
    LS_("sweep stop");
    clean_matrix_display();
}
