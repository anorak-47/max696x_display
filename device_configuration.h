#ifndef __DEVICE_CONFIGURATION__
#define __DEVICE_CONFIGURATION__

#define SB_TIMER_TIMER0
#define SB_LOCAL_I2C_SLAVE_ADDRESS (0x16) // Address is: 0x16, 22

#define SB_WITH_CMDPARSER
#define SB_ENABLE_DEBUG_BASE
//#define SB_ENABLE_DUMP
//#define SB_ENABLE_I2C_DEBUG
//#define SB_ENABLE_I2CCOMM_DEBUG
//#define SB_ENABLE_ANIMATION_QUEUE_DEBUG
//#define SB_ENABLE_COMMAND_QUEUE_DEBUG
#define SB_ENABLE_DEBUG_IRQ
//#define SB_ENABLE_COMMAND_DEBUG
//#define SB_ENABLE_ANIMATION_DEBUG

//#define DEFAULTFONT u8g_font_orgv01
#define DEFAULTFONT u8g_font_04b_03br
//#define DEFAULTFONT u8g_font_04b_24r
//#define DEFAULTFONT u8g_font_profont10r
#define FONT_1 u8g_font_5x7r
#define FONT_2 u8g_font_4x6

#endif
