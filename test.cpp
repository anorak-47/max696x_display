
#include "test.h"
#include "debug.h"
#include "global.h"
#include "max696x/max696x.h"
#include <string.h>

static max696x max6960;

void max_test()
{
    // call the max696x initialisation function
    max6960.init((GPC_PIXEL_INTENSITY_2BPP | GPC_RIPPLESYNC_ENABLED | GPC_DISPLAY_MONO | GPC_ENABLE |
                  GPC_PIXEL_NORMAL),        // Global Panel Config register setting
                 0x80,                      // Panel Intensity
                 PIS_FULL,                  // Pixel Intensity Scale
                 NUMBER_OF_DRIVER_DEVICES,  // Number of driver devices
                 NUMBER_OF_DRIVERS_PER_ROW, // Number of drivers per row
                 PCR_RATE);

    // dump the register settings of the display drivers
    max6960.reg_dump();

    // test display pixels
    if (max6960.display_test())
    {
        LS_("WARNING: DISPLAY HAS A FAULT!");
    }

    // max6960.command_wr(GLOBAL_PLANE_COUNTER, 2, GLOBAL);

    // uint8_t reg = max6960.command_rd(GLOBAL_PANEL_CONFIG, GLOBAL);
    // reg |= GPC_PLANE0_2BPP;
    // max6960.command_wr(GLOBAL_PANEL_CONFIG, reg, GLOBAL);

    uint8_t planeData[64];
    memset(planeData, 0, 64);

    planeData[0] = 0xF1;
    planeData[1] = 0xF2;
    planeData[2] = 0xF3;
    planeData[3] = 0xF4;
    planeData[4] = 0xF5;
    planeData[5] = 0xF6;
    planeData[6] = 0xF7;
    planeData[7] = 0xF8;
    planeData[8] = 0xF9;
    planeData[9] = 0xFA;
    planeData[10] = 0xFB;
    planeData[11] = 0xFC;
    planeData[12] = 0xFD;
    planeData[13] = 0xFE;
    planeData[14] = 0xFF;
    planeData[15] = 0x10;

    planeData[0 + 16] = 0xFF;
    planeData[1 + 16] = 0x2;
    planeData[2 + 16] = 0x3;
    planeData[3 + 16] = 0x4;
    planeData[4 + 16] = 0x5;
    planeData[5 + 16] = 0x6;
    planeData[6 + 16] = 0x7;
    planeData[7 + 16] = 0x8;
    planeData[8 + 16] = 0x9;
    planeData[9 + 16] = 0xA;
    planeData[10 + 16] = 0xB;
    planeData[11 + 16] = 0xC;
    planeData[12 + 16] = 0xD;
    planeData[13 + 16] = 0xE;
    planeData[14 + 16] = 0xF;
    planeData[15 + 16] = 0xFF;

    max6960.mem_update(planeData, 32);
}

void set_plane(uint8_t plane)
{
    max6960.command_wr(GLOBAL_PLANE_COUNTER, plane, GLOBAL);
}

uint8_t get_plane()
{
    return max6960.command_rd(GLOBAL_PLANE_COUNTER, GLOBAL);
}

void set_plane_config(uint8_t cfg)
{
    max6960.command_wr(GLOBAL_PANEL_CONFIG, cfg, GLOBAL);
}

uint8_t get_plane_config()
{
    return max6960.command_rd(GLOBAL_PANEL_CONFIG, GLOBAL);
}

void draw_test_init()
{
    u8g_Init(&global.u8g, &u8g_dev_max696x);
    u8g_SetContrast(&global.u8g, 12);
}

void draw_test_1()
{
    char b[20];

    u8g_FirstPage(&global.u8g);
    do
    {
        u8g_SetFont(&global.u8g, DEFAULTFONT);
        u8g_SetColorIndex(&global.u8g, 2);

        strcpy_P(b, PSTR("ABCDEFGHIJ"));
        u8g_DrawStr(&global.u8g, 1, 7, b);

    } while (u8g_NextPage(&global.u8g));
}

void draw_test_2()
{
    u8g_FirstPage(&global.u8g);
    do
    {
        u8g_SetFont(&global.u8g, DEFAULTFONT);

        for (uint8_t c = 0; c < 8; c++)
        {
            u8g_SetColorIndex(&global.u8g, c);

            for (uint8_t x = 0; x < u8g_GetWidth(&global.u8g); x++)
            {
                u8g_DrawPixel(&global.u8g, x, c);
            }
        }

    } while (u8g_NextPage(&global.u8g));
}

void draw_test_3()
{
    u8g_FirstPage(&global.u8g);
    do
    {
        u8g_SetFont(&global.u8g, DEFAULTFONT);
        uint8_t color1 = 0;
        uint8_t color2 = 3;
        for (uint8_t x = 0; x < u8g_GetWidth(&global.u8g); x++)
        {
        	u8g_SetColorIndex(&global.u8g, color1++);
            u8g_DrawPixel(&global.u8g, x, 0);

            u8g_SetColorIndex(&global.u8g, color2--);
            u8g_DrawPixel(&global.u8g, x, 7);
        }

    } while (u8g_NextPage(&global.u8g));
}

void draw_test_4()
{
    u8g_FirstPage(&global.u8g);
    do
    {
        u8g_SetFont(&global.u8g, DEFAULTFONT);
    	u8g_SetColorIndex(&global.u8g, 3);
        u8g_DrawBox(&global.u8g, 0, 0, u8g_GetWidth(&global.u8g), u8g_GetHeight(&global.u8g));

    } while (u8g_NextPage(&global.u8g));
}
