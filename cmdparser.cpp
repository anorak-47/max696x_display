
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "command_queue.h"
#include "commands.h"
#include "debug.h"
#include "global.h"
#include "test.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

#ifdef SB_WITH_CMDPARSER

struct _Command
{
    const char *cmd;
    const char *desc;
    uint8_t (*func_run_get_cmd)(void);
    uint8_t (*func_run_set_cmd)(char *input, uint8_t input_len);
};

typedef struct _Command Command;
unsigned int value;

#define CMDHLP(arg) arg

uint8_t cmd_help(void);
uint8_t cmd_version(void);

uint8_t cmd_dump_timer_get(void);
uint8_t cmd_dump_get(void);
uint8_t cmd_init_get(void);

uint8_t cmd_led_get(void);
uint8_t cmd_led_set(char *input, uint8_t input_len);

uint8_t cmd_brightness_get(void);
uint8_t cmd_brightness_set(char *input, uint8_t input_len);

uint8_t cmd_sleep_get(void);
uint8_t cmd_sleep_set(char *input, uint8_t input_len);

uint8_t cmd_plane_get(void);
uint8_t cmd_plane_set(char *input, uint8_t input_len);

uint8_t cmd_plane_cfg_get(void);
uint8_t cmd_plane_cfg_set(char *input, uint8_t input_len);

uint8_t cmd_test_draw_init_get(void);

uint8_t cmd_test_draw_get(void);
uint8_t cmd_test_draw_set(char *input, uint8_t input_len);

uint8_t cmd_test_max_get(void);

uint8_t cmd_run_cmd_get(void);
uint8_t cmd_run_cmd_set(char *input, uint8_t input_len);

const Command Commands[] PROGMEM = {{"h", "help", cmd_help, 0},
                                    {"v", "version", cmd_version, 0},

                                    {"ini", "init", cmd_init_get, 0},
                                    {"dmp", "dump", cmd_dump_get, 0},
									{"tim", "dump timer", cmd_dump_timer_get, 0 },
                                    {"led", "set led", cmd_led_get, cmd_led_set},
                                    {"bri", "set bri", cmd_brightness_get, cmd_brightness_set},
                                    {"slp", "set sleep", cmd_sleep_get, cmd_sleep_set},
                                    {"pla", "set plane", cmd_plane_get, cmd_plane_set},
                                    {"pcf", "set plane cfg", cmd_plane_cfg_get, cmd_plane_cfg_set},

                                    {"tdi", "init draw test", cmd_test_draw_init_get, 0},
                                    {"tdr", "run draw test", cmd_test_draw_get, cmd_test_draw_set},
                                    {"tmx", "run max test", cmd_test_max_get, 0},

                                    {"cmd", "run cmd", cmd_run_cmd_get, cmd_run_cmd_set},

                                    {0, 0, 0, 0}};

uint8_t ucInputPos = 0; // Wird gebraucht um ein Delete am Zeilenanfang zu verhindern

#define UART_INPUT_LENGTH 32
char acUARTRecData[UART_INPUT_LENGTH];

uint8_t parserProcessWithError(char *input, char **error);

void parserInit(void)
{
    printf_P(PSTR("Enter 'h' for help\n"));
}

//
// Handle uart input
//

void parserUARTHandleInput(void)
{
    uint8_t rc = 0;
    uint8_t ucData;
    char *pcErrPos;

    unsigned int rd = uart_getc();

    // wenn Zeichen empfangen
    if (HIGH_BYTE(rd) == 0)
    {
        ucData = LOW_BYTE(rd);

        // Eingabe mit \x0d (Enter) abgeschlossen ?
        if (ucData == '\n')
        {
            // CRLF
            // printf_P(PSTR("\n"));

            acUARTRecData[ucInputPos] = 0;

            ucInputPos--;
            if (acUARTRecData[ucInputPos] == '\r' || acUARTRecData[ucInputPos] == '\n')
                acUARTRecData[ucInputPos] = 0;

            ucInputPos--;
            if (acUARTRecData[ucInputPos] == '\r' || acUARTRecData[ucInputPos] == '\n')
                acUARTRecData[ucInputPos] = 0;

            // und Parser aufrufen. Im Fehlerflle bekommen wir mit pucErrPos
            // die Stelle des Fehlers (wenn mehrere Befehle hintereinander stehen)
            if ((rc = parserProcessWithError(&acUARTRecData[0], &pcErrPos)) == 0)
            {
                printf_P(PSTR("Ok\n"));
            }
            else
            {
                if (rc == 1)
                {
                    printf_P(PSTR("Invalid value: '%s'\n"), pcErrPos);
                }
                else if (rc == 2)
                {
                    printf_P(PSTR("No set\n"));
                }
                else if (rc == 3)
                {
                    printf_P(PSTR("No get\n"));
                }
                else
                {
                    printf_P(PSTR("Unknown error"));
                }
            }

            ucInputPos = 0;
        }
        else
        {
            // bei Del (\x08 oder \x7f für vt100 via putty)
            // letztes Zeichen (sofern wir nicht am Zeilenanfang sind) löschen
            if ((ucData == 0x08) || (ucData == 0x7f))
            {
                if (ucInputPos > 0)
                {
                    ucInputPos--;
                    printf_P(PSTR("\x08 \x08"));
                }
            }
            // ansonsten ausgeben
            else
            {
                acUARTRecData[ucInputPos] = ucData;
                ucInputPos++;

                uart_putc(ucData);
            }
        }
    }
    else if ((rd & UART_NO_DATA) != UART_NO_DATA)
    {
        printf_P(PSTR("uart error: 0x%X\r\n"), HIGH_BYTE(rd));
    }
}

uint8_t parserProcessWithError(char *input, char **error)
{
    uint8_t rc = 1;
    *error = input;

    Command command;
    memcpy_P(&command, &Commands[0], sizeof(Command));

    uint8_t cmd = 0;
    while (command.cmd != 0)
    {
        uint8_t cmd_len = strlen(command.cmd);
        if (strncmp(command.cmd, input, cmd_len) == 0)
        {
            if (input[cmd_len] == '=')
            {
                uint8_t input_len = strlen(input);
                *error = &input[cmd_len + 1];

                if (command.func_run_set_cmd == 0)
                {
                    *error = (char *)command.cmd;
                    return 2;
                }

                if (input_len > (cmd_len + 1))
                {
                    input_len -= cmd_len + 1;
                }
                else
                {
                    input_len = 0;
                }

                rc = command.func_run_set_cmd(&input[cmd_len + 1], input_len);
            }
            else
            {
                if (command.func_run_get_cmd == 0)
                {
                    *error = (char *)command.cmd;
                    return 3;
                }

                rc = command.func_run_get_cmd();
            }
        }

        cmd++;
        memcpy_P(&command, &Commands[cmd], sizeof(Command));
    }

    return rc;
}

bool getIntValue(const char *input, unsigned int *value)
{
    return sscanf(input, "%u", value) > 0;
}

bool getIntIntValue(const char *input, unsigned int *value1, unsigned int *value2)
{
    return sscanf(input, "%u %u", value1, value2) > 1;
}

void printIntValue(unsigned int value)
{
    printf_P(PSTR("%u\n"), value);
}

uint8_t cmd_help(void)
{
    uint8_t cmd = 0;

    Command command;
    memcpy_P(&command, &Commands[0], sizeof(Command));

    while (command.cmd != 0)
    {
        printf_P(PSTR("%s: %s%s%s %s\n"), command.cmd, (command.func_run_get_cmd != 0 ? "get" : ""),
                 ((command.func_run_get_cmd != 0 && command.func_run_set_cmd != 0) ? "/" : ""), (command.func_run_set_cmd != 0 ? "set" : ""),
                 command.desc);

        cmd++;
        memcpy_P(&command, &Commands[cmd], sizeof(Command));
    }
    return 0;
}

uint8_t cmd_version(void)
{
    printf_P(PSTR("max696x_cpu v" VERSION " " COMPILED_TS "\n"));
    return 0;
}

uint8_t cmd_init_get(void)
{

    return 0;
}

uint8_t cmd_dump_get(void)
{

    return 0;
}

uint8_t cmd_led_get(void)
{

    return 0;
}

uint8_t cmd_led_set(char *input, uint8_t input_len)
{
    unsigned int led;
    unsigned int enable;
    if (getIntIntValue(input, &led, &enable))
    {
        printf_P(PSTR("enable led %d: %d\n"), led, enable);

        return 0;
    }
    return 1;
}

uint8_t cmd_brightness_get(void)
{
    return 0;
}

uint8_t cmd_brightness_set(char *input, uint8_t input_len)
{
    unsigned int brightness;
    if (getIntValue(input, &brightness))
    {
        printf_P(PSTR("set brightness: %d\n"), brightness);
        u8g_SetContrast(&global.u8g, brightness);
        return 0;
    }
    return 1;
}

uint8_t cmd_sleep_get(void)
{
    return 0;
}

uint8_t cmd_sleep_set(char *input, uint8_t input_len)
{
    unsigned int pwr;
    if (getIntValue(input, &pwr))
    {
        printf_P(PSTR("sleep: %d\n"), pwr);
        if (pwr)
            u8g_SleepOff(&global.u8g);
        else
            u8g_SleepOn(&global.u8g);
        return 0;
    }
    return 1;
}

uint8_t cmd_plane_get(void)
{
    printf_P(PSTR("plane: %u\n"), get_plane());
    return 0;
}

uint8_t cmd_plane_set(char *input, uint8_t input_len)
{
    if (getIntValue(input, &value))
    {
        printf_P(PSTR("plane: %d\n"), value);
        set_plane(value);
        return 0;
    }
    return 1;
}

uint8_t cmd_plane_cfg_get(void)
{
    printf_P(PSTR("plane cfg: %u\n"), get_plane_config());
    return 0;
}

uint8_t cmd_plane_cfg_set(char *input, uint8_t input_len)
{
    if (getIntValue(input, &value))
    {
        printf_P(PSTR("plane cfg: %d\n"), value);
        set_plane_config(value);
        return 0;
    }
    return 1;
}

uint8_t cmd_test_draw_init_get(void)
{
    draw_test_init();
    return 0;
}

uint8_t cmd_test_draw_get(void)
{
    draw_test_1();
    return 0;
}

uint8_t cmd_test_draw_set(char *input, uint8_t input_len)
{
    if (getIntValue(input, &value))
    {
        printf_P(PSTR("test: %d\n"), value);
        switch (value)
        {
        case 0:
        case 1:
            draw_test_1();
            break;
        case 2:
            draw_test_2();
            break;
        case 3:
            draw_test_3();
            break;
        case 4:
            draw_test_4();
            break;
        }
        return 0;
    }
    return 1;
}

uint8_t cmd_run_cmd_get(void)
{
    return 0;
}

uint8_t cmd_run_cmd_set(char *input, uint8_t input_len)
{
    if (getIntValue(input, &value))
    {
        cmd_queue_data_t cmd_data;
        memset(&cmd_data, 0, sizeof(cmd_queue_data_t));

        printf_P(PSTR("cmd: %d\n"), value);

        unsigned int arg1 = 0;
        if (getIntIntValue(input, &value, &arg1))
        {
            printf_P(PSTR("arg1: %u\n"), arg1);
        }

        switch (value)
        {
        case 0:
            cmd_data.cmd = MATRIX_CMD_INITIALIZE;
            break;
        case 1:
            cmd_data.cmd = MATRIX_CMD_BRIGHTNESS_INC;
            break;
        case 2:
            cmd_data.cmd = MATRIX_CMD_BRIGHTNESS_DEC;
            break;
        case 3:
            cmd_data.cmd = MATRIX_CMD_BRIGHTNESS_SET;
            cmd_data.data[0] = arg1;
            cmd_data.data_length = 1;
            break;
        case 4:
            cmd_data.cmd = MATRIX_CMD_SLEEP_SET;
            cmd_data.data[0] = arg1;
            cmd_data.data_length = 1;
            break;
        case 5:
            cmd_data.cmd = MATRIX_CMD_INVERT_SET;
            cmd_data.data[0] = arg1;
            cmd_data.data_length = 1;
            break;

        case 10:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_STOP;
            cmd_data.data_length = 0;
        }
        break;
        case 11:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_RESTART;
            cmd_data.data_length = 0;
        }
        break;
        case 12:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_FASTER;
            cmd_data.data_length = 0;
        }
        break;
        case 13:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_SLOWER;
            cmd_data.data_length = 0;
        }
        break;
        case 14:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_SET_SPEED;
            cmd_data.data[0] = arg1;
            cmd_data.data_length = 1;
        }
        break;
        case 20:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_SHOW_TEXT;
            cmd_text *ct = (cmd_text *)cmd_data.data;
            strncpy_P(ct->msg.text, PSTR("Anorak"), MAX_TEXT_LENGTH);
            ct->msg.direction = MATRIX_ANIMATION_DIRECTION_LEFT;
            ct->msg.speed = 0;
            ct->msg.color = 2;
            ct->msg.font = 2;
            ct->msg.duration = 0;
            cmd_data.data_length = strlen(ct->msg.text) + 1 + sizeof(cmd_animation);
        }
        break;
        case 21:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_SCROLL_TEXT;
            cmd_scroll_text *cts = (cmd_scroll_text *)cmd_data.data;
            strncpy_P(cts->msg.text, PSTR("Anorak splitbrain"), MAX_TEXT_LENGTH);
            cts->msg.direction = (arg1 == 0 ? MATRIX_ANIMATION_DIRECTION_LEFT : arg1);
            cts->msg.speed = 5;
            cts->msg.color = 3;
            cts->msg.font = 1;
            cts->msg.duration = 180;
            cmd_data.data_length = strlen(cts->msg.text) + 1 + sizeof(cmd_animation);
        }
        break;
        case 22:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_SWEEP;
            cmd_scroll_text *cts = (cmd_scroll_text *)cmd_data.data;
            cts->msg.direction = (arg1 == 0 ? MATRIX_ANIMATION_DIRECTION_LEFT : arg1);
            cts->msg.speed = 5;
            cts->msg.color = 0;
            cts->msg.font = 0;
            cts->msg.duration = 180;
            cmd_data.data_length = sizeof(cmd_animation);
        }
        break;
        case 23:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_BOX;
            cmd_scroll_text *cts = (cmd_scroll_text *)cmd_data.data;
            cts->msg.direction = (arg1 == 0 ? MATRIX_ANIMATION_DIRECTION_LEFT : arg1);
            cts->msg.speed = 5;
            cts->msg.color = 3;
            cts->msg.font = 0;
            cts->msg.duration = 180;
            cmd_data.data_length = sizeof(cmd_animation);
        }
        break;
        case 24:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_TYPEMATRIX;
            cmd_scroll_text *cts = (cmd_scroll_text *)cmd_data.data;
            cts->msg.direction = 0;
            cts->msg.speed = 5;
            cts->msg.color = 0;
            cts->msg.font = 0;
            cts->msg.duration = MATRIX_ANIMATION_RUN_FOREVER;
            cmd_data.data_length = sizeof(cmd_animation);
        }
        break;
        case 25:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_TYPEMATRIX_KEY;
            cmd_typematrix_key *cts = (cmd_typematrix_key *)cmd_data.data;
            cts->msg.row_number = arg1;
            cts->msg.row = 0x00AADD00;
            cmd_data.data_length = sizeof(cmd_typematrix_key);
        }
        break;
        case 26:
        {
            cmd_data.cmd = MATRIX_CMD_ANIMATE_INFO_TEXT;
            cmd_text *ct = (cmd_text *)cmd_data.data;
            strncpy_P(ct->msg.text, PSTR("InfoText"), MAX_TEXT_LENGTH);
            ct->msg.direction = MATRIX_ANIMATION_DIRECTION_LEFT;
            ct->msg.speed = 0;
            ct->msg.color = 2;
            ct->msg.font = 2;
            ct->msg.duration = 0;
            cmd_data.data_length = strlen(ct->msg.text) + 1 + sizeof(cmd_animation);
        }
        break;
        }

        cmd_enqueue_data(cmd_data.cmd, cmd_data.data, cmd_data.data_length);

        return 0;
    }
    return 1;
}

uint8_t cmd_test_max_get(void)
{
    max_test();
    return 0;
}

uint8_t cmd_dump_timer_get(void)
{
#ifdef SB_ENABLE_DUMP
	dumpTimer();
#endif
	return 0;
}

#endif
