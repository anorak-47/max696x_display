#ifndef __POWER_H_
#define __POWER_H_

void enter_idle_sleep(void);
void enter_power_down_sleep(void);
void enter_power_down_on_repearted_watchdog_reset(void);
void enter_power_down_sleep_forever(void);

#endif
