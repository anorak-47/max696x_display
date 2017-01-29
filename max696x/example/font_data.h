#ifndef __FONT_DATA_H__
#define __FONT_DATA_H__

#include <avr/pgmspace.h>

#define START_CHAR 1
#define END_CHAR 255
#define CHAR_LEN 5

extern prog_uint8_t font_data[END_CHAR-START_CHAR+1][CHAR_LEN];

#endif // __FONT_DATA_H__

