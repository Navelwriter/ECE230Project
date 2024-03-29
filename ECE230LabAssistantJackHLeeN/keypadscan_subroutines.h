//File Name: keypadscan_subroutines.h
//Author: Jianjian Song
//ECE230 Winter 2021-2022
//Date: February 8, 2022

#ifndef KEYPADSCAN_SUBROUTINES_H_
#define KEYPADSCAN_SUBROUTINES_H_

#endif /* KEYPADSCAN_SUBROUTINES_H_ */

//Purpose: Scan a 4x4 keypad to provide key numbers from 1 to 16
//         if a corresponding key is pressed
//         key_number is 0 if no key is pressed
/*
 * MSP432P4111 GPIO - Port 4 Input Interrupt on Pins 4, 5, 6, 7
 *
 *                MSP432P4111
 *             ------------------
 *         /|\|       input P6.0|---> button1
 *          | |       input P6.1|---> button2
 *          --|RST    output P4.0|---> key0
 *            |       output P4.1|---> key1
 *            |       output P4.2|---> key2
 *            |       output P4.3|---> key3
 *            | pullup input P4.4|---> key4
 *            | pullup input P4.5|---> key5
 *            | pullup input P4.6|---> key6
 *            | pullup input P4.7|---> key7
 *            |                  |
 *            |                  |
 ******************************************************************************/
/* DriverLib Includes */
#include <msp.h>
#define KeypadPort P4
#define buttonPort P6
#define button1 BIT0
#define button2 BIT1

#define KeypadInputPins (BIT7 | BIT6 | BIT5 | BIT4)
#define KeypadOutputPins (BIT3 | BIT2 | BIT1 | BIT0)

void debounce(uint16_t DEBOUNCETIME);
void keypadconfiguration(void);
void buttonConfig(void);
char FindKey(void);
void PORT4_IRQHandler(void);
char reassignKeys(char input);


