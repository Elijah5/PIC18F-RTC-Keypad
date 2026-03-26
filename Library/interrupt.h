/* 
 * File:   interrupt.h
 * Author: Elijah Burtner
 *
 * Created on March 5, 2026, 3:31 PM
 */

#ifndef INTERRUPT_H
#define	INTERRUPT_H

#ifdef	__cplusplus
extern "C" {
#endif

unsigned char tickCount; //Keep track of # of timer interrupts (ms)
unsigned char intervalTrigger; //Counter for non-blocking delays (ms)
void timerInit(void);
void __interrupt(irq(TMR0),high_priority) tickINT(void);
void __interrupt(irq(default),high_priority) defaultINT(void);


#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPT_H */

