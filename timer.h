#ifndef _TIMER_H_
#define _TIMER_H_

#include <inttypes.h>

void initTimer(void);
void resetTimer(void);
void dumpTimer(void);

int8_t addTimer(uint16_t msecs, void (*f)(void *closure), void *closure);
int8_t addLongTimer(uint16_t secs, uint16_t msecs, void (*f)(void *closure), void *closure);
int8_t addLoopTimer(uint16_t msecs, void (*f)(void *closure), void *closure);

int8_t createTimer(uint16_t msecs, void (*f)(void *closure), void *closure);
int8_t restartTimer(int8_t t);
int8_t stopTimer(int8_t t);
int8_t pauseTimer(int8_t t);
int8_t continueTimer(int8_t t);
void delTimer(int8_t t);

uint16_t ticksLeftTimer(int8_t t);
uint16_t secondsLeftTimer(int8_t t);

// call this in your mainloop
void checkTimer();

#endif
