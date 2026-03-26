/* 
 * File:   interrupt.c
 * Author: Elijah Burtner
 *
 * Created on March 5, 2026, 2:27 PM
 */

#include <xc.h>
#include "PIC18FConfig.h"
#include "interrupt.h"

void timerInit(void) { //Init timer0 to produce a 10ms interrupt
    
    T0CON0 = 0b10001001;   // tmr0 enable, 8 bit, 1:10 Postscaler
    T0CON1 = 0b10100001;   // 500 kHz, sync, 1:2 prescaler
    TMR0H = 249;           // TMR0H 0 - 249 count for a period of 250
    GIEH = 1;              // Enable global interrupts
    TMR0IE = 1;            // TMR0 interrupt enable
    TMR0IF = 0;            // Clear TMR0 interrupt flag
    TMR0IP = 1;            // TMR0 high priority interrupt
} //Timer init

void __interrupt(irq(TMR0),high_priority) tickINT(void)
{
	TMR0IF = 0;
    tickCount++;
    intervalTrigger++; //Increment the interval trigger counter every ms
	return;
}

void __interrupt(irq(default),high_priority) defaultINT(void)
{
return;
}