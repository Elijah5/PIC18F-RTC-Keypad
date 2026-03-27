/* 
 * File:   main.c
 * Author: Elijah Burtner
 *
 * Created on March 19, 2026
 *
 * This program uses the PIC18Fs XTAL and interrupts to create an RTC that displays the time on a 16x2 LCD, and allows the time to be updated with a keypad. 
 *
 */

//Include Header Files
#include <pic18f57q43.h>
#include <xc.h>
#include "Library/PIC18FConfig.h" 			//Config bits and XTAL Freq
#include "Library/lcd18f.h" 				//LCD Lib
#include "Library/interrupt.h" 				//Interrupt Lib
#include "Library/RTC.h" 					//RTC Lib
#include "Library/keypad18f.h" 				//Keypad Lib

int main(void) {

	//Setup Ports
    ANSELD = 0x00;  		//Set PORTD as digital
    LATD = 0x00;  			//Clear PORTD Latch
    TRISD = 0x00;   		//Set PORTD
    ANSELA = 0x00; 			//Set PORTA as digital
    WPUA = 0xF0; 			//Turn on weak pullups for PORTA (keypad)
	PORTF = 0x00; 			//Clear Port F
    TRISF = 0x00; 			//Set Port F to output
	PORTC = 0x00; 			//Clear Port C (LEDs)
    TRISC = 0x00; 			//Set Port C to output

	unsigned char key = 0;	//Init var to store keypad input

	//Time Vars (Set initial time)
	unsigned char hour = 12;
	unsigned char min = 0;
	unsigned char sec = 0;
	unsigned char AMnPM = 1; //1 for AM, 0 for PM

	//Format String
	unsigned char timeBuffer[12] = "HH:MM:SS XM";	//Time is stored into buffer before being displayed on the LCD (LCD put string)

	//lcd18f.c Function Calls
    lcdInit(); 				 //Init LCD
    lcdBl(1); 		 //Turn on LCD backlight
    lcdClear(); 			//Clear LCD
    
    timerInit(); 			//Call function  from interrupt.c to init the timer for 10ms   

    while(1) { //Main Loop
	    if (tickCount >= 50) { //Toggle RF3 (internal LED) every 500ms)
		RF3 = 0; //Internal LED off
	    } //if tickCount >= 50
	    
		if (tickCount > 99) {
			tickCount = tickCount - 100; //Subtract 100 to reset ticks without losing time
			RF3 = 1; 					//Internal LED on
			
			//Call function to increment the time by one second, and handle rollovers (Every ms when tickcount > 99)
			incTime(&sec, &min, &hour, &AMnPM); 
			
			//Call function to display the time on the LCD. Handles formatting and stores in timeBuffer
			displayTime(timeBuffer, &sec, &min, &hour, &AMnPM); 
		} //if tickCount > 99
		
		if (intervalTrigger >= 3) { //Scan keypad every 30ms
			key = keypad(); 		//Scan the keypad and store the result in key
			intervalTrigger = 0; 	//Reset the interval trigger
		} //else if intervalTrigger
		
		if (key == 'C') { 			//If key C was pressed, enter time set mode
			GIEH = 0; 				//Disable interrupts while in time set mode
			LATC = 0xFF; 			//Turn on LEDs (Port C) to signify time set mode
			
			//Put user in time set mode (call to function in RTC.c)
			setTime(&hour, &min, &sec, &AMnPM); 
			
			GIEH = 1; 				//Re-enable interrupts after time set mode
			lcdClear(); 			//Clear LCD after exiting time set mode
			key = 0; 				//Reset key to prevent repeated triggering
		} //if key
		
		LATC = tickCount; 			//Display tickCount on Port C for testing 
    	
	} //while 1 (main loop)
    return 0;
} //main
