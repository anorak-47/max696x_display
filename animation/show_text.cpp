
#include "show_text.h"
#include "../device_configuration.h"
#include "../global.h"
#include "animation_utils.h"

#if defined(SB_ENABLE_DEBUG_BASE) && defined(SB_ENABLE_ANIMATION_DEBUG)
#define DEBUG
#define DEBUG_UART
#endif

#include "../debug.h"

void show_text(animation_queue_data_t const *data)
{
    LV_("st:[%s]", (char *)data->data);

    u8g_FirstPage(&global.u8g);
    do
    {
        u8g_SetFont(&global.u8g, select_font(data->font));
        u8g_SetColorIndex(&global.u8g, data->color);

        u8g_DrawStr(&global.u8g, 1, 7, (char *)data->data);

    } while (u8g_NextPage(&global.u8g));
}
