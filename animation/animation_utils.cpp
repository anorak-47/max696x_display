
#include "animation_utils.h"
#include "../commands.h"
#include "../global.h"
#include <avr/pgmspace.h>

animation_interface animation;

void clean_matrix_display()
{
    u8g_FirstPage(&global.u8g);
    do
    {
        u8g_SetColorIndex(&global.u8g, 0);
        u8g_DrawBox(&global.u8g, 0, 0, u8g_GetWidth(&global.u8g), u8g_GetHeight(&global.u8g));
    } while (u8g_NextPage(&global.u8g));
}

const u8g_fntpgm_uint8_t *select_font(uint8_t font)
{
    switch (font)
    {
    case MATRIX_ANIMATION_FONT_DEFAULT:
        return DEFAULTFONT;
        break;
    case MATRIX_ANIMATION_FONT_1:
    	return FONT_1;
    	break;
    case MATRIX_ANIMATION_FONT_2:
    	return FONT_2;
    	break;
    }

    return DEFAULTFONT;
}
