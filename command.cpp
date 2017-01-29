
#include "command.h"
#include "animation/animation.h"
#include "animation/animation_queue.h"
#include "animation/animation_utils.h"
#include "command_queue.h"
#include "commands.h"
#include "device_configuration.h"
#include "global.h"
#include "max696x_device.h"
#include <string.h>

#if defined(SB_ENABLE_DEBUG_BASE) && defined(SB_ENABLE_COMMAND_DEBUG)
#define DEBUG
#define DEBUG_UART
#endif

#include "debug.h"

struct _command_payload_t
{
    uint8_t data[CMD_PAYLOAD_SIZE];
    uint8_t data_length;
};

typedef _command_payload_t command_payload_t;

struct _command_t
{
    uint8_t cmd;
    command_payload_t payload;
};

typedef struct _command_t command_t;

struct _command_description_t
{
    uint8_t cmd;
    void (*func_run_cmd)(command_t const *);
};

typedef struct _command_description_t command_description_t;

void cmd_init(command_t const *cmd);
void cmd_brightness_inc(command_t const *cmd);
void cmd_brightness_dec(command_t const *cmd);
void cmd_brightness_set(command_t const *cmd);
void cmd_sleep_set(command_t const *cmd);
void cmd_invert_set(command_t const *cmd);
void cmd_enqueue_animation(command_t const *cmd);
void cmd_control_animation(command_t const *cmd);
void cmd_control_animation_set_typematrix_key(command_t const *cmd);
void cmd_control_animation_set_lock_state(command_t const *cmd);
void cmd_control_show_info_text(command_t const *cmd);

const command_description_t commands[] PROGMEM = {{MATRIX_CMD_INITIALIZE, cmd_init},
                                                  {MATRIX_CMD_BRIGHTNESS_SET, cmd_brightness_set},
                                                  {MATRIX_CMD_BRIGHTNESS_INC, cmd_brightness_inc},
                                                  {MATRIX_CMD_BRIGHTNESS_DEC, cmd_brightness_dec},
                                                  {MATRIX_CMD_SLEEP_SET, cmd_sleep_set},
                                                  {MATRIX_CMD_INVERT_SET, cmd_invert_set},
                                                  {MATRIX_CMD_ANIMATE_STOP, cmd_control_animation},
                                                  {MATRIX_CMD_ANIMATE_RESTART, cmd_control_animation},
                                                  {MATRIX_CMD_ANIMATE_START_NEXT, cmd_control_animation},
                                                  {MATRIX_CMD_ANIMATE_START_PREV, cmd_control_animation},
                                                  {MATRIX_CMD_ANIMATE_TOGGLE, cmd_control_animation},
                                                  {MATRIX_CMD_ANIMATE_FASTER, cmd_control_animation},
                                                  {MATRIX_CMD_ANIMATE_SLOWER, cmd_control_animation},
                                                  {MATRIX_CMD_ANIMATE_SET_SPEED, cmd_control_animation},
                                                  {MATRIX_CMD_ANIMATE_TYPEMATRIX_KEY, cmd_control_animation_set_typematrix_key},
                                                  {MATRIX_CMD_ANIMATE_LOCK_STATE, cmd_control_animation_set_lock_state},
                                                  {MATRIX_CMD_ANIMATE_SHOW_TEXT, cmd_enqueue_animation},
                                                  {MATRIX_CMD_ANIMATE_SCROLL_TEXT, cmd_enqueue_animation},
                                                  {MATRIX_CMD_ANIMATE_SWEEP, cmd_enqueue_animation},
												  {MATRIX_CMD_ANIMATE_BOX, cmd_enqueue_animation},
                                                  {MATRIX_CMD_ANIMATE_TYPEMATRIX, cmd_enqueue_animation},
                                                  {MATRIX_CMD_ANIMATE_INFO_TEXT, cmd_control_show_info_text},
                                                  {0, 0}};

void cmd_init(command_t const *cmd)
{
    LS_("cmd_init");
    cmd_queue_data_t *cmd_head;
    while (cmd_dequeue_head(&cmd_head))
        ;

    animation_queue_data_t *animation_head;
    while (animation_get_queue_head(&animation_head))
        ;

    stop_animation();
    clean_matrix_display();

    u8g_SetContrast(&global.u8g, global.state.brightness);
}

void cmd_brightness_set(command_t const *cmd)
{
    if (cmd->payload.data_length == 0)
        return;
    global.state.brightness = cmd->payload.data[0];
    LV_("brightness:%u", global.state.brightness);
    global.state.brightness = u8g_SetContrast(&global.u8g, global.state.brightness);
    // eeprom_write_byte();
}

void cmd_brightness_inc(command_t const *cmd)
{
    if (global.state.brightness > 250)
        global.state.brightness = 255;
    else
        global.state.brightness += 5;

    u8g_SetContrast(&global.u8g, global.state.brightness);
}

void cmd_brightness_dec(command_t const *cmd)
{
    if (global.state.brightness > 5)
        global.state.brightness -= 5;
    else
        global.state.brightness = 0;

    u8g_SetContrast(&global.u8g, global.state.brightness);
}

void cmd_sleep_set(command_t const *cmd)
{
    if (cmd->payload.data_length == 0)
        return;

    if (cmd->payload.data[0] > 0)
        u8g_SleepOn(&global.u8g);
    else
        u8g_SleepOff(&global.u8g);
}

void cmd_invert_set(command_t const *cmd)
{
}

void cmd_control_show_info_text(command_t const *cmd)
{
    LS_("cmd info");

    cmd_animation const *ani = (cmd_animation const *)cmd->payload.data;

    LV_("dir:%u", ani->msg.direction);
    LV_("dur:%u", ani->msg.duration);
    LV_("spe:%u", ani->msg.speed);
    LV_("col:%u", ani->msg.color);
    LV_("fnt:%u", ani->msg.font);

    animation_queue_data_t data;

    data.animation = cmd->cmd;
    data.direction = ani->msg.direction;
    data.duration = ani->msg.duration;
    data.speed = ani->msg.speed;
    data.font = ani->msg.font;
    data.color = ani->msg.color;
    data.data_length = cmd->payload.data_length - sizeof(cmd_animation);

    if (data.data_length)
    {
        if (data.data_length >= MAX_TEXT_LENGTH)
            data.data_length = MAX_TEXT_LENGTH - 1;

        memcpy(data.data, cmd->payload.data + sizeof(cmd_animation), data.data_length);
        data.data[MAX_TEXT_LENGTH - 1] = 0;
    }
    else
    {
        data.data[0] = 0;
    }

    animation_show_info_text(&data);
}

void cmd_control_animation_set_typematrix_key(command_t const *cmd)
{
    if (cmd->payload.data_length < sizeof(cmd_typematrix_key))
        return;

    LS_("cmd type");

    cmd_typematrix_key const *key = (cmd_typematrix_key const *)cmd->payload.data;
    animation_set_typematrix_key(key->msg.row_number, key->msg.row);
}

void cmd_control_animation_set_lock_state(command_t const *cmd)
{
    LS_("cmd lock");

    if (cmd->payload.data_length < sizeof(cmd_lock_state))
        return;

    cmd_lock_state const *state = (cmd_lock_state const *)cmd->payload.data;
    animation_set_lock_state(state->msg.locks);
}

void cmd_control_animation(command_t const *cmd)
{
    LV_("cmd_control_animation 0x%x l:%u", cmd->cmd, cmd->payload.data_length);

    if (cmd->cmd == MATRIX_CMD_ANIMATE_STOP)
    {
        LS_("stop");
        if (animation_is_running())
            stop_animation();
    }

    else if (cmd->cmd == MATRIX_CMD_ANIMATE_RESTART)
    {
        LS_("restart");
        if (animation_is_running())
            stop_animation();
        start_animation();
    }

    else if (cmd->cmd == MATRIX_CMD_ANIMATE_FASTER)
    {
        LS_("faster");
        animation_increase_speed();
    }

    else if (cmd->cmd == MATRIX_CMD_ANIMATE_SLOWER)
    {
        LS_("slower");
        animation_decrease_speed();
    }

    else if (cmd->cmd == MATRIX_CMD_ANIMATE_SET_SPEED)
    {
        LS_("speed");
        if (cmd->payload.data_length > 0)
            animation_set_speed(cmd->payload.data[0]);
    }

    else if (cmd->cmd == MATRIX_CMD_ANIMATE_START_NEXT)
    {
        LS_("next");
        animation_next();
    }

    else if (cmd->cmd == MATRIX_CMD_ANIMATE_START_PREV)
    {
        LS_("prev");
        animation_previous();
    }

    else if (cmd->cmd == MATRIX_CMD_ANIMATE_TOGGLE)
    {
        LS_("toggle");
        animation_toggle();
    }
}

void cmd_enqueue_animation(command_t const *cmd)
{
    LV_("cmd_enqueue_animation 0x%x l:%u", cmd->cmd, cmd->payload.data_length);

    cmd_animation const *ani = (cmd_animation const *)cmd->payload.data;

    /*
    LV_("dir:%u", ani->msg.direction);
    LV_("dur:%u", ani->msg.duration);
    LV_("spe:%u", ani->msg.speed);
    LV_("col:%u", ani->msg.color);
    LV_("fnt:%u", ani->msg.font);
    */

    animation_queue_data_t data;

    data.animation = cmd->cmd;
    data.direction = ani->msg.direction;
    data.duration = ani->msg.duration;
    data.speed = ani->msg.speed;
    data.font = ani->msg.font;
    data.color = ani->msg.color;
    data.data_length = cmd->payload.data_length - sizeof(cmd_animation) - 1;

    if (data.data_length)
    {
        if (data.data_length >= MAX_TEXT_LENGTH)
            data.data_length = MAX_TEXT_LENGTH - 1;

        memcpy(data.data, cmd->payload.data + sizeof(cmd_animation), data.data_length);
        data.data[MAX_TEXT_LENGTH - 1] = 0;
    }
    else
    {
        data.data[0] = 0;
    }

    LV_("dir:%u", data.direction);
    LV_("dur:%u", data.duration);
    LV_("spe:%u", data.speed);
    LV_("col:%u", data.color);
    LV_("fnt:%u", data.font);
    LV_("enq ani l:%u [%s]", data.data_length, (char *)data.data);

    animation_enqueue_data(&data);
}

bool handle_command(cmd_queue_data_t *cmd_data)
{
    command_description_t cmddesc;
    bool command_found = false;
    uint8_t cmd_count = 0;

    memcpy_P(&cmddesc, &commands[0], sizeof(command_description_t));

    while (cmddesc.cmd != 0 && !command_found)
    {
        if (cmddesc.cmd == cmd_data->cmd)
        {
            command_found = true;
        }

        if (!command_found)
        {
            cmd_count++;
            memcpy_P(&cmddesc, &commands[cmd_count], sizeof(command_description_t));
        }
    }

    if (command_found)
    {
    	LV_("command:0x%X %u", cmddesc.cmd, cmddesc.cmd);

        command_t command;

        command.cmd = cmddesc.cmd;

        if (cmd_data->data_length)
            memcpy(&command.payload.data, cmd_data->data, cmd_data->data_length);
        command.payload.data_length = cmd_data->data_length;

        if (cmddesc.func_run_cmd)
            cmddesc.func_run_cmd(&command);
    }
    else
    {
        LV_("unknown cmd! 0x%x %u", cmd_data->cmd, cmd_data->cmd);
    }

    return true;
}

void reset_command(command_t *cmd)
{
    cmd->cmd = 0;
    cmd->payload.data_length = 0;
}

void initCommand()
{
    // reset_command(&current_command);
}

void commandMainLoop()
{
    static cmd_queue_data_t *head;

    if (cmd_get_queue_head(&head))
    {
        if (handle_command(head))
            cmd_dequeue_head(&head);
    }
}
