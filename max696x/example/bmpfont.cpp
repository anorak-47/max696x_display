#include "bmpfont.h"

unsigned char * get_char_data(unsigned char c, unsigned char * pos)
{
  unsigned char data = 0;
  if ((c < START_CHAR) || (c >= END_CHAR)) {
    return 0x00;
  }
  return (unsigned char *) memcpy_P(pos, &(font_data[(uint8_t)c - START_CHAR]), CHAR_LEN);
}
