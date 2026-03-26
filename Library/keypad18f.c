//keypad18f.c

//on PORTA Keypad Board

//code modified from northMicro keypad.c

#include <xc.h>

#include "keypad18f.h"

#include "PIC18FConfig.h"

//The following code requires the port to be digital and weak pull ups to be set in the main code

unsigned char keypad(void)

{
TRISA = 0xF0;
 
// upper nibble inputs lower outputs for keypad

NOP();

NOP();

NOP();

NOP();

LATA0 = 0;
 
// 0 rotated through lower nibble

LATA1 = 1;
 
// upper nibble set to inputs with weak pullups

LATA2 = 1;
 
// check which is lo

LATA3 = 1;

NOP();

NOP();

NOP();

NOP();

if(!RA4)return('1');

if(!RA5)return('4');

if(!RA6)return('7');

if(!RA7)return('*');

LATA0 = 1;

LATA1 = 0;

NOP();

NOP();

NOP();

NOP();

if(!RA4)return('2');

if(!RA5)return('5');

if(!RA6)return('8');

if(!RA7)return('0');

LATA1 = 1;

LATA2 = 0;

NOP();

NOP();

NOP();

NOP();

if(!RA4)return('3');

if(!RA5)return('6');

if(!RA6)return('9');

if(!RA7)return('#');

LATA2 = 1;

LATA3 = 0;

NOP();

NOP();

NOP();

NOP();

if(!RA4)return('A');

if(!RA5)return('B');

if(!RA6)return('C');

if(!RA7)return('D');

return(0);
 
// no valid key pressed

}