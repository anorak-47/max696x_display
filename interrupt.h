
#ifndef INTERRUPT_H_
#define INTERRUPT_H_

void init_interrupts(void);
void release_interrupts(void);
void release_interrupt_external_shutdown(void);

void handleInterrupts(void);

#endif /* INTERRUPT_H_ */
