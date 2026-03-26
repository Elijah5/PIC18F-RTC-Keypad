/* 
 * File:   RTC.c
 * Author: Elijah Burtner
 *
 * Created on March 12, 2026
 *
 * HEADER file to handle the RTC functions, including incrementing time and displaying it on the LCD.
 */

#ifndef RTC_H
#define	RTC_H

#ifdef	__cplusplus
extern "C" {
#endif

void displayTime(unsigned char timeBuffer[12], unsigned char *sec, unsigned char *min, unsigned char *hour, unsigned char *AMnPM); //Call to display the time on the LCD

void incTime(unsigned char *sec, unsigned char *min, unsigned char *hour, unsigned char *AMnPM); //Call to increment the time by one second, and handle rollovers (Every ms when tickcount > 9)

void setTime(unsigned char *hour, unsigned char *min, unsigned char *sec, unsigned char *AMnPM); //Call to set hours, minutes, and seconds via keypad input (D to cancel, A/B for AM/PM)

#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */