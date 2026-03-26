/*
 * File:   lcd18f.c
 * Author: Elijah Burtner
 *
 * This program holds the functions for a 16x2 LCD screen using the PIC18F in 4 bit mode.
 * 
 * Created on February 12, 2026
 * 
 * PINOUT: Standard 14 pin LCD connector used.
 * LATD bits 0-3 -> LCD data bits 4-7 (high nibble)
 * LATD bit 4 -> LCD RS input (register select)
 * LATD bit 5 -> R/!W
 * LATD bit 6 -> LCD EN bit (enable)
 * LATD bit 7 -> LCD Backlight
 * 
 */

#include "PIC18FConfig.h" //Include C header file which contains the PIC configuration bits, config and XTAL Freq.

//LCD Define Bits
#define lcdRS LATD4     //Control/Data mode
#define lcdRW LATD5     //Read/!Write
#define lcdEN LATD6     //Enable
#define lcdBL LATD7     //Backlight

//Define LCD Func
#define lcdStrobe lcdEN = 1; __delay_us(1); lcdEN = 0
#define lcdPortEN TRISD = 0; __delay_us(1); lcdRW = 0 //Enables the port and write mode

void lcdWrite(unsigned char c) {
    LATD = (LATD & 0xF0) | (c >> 4);   //Send M.S. nibble
    lcdStrobe;  //Latch data
    LATD = (LATD & 0xF0) | (c & 0x0F); //Send L.S. nibble
    lcdStrobe;  //Latch data
    __delay_us(80);
}

//Init the LCD
void lcdInit(void) {
    lcdEN = 0;      //Setup LCD enable for strobe
    lcdPortEN;      //Enable port and write mode
    lcdRS = 0;      //CLR to write control bytes
    __delay_ms(30); //Power on delay
    
    LATD = (LATD & 0xF0) | 0x03;   //3 to lower nibble, don't change upper nibble
    lcdStrobe;          //Latch data
    __delay_ms(30);     //Wait
    
    lcdStrobe;          //3 to lower nibble
    __delay_ms(5);
    
    lcdStrobe;          //3 to lower nibble
    __delay_ms(5);
    
    LATD = (LATD & 0xF0) | 0x02;   //3 to lower nibble, don't change upper nibble
    lcdStrobe;         //Latch data
    __delay_ms(5);     //Wait
    
    lcdWrite(0x28);    // 1/16 duty, 5x8 font
    lcdWrite(0x0c);    // Display on, cursor off, blink off
    lcdWrite(0x01);    // Clear Display
    __delay_ms(2);
    lcdWrite(0x06);    // Entry mode set, inc mode, entire shift off
}

//Go to the specified position
void lcdGoto(unsigned char pos) {
    lcdPortEN;
    lcdRS = 0;
    lcdWrite(0x80+pos); //Num is goto command
    
}

//Write one character to the LCD
void lcdPutCh(unsigned char c) {
    lcdPortEN;
    lcdRS = 1;  //Write characters
    lcdWrite(c);
    
}

//Control Backlight
void lcdBl(unsigned char status) {
    if(status) {
        lcdBL = 1; //Backlight on
    }
    
    else {
        lcdBL = 0; //Backlight off
    }
}

//Clear the LCD
void lcdClear(void) {
    lcdPortEN;
    lcdRS = 0;
    lcdWrite(0x01);
    __delay_ms(2);
    
}

//Write a string of characters to the LCD
void lcdPuts(unsigned char * s) {
    lcdPortEN;
    lcdRS = 1; //Write characters
    while(*s) {
        lcdWrite(*s++);
    }
}