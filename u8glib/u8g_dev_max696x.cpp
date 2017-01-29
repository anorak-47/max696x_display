/*

  u8g_dev_ht1632.c

  1-Bit (BW) Driver for HT1632 controller

  Universal 8bit Graphics Library

  Copyright (c) 2013, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "u8g.h"
#include "../max696x/max696x.h"

#if MAX696x_DEBUG == 1
#define DEBUG
#define DEBUG_UART
#endif

#include "../debug.h"

#define WIDTH NUMBER_OF_DRIVERS_PER_ROW * 16
#define HEIGHT (NUMBER_OF_DRIVER_DEVICES / NUMBER_OF_DRIVERS_PER_ROW) * 8
#define PAGE_HEIGHT 8

extern max696x max6960;

void max696x_init(u8g_t *u8g)
{
    // call the max696x initialisation function
    max6960.init((GPC_PIXEL_INTENSITY_2BPP | GPC_RIPPLESYNC_ENABLED | GPC_DISPLAY_MONO | GPC_ENABLE |
                  GPC_PIXEL_NORMAL),        // Global Panel Config register setting
                 0x80,                      // Panel Intensity
                 PIS_FULL,                  // Pixel Intensity Scale
                 NUMBER_OF_DRIVER_DEVICES,  // Number of driver devices
                 NUMBER_OF_DRIVERS_PER_ROW, // Number of drivers per row
                 PCR_RATE);

#if MAX696x_DEBUG == 2
    // store the mode of operation of the display, 1 BPP or 2 BPP
    uint8_t mode = max6960.command_rd(GLOBAL_PANEL_CONFIG, GLOBAL) & GPC_PI;

    // dump the register settings of the display drivers
    max6960.reg_dump();

    if (mode == GPC_PIXEL_INTENSITY_1BPP)
    {
        LS_("GPC_PIXEL_INTENSITY_1BPP");
    }
    else if (mode == GPC_PIXEL_INTENSITY_2BPP)
    {
        LS_("GPC_PIXEL_INTENSITY_2BPP");
    }

    // test display pixels
    if (max6960.display_test())
    {
        LS_("WARNING: DISPLAY HAS A FAULT!");
    }
#endif
}

void max696x_transfer_data(u8g_t *u8g, uint8_t page, uint8_t cnt, uint8_t *data)
{
    // LV_("transfer p:%u, cnt:%u", page, cnt);

    /*
    for (uint8_t i = 0; i < cnt; i++)
    {
        LV_("%u: 0x%X", i, data[i]);
    }
    */

    unsigned int panel = 0;

#if MAX696x_DEBUG == 2
    // read the current memory plane being displayed
    panel = max6960.command_rd(GLOBAL_PANEL_CONFIG, GLOBAL);
    panel = panel & 0x8C; // mask out bits we're not interested in

    if (panel == GPC_PLANE1_2BPP)
    {
        LS_("is plane 0");
    }
    else if (panel == GPC_PLANE0_2BPP)
    {
        LS_("is plane 1");
    }
#endif

    // write to display memory, copy the whole planeData array to the display
    max6960.mem_update(data, cnt);

    // read the current memory plane being displayed
    panel = max6960.command_rd(GLOBAL_PANEL_CONFIG, GLOBAL);
    panel = panel & 0x8C; // mask out bits we're not interested in

    if (panel == GPC_PLANE1_2BPP)
    {
        //LS_("show plane 0");
        max6960.command_wr(GLOBAL_PLANE_COUNTER, 0, GLOBAL);
    }
    else if (panel == GPC_PLANE0_2BPP)
    {
        //LS_("show plane 1");
        max6960.command_wr(GLOBAL_PLANE_COUNTER, 1, GLOBAL);
    }
}

void max696x_set_contrast(u8g_t *u8g, uint8_t value)
{
    max6960.command_wr(PANEL_INTENSITY, value, GLOBAL);
}

void max696x_sleep_on()
{
    uint8_t data = max6960.command_rd(GLOBAL_PANEL_CONFIG, GLOBAL);
    max6960.command_wr(GLOBAL_PANEL_CONFIG, data & ~(GPC_ENABLE), GLOBAL);
    // shutdown = 1;
}

void max696x_sleep_off()
{
    uint8_t data = max6960.command_rd(GLOBAL_PANEL_CONFIG, GLOBAL);
    max6960.command_wr(GLOBAL_PANEL_CONFIG, data | GPC_ENABLE, GLOBAL);
    // shutdown = 0;
}

void u8g_pb_Clear_max696x(u8g_pb_t *b)
{
    uint8_t *ptr = (uint8_t *)b->buf;
    uint8_t *end_ptr = ptr;
    end_ptr += b->width * 2;
    do
    {
        *ptr++ = 0;
    } while (ptr != end_ptr);
}

uint8_t u8g_dev_max696x_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
    switch (msg)
    {
    case U8G_DEV_MSG_INIT:
        max696x_init(u8g);
        break;
    case U8G_DEV_MSG_STOP:
        break;
    case U8G_DEV_MSG_PAGE_FIRST:
    {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_pb_Clear_max696x(pb);
        u8g_page_First(&(pb->p));
    }
    break;
    case U8G_DEV_MSG_PAGE_NEXT:
    {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

        /* current page: pb->p.page */
        /* ptr to the buffer: pb->buf */
        max696x_transfer_data(u8g, pb->p.page, pb->width * 2, (uint8_t *)pb->buf);
    }
    break;
    case U8G_DEV_MSG_CONTRAST:
        /* values passed to SetContrast() are between 0 and 255 */
        max696x_set_contrast(u8g, (*(uint8_t *)arg));
        return 1;
    case U8G_DEV_MSG_SLEEP_ON:
        max696x_sleep_on();
        break;
    case U8G_DEV_MSG_SLEEP_OFF:
        max696x_sleep_off();
        break;
    }
    return u8g_dev_pb8v2_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_max696x_buf[WIDTH * 2] U8G_NOCOMMON;
u8g_pb_t u8g_dev_max696x_pb = {{8, HEIGHT, 0, 0, 0}, WIDTH, u8g_dev_max696x_buf};
u8g_dev_t u8g_dev_max696x = {u8g_dev_max696x_fn, &u8g_dev_max696x_pb, u8g_com_null_fn};
