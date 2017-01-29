
#include "max696x_device.h"
#include "command_queue.h"
#include "commands.h"
#include "global.h"
#include <avr/pgmspace.h>
#include <string.h>

max696x max6960;

void initializeMatrixDisplay()
{
    u8g_Init(&global.u8g, &u8g_dev_max696x);
    u8g_SetContrast(&global.u8g, global.state.brightness);

    global.state.fault = max6960.fault_check();

    cmd_queue_data_t cmd_data;
    cmd_data.cmd = MATRIX_CMD_ANIMATE_SCROLL_TEXT;
    cmd_scroll_text *cts = (cmd_scroll_text *)cmd_data.data;
    strncpy_P(cts->msg.text, PSTR("max696x_cpu v" VERSION), MAX_TEXT_LENGTH);
    cts->msg.direction = MATRIX_ANIMATION_DIRECTION_LEFT;
    cts->msg.text[MAX_TEXT_LENGTH-1] = '\0';
    cts->msg.speed = 5;
    cts->msg.color = 3;
    cts->msg.font = 0;
    cts->msg.duration = 5;
    cmd_data.data_length = strlen(cts->msg.text) + 1 + sizeof(cmd_animation);
    cmd_enqueue_data(cmd_data.cmd, cmd_data.data, cmd_data.data_length);
}
