//File Name: keypadscan_subroutines.c
//Author: Noah Lee
//ECE230 Winter 2021-2022
//Date: February 13, 2022
//Purpose: Scan a 4x4 keypad to provide key numbers from 1 to 16
//		   and assign numbers to mapped assignments
//         if a corresponding key is pressed
//         key_number is 0 if no key is pressed
//		   also enables an interrupt button in pin 6.0
/*
 * MSP432P111 GPIO -
 * Port 4 Input Interrupt on input pins 4, 5, 6, 7
 *
 *                MSP432P4111
 *             ------------------
 *         /|\|        input P6.0|---> button1
 *          | |        input P6.1|---> button2
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
/* Standard Includes */
#include <msp.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <keypadscan_subroutines.h>
typedef enum _SwitchState {
	idle, hex, pwm
} SwitchState;
extern SwitchState modeSelect;

void keypadconfiguration(void) {
//Configuring P4.3-0 as output
	KeypadPort->DIR |= KeypadOutputPins;
	KeypadPort->SEL0 &= ~KeypadOutputPins;
	KeypadPort->SEL1 &= ~KeypadOutputPins;

//P4.7-4 as pulled-up input to interface a keypad
//Pins 7 to 4 cause input change interrupt
	KeypadPort->DIR &= ~KeypadInputPins;
	KeypadPort->SEL0 &= ~KeypadInputPins;
	KeypadPort->SEL1 &= ~KeypadInputPins;
//Pullup resistors on input pins
	KeypadPort->OUT |= KeypadInputPins;
	KeypadPort->REN |= KeypadInputPins;

//select interrupt edge on high to low transition on input pins
	KeypadPort->IES |= KeypadInputPins; //bit = 1 in PxIES register
//enable pin interrupts
	KeypadPort->IE |= KeypadInputPins; //bit = 1 to enable pin in PxIE register
//clear interrupt flags
	KeypadPort->IFG &= ~KeypadInputPins;
//enable PORT interrupt
	NVIC->ISER[1] |= (1) << (PORT4_IRQn - 32);

}

void buttonConfig(void){
	//set buttonport to be intput and pullup
	//BUTTON 1
	buttonPort ->DIR &= ~button1;
	buttonPort ->REN |=  button1;
	buttonPort ->OUT |=  button1;

	buttonPort ->IE  |=  button1; //Enable interrupt
	buttonPort ->IES |=  button1; //Enable interrupt edge on high to low
	buttonPort ->IFG &= ~button1;//clear int flags
	//enable PORT interrupt
	NVIC->ISER[1] |= (1) << (PORT6_IRQn - 32);

	//BUTTON 2
	buttonPort ->DIR &= ~button2;
	buttonPort ->REN |=  button2;
	buttonPort ->OUT |=  button2;

	buttonPort ->IE  |=  button2; //Enable interrupt
	buttonPort ->IES |=  button2; //Enable interrupt edge on high to low
	buttonPort ->IFG &= ~button2;//clear int flags
	//enable PORT interrupt
	NVIC->ISER[2] |= (PORT6_IRQn - 32);
}

const char KeyPatterns[16] = {
		0b01110111, 0b01111011, 0b01111101, 0b01111110,
		0b10110111, 0b10111011, 0b10111101, 0b10111110,
		0b11010111, 0b11011011, 0b11011101, 0b11011110,
		0b11100111, 0b11101011, 0b11101101, 0b11101110 };
const char KeyAssignments[16] = {
		'1', '2', '3', 'A',
		'4', '5', '6', 'B',
		'7', '8', '9', 'C',
		'E', '0', 'F', 'D'
};

enum Status {
	NO, YES
};
char NewKeyPressed = NO;
char buttonPress = NO;
char FoundKey;
//return key numbers from 1 to 16 if a key is found to be pressed
//return 0 if no key is pressed
void debounce(uint16_t DEBOUNCETIME){
	uint16_t delayloop;
	for (delayloop = 0; delayloop < DEBOUNCETIME; delayloop++); //debounce the key
}

char FindKey(void) {
	#define DEBOUNCETIME 500
	char i;
	char KeyNumber;
	char PortValue;
	uint16_t  delayloop;
	for (delayloop = 0; delayloop < DEBOUNCETIME; delayloop++); //debounce the key
	for (i = 0; i <= 15; i++) { //output key pattern
		KeypadPort->OUT = (KeypadPort->OUT & ~KeypadOutputPins)
				| (KeyPatterns[i] & KeypadOutputPins);
		PortValue = KeypadPort->IN;
		if ((PortValue & KeypadInputPins) == (KeyPatterns[i] & KeypadInputPins)) {   //check key press
			for (delayloop = 0; delayloop < DEBOUNCETIME; delayloop++); //debounce the key
			if ((PortValue & KeypadInputPins) == (KeyPatterns[i] & KeypadInputPins)) {   //still pressed
				KeyNumber = i + 1;
				while ((PortValue & KeypadInputPins)
						== (KeyPatterns[i] & KeypadInputPins))
					PortValue = KeypadPort->IN; //wait for release
				break;
			}
			for (delayloop = 0; delayloop < DEBOUNCETIME; delayloop++); //debounce the key
		}
	}
	return KeyNumber;
} //end FindKey()

char reassignKeys(char input){
	char test = KeyAssignments[input - 1];
	return test;
} //end reassignKeys()

/* GPIO ISR */

void PORT4_IRQHandler(void) {
	uint32_t status;
	char test;

	status = KeypadPort->IFG;
	KeypadPort->IFG &= ~status;

	FoundKey = FindKey();
	test =FoundKey;

//set up for next key interrupt. Set all output pins to 0
//do not change input values
	KeypadPort->OUT = (KeypadPort->OUT & ~KeypadOutputPins)
			| (0b11110000 & KeypadOutputPins);
	if (FoundKey != 0) {
		NewKeyPressed = YES;
		FoundKey = KeyAssignments[FoundKey - 1];
	} else
		NewKeyPressed = NO;

} // end PORT4_IRQHandler(void)


