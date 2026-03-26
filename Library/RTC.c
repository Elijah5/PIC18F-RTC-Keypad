/* 
 * File:   RTC.c
 * Author: Elijah Burtner
 *
 * Created on March 12, 2026
 *
 * SOURCE file to handle the RTC functions, including incrementing / setting the time (with a keypad) and displaying it on the LCD.
 */

//Include Header Files
#include "RTC.h"
#include "lcd18f.h"
#include "PIC18FConfig.h"
#include "keypad18f.h"
#include "interrupt.h"
#include <pic18.h>
#include <pic18f57q43.h>
#include <xc.h>

//Define LCD Positions for setting time
#define hourTens   0x40
#define hourOnes   0x41
#define minTens    0x43
#define minOnes    0x44
#define secTens    0x46
#define secOnes    0x47

//* ------------- STATIC FUNCTION PROTOTYPES -------------*
static unsigned char set2DigitVal(unsigned char tensPos, unsigned char onesPos);
static unsigned char scanKey(void);

//* ------------- SCAN KEYPAD WITH DEBOUNCE / RELEASE FUNCTION -------------*
//Scan the keypad and return the key value this keyscan is only used in setTime, so it can be static
static unsigned char scanKey(void) {
    __delay_ms(5);                          //Delay to debounce the key press
    unsigned char scannedKey = keypad();    //Scan the keypad and store the result
    while (keypad() != 0);                  //Wait for the user to release the key
    return scannedKey;                      //Return the scanned key value
} //scanKey function

//* ------------- INCREMENT TIME & HANDLE ROLLOVER FUNCTION -------------*
//Call to increment the time by one second, and handle rollovers (Every ms when tickcount > 99)
void incTime(unsigned char *sec, unsigned char *min, unsigned char *hour, unsigned char *AMnPM) {
    ++*sec;                         //Increment seconds every time func is called in main

    if (*sec > 59) {
        ++*min;                     //Increment minutes when seconds rollover
        *sec = 0;                   //Reset seconds
        
        if (*min > 59) {
            ++*hour;                //Increment hours when minutes rollover
            *min = 0;               //Reset minutes
            
            //Toggle AM/PM when hours rollover from 11 to 12 & the hour rollover is not being cascaded 
            if (*hour == 12) {
                *AMnPM = !*AMnPM;   //Toggle AM/PM when hours rollover
            } //if hour > 11
           
            if (*hour > 12) {
                *hour = 1;          //Reset hours to 1 after 12
            } //if hour > 12

        } //if min
    } //if sec
} //incTime function

//* ------------- DISPLAY TIME FUNCTION -------------*
void displayTime(unsigned char timeBuffer[12], unsigned char *sec, unsigned char *min, unsigned char *hour, unsigned char *AMnPM) {
    //Splice Digits and convert to ASCII, then store in buffer
    timeBuffer[0] = (*hour / 10) + '0';     //Hours tens digit
    timeBuffer[1] = (*hour % 10) + '0';     //Hours ones digit
    
    timeBuffer[3] = (*min / 10) + '0';      //Minutes tens digit
    timeBuffer[4] = (*min % 10) + '0';      //Minutes ones digit
    
    timeBuffer[6] = (*sec / 10) + '0';      //Seconds tens digit
    timeBuffer[7] = (*sec % 10) + '0';      //Seconds ones digit
    
    timeBuffer[9] = *AMnPM ? 'A' : 'P';     //Display A for AM, P for PM

    //Display the Buffer on the LCD
    lcdGoto(0x00);
    lcdPuts(timeBuffer);
} //displayTime function

//* ------------- SET TIME MAIN FUNCTION -------------*
void setTime(unsigned char *hour, unsigned char *min, unsigned char *sec, unsigned char *AMnPM) {
    unsigned char newHour = 0;                   //Variable to store the new hour being set
    unsigned char newMin = 0;                    //Variable to store the new minute being set
    unsigned char newSec = 0;                    //Variable to store the new second being set
    unsigned char key = 0;                       //Variable to store keypad input
    unsigned char waitingFor1sDigit = 0;         //Flag to indicate we're waiting for 1s digit after pressing tens digit
    unsigned char tenthsDigit = 0;               //Store which tens digit was entered (0 or 1) to validate 1s digit

    unsigned char AMformat[6] = "A: AM";             //Guide the user on time format / inputs (AM)
    unsigned char timeFormat[17] = "hh:mm:ss B: PM *"; //Guide the user on time format / inputs
    
    lcdGoto(0x09);                          //Move cursor to first line, 10th character
    lcdPuts(AMformat);                        //Display the AM format to guide the user
    lcdGoto(0x40);                          //Move cursor to second line
    lcdPuts(timeFormat);                      //Display the time format to guide the user
    
  //*------------- SET HOURS -------------*
    unsigned char setHours = 1;                 //Set hours flag/loop control
    
    while (setHours) {                          //Loop until hours are entered
        key = scanKey();                        //Scan the keypad for input
        
        if (key == 'D') {                       //If D is pressed, discard changes and exit
            return;
        } //if key == 'D'
        
        else if (waitingFor1sDigit) {                    //Waiting for 1s digit after setting tens digit
            if (tenthsDigit == 1) {                      //If tens digit was 1, accept 0, 1, 2
                if (key >= '0' && key <= '2') {          //Handle valid 1s digits (0, 1, 2)
                    lcdGoto(hourOnes);               //Move cursor to hour ones position on LCD
                    lcdPutCh(key);                       //Display the 1s digit
                    newHour += (key - '0');              //Add the ones digit to the hour
                    waitingFor1sDigit = 0;               //Exit 1s digit input mode
                    setHours = 0;                        //Exit time set mode for hours
                } //if key is 0, 1, or 2
            } //if tenthsDigit 
            
            else if (tenthsDigit == 0) {                //If tens digit was 0, accept 1-9 (not 0)
                if (key >= '1' && key <= '9') {         //Handle valid 1s digits (1-9)
                    lcdGoto(hourOnes);              //Move cursor to ones digit location
                    lcdPutCh(key);                      //Display the 1s digit
                    newHour += (key - '0');             //Add the ones digit to the hour
                    waitingFor1sDigit = 0;              //Exit 1s digit input mode
                    setHours = 0;                       //Exit time set mode for hours
                } //if key is 1-9
            } //else if tenthsDigit == 0
        } //if waitingFor1sDigit
        
        else if (key == '0' || key == '1') {            //Handle cases where user entered 1 or 0 in the 10s digit directly (was not skipped by using >=2)
            lcdGoto(hourTens);                      //Move cursor to second line (hour tens digit position)
            lcdPutCh(key);                              //Display the tens digit
            newHour = (key == '0') ? 0 : 10;            //Set newHour to 0 (0) or 10 (1)            
            waitingFor1sDigit = 1;                      //Set flag to wait for 1s digit
            tenthsDigit = (key == '0') ? 0 : 1;         //Store tens digit (0  or 1) for validation of 1s digit
        } //if key '0' || '1'

        else if (key >= '2' && key <= '9') {            //If user enters 2-9, use as 1s digit only
            lcdGoto(hourTens);                      //Move cursor to second line (hour tens digit position)
            lcdPutCh('0');                              //Display 0 for the tens digit
            lcdGoto(hourOnes);                      //Move cursor to ones digit location
            lcdPutCh(key);                              //Display the key for ones digit
            newHour = (key - '0');                      //Set hour to the single digit entered
            setHours = 0;                               //Exit time set mode for hours
        } //else if key is 2-9        
    } //while setHours
    
    //*------------- SET MINUTES & SECONDS -------------*
    //Call function to set the seconds. Returns 0xFF if user cancels with 'D'    
    newMin = set2DigitVal(minTens, minOnes);
    if (newMin == 0xFF) return;

    //Call function to set the seconds. Returns 0xFF if user cancels with 'D'
    newSec = set2DigitVal(secTens, secOnes);
    if (newSec == 0xFF) return;
    
  //*------------- SELECT AM/PM & PUSH VALUES -------------*
    unsigned char selectAMPM = 1;               //Flag to indicate we are waiting for AM/PM selection
    
    while (selectAMPM) {                        //Loop until the user selects AM or PM
        key = scanKey();                        //Scan the keypad for input
        
        if (key == 'D') {                       //If D is pressed, discard all changes and exit
            return;                             //Exit without storing any values
        } //if key == 'D'
        
        else if (key == 'A' || key =='B') {     //If A is pressed, set AM
            *AMnPM = (key == 'A') ? 1 : 0;      //Set AM (1) or PM (0)
            *hour = newHour;                    //Store the new hour
            *min = newMin;                      //Store the new minute
            *sec = newSec;                      //Store the new second
            selectAMPM = 0;                     //Exit AM/PM selection mode
        } //if key == 'A' or 'B'
    } //while selectAMPM
} //setTime Function

    //*------------- SET MINUTES / HOURS HELPER FUNCTION -------------*
    static unsigned char set2DigitVal(unsigned char tensPos, unsigned char onesPos) {
    
        unsigned char waitingFor1sDigit = 0;       //Reset 1s digit flag
        unsigned char tenthsDigit = 0;             //Reset tens digit tracker
        unsigned char key = 0;                     //Variable to store keypad input
        unsigned char newVal = 0;                 //Variable to store the new value being setS

        while (1) {                                 //Loop until the user finishes entering minutes or seconds
            key = scanKey();                        //Scan the keypad for input
        
            if (key == 'D') {                       //If D is pressed, discard changes and exit
                return 0xFF;                        //Return cancellation flag
            } //if key == 'D'
            
            if (waitingFor1sDigit) {               //Waiting for 1s digit after setting tens digit
                if (key >= '0' && key <= '9') {
                    lcdGoto(onesPos);          //Move cursor to ones digit location (Function call specifies hours or minutes)
                    lcdPutCh(key);                  //Display the 1s digit
                    newVal += (key - '0');          //Add the ones digit to the newVal
                    return newVal;                  //Return the new value (both digits entered)
                } //if waitingFor1sDigit
            } //if waitingFor1sDigit

            else if (key >= '0' && key <= '5') {    //Valid tens digit for min or hrs is 0-5
                lcdGoto(tensPos);               //Move cursor to tens digit location for minutes
                lcdPutCh(key);                      //Display tens digit
                newVal = (key - '0') * 10;          //Set newVal tens digit
                waitingFor1sDigit = 1;              //Set flag to wait for ones digit on next scan
            } //else if key == '0'
        } //while setMinutes
} //set2DigitVal helper function