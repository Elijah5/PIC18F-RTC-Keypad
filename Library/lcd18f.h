/* 
 * File:   lcd18f.h
 * Author: C0506942
 *
 * Created on February 12, 2026, 5:25 PM
 */

#ifndef LCD18F_H
#define	LCD18F_H

#ifdef	__cplusplus
extern "C" {
#endif


// Turn on or off backlight
    void lcdBl(unsigned char status);
    
// Write a byte to the LCD in 4 bit mode
    void lcdWrite(unsigned char);
    
// Clear and home the LCD
    void lcdClear(void);
    
// Write a sting of characters to the LCD
    void lcdPuts(unsigned char * s);
    
 // Go to the specified position
    void lcdGoto(unsigned char pos);
    
// Init the LCD - Call before anything else
    void lcdInit(void);
    
// Write one character to the LCD
    void lcdPutCh(unsigned char);



#ifdef	__cplusplus
}
#endif

#endif	/* LCD18F_H */

