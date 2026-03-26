# PIC18F-RTC-Keypad
## Real time clock based on the PIC18F57Q43 MCU, that uses a standard 16x2 LCD to display the time, and accepts inputs from a keypad to set the time.

## Features
- Displays time in HH:MM:SS AM/PM format (12 hour) on a 16x2 LCD

- Set time interactively via a matrix keypad

- Input validation for all fields (hours, minutes, seconds, AM/PM)

- Uses the PICs onboard XTAL, utilizing interrupts to keep track of time and inputs.

## Hardware - PIC18F57Q43 MCU
- Internal LED (for second tick) - Port RF3

- Debug LEDs - Port C

- 16x2 LCD - 4-bit wiring on Port D

- 4x4 Matrix keypad - Port A + internal weak pullups enabled

### Keypad layout
| 1 | 2 | 3 | A |
|---|---|---|---|
| 4 | 5 | 6 | B |
| 7 | 8 | 9 | C |
| * | 0 | # | D |
