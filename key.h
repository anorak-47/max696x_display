#ifndef _KEYBUTTON_H_
#define _KEYBUTTON_H_

/************************************************************************/
/*                                                                      */
/*                      Debouncing 8 Keys                               */
/*                      Sampling 4 Times                                */
/*                      With Repeat Function                            */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/

#define KEY0 6
#define KEY1 7

#define KEY_MASK(arg) (1 << arg)

#define KEY_S1 KEY_MASK(KEY0)
#define KEY_S2 KEY_MASK(KEY1)

#define ALL_KEYS (1 << KEY0 | 1 << KEY1)

void initKeys(void);
void checkKeys(void);
void keypressMainLoop(void);

uint8_t get_key_press(uint8_t key_mask);
uint8_t get_key_rpt(uint8_t key_mask);
uint8_t get_key_short(uint8_t key_mask);
uint8_t get_key_long(uint8_t key_mask);



#endif

