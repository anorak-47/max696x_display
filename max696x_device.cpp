
#include "max696x_device.h"
#include "command_queue.h"
#include "commands.h"
#include "global.h"
#include <avr/pgmspace.h>
#include <string.h>

#ifdef SB_ENABLE_DEBUG_BASE
#define DEBUG
#define DEBUG_UART
#endif

#include "debug.h"

max696x max6960;

void initializeMatrixDisplay()
{
    u8g_Init(&global.u8g, &u8g_dev_max696x);
    u8g_SetContrast(&global.u8g, global.state.brightness);

    if (global.state.config.do_fault_check_on_init)
        global.state.fault = max6960.fault_check();

    if ((PIND & (1 << PD3)) == 0)
    {
        // PD3 is LOW
        LS_("init: PD3 is LOW -> sleep off");
        u8g_SleepOff(&global.u8g);
        //EICRA |= (0 << ISC11) | (1 << ISC10); // INT1 auf Wechsel der Flanke
    }
    else
    {
        // PD3 high HIGH
        LS_("init: PD3 is HIGH -> sleep on");
        u8g_SleepOn(&global.u8g);
        //EICRA |= (0 << ISC11) | (0 << ISC10); // INT1 auf Low Level
    }

    if (global.state.config.show_version_on_init)
    {
        cmd_queue_data_t cmd_data;
        cmd_data.cmd = MATRIX_CMD_ANIMATE_SCROLL_TEXT;
        cmd_scroll_text *cts = (cmd_scroll_text *)cmd_data.data;
        strcpy_P(cts->msg.text, PSTR("max696x_cpu v" VERSION));
        cts->msg.direction = MATRIX_ANIMATION_DIRECTION_LEFT;
        cts->msg.text[MAX_TEXT_LENGTH - 1] = '\0';
        cts->msg.speed = 5;
        cts->msg.color = 3;
        cts->msg.font = 0;
        cts->msg.duration = 5;
        cmd_data.data_length = strlen(cts->msg.text) + 1 + sizeof(cmd_animation);
        cmd_enqueue_data(cmd_data.cmd, cmd_data.data, cmd_data.data_length);
    }
}
