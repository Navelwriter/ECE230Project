#include "msp.h"
#include "keypadscan_subroutines.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "csHFXT.h"
#include "csLFXT.h"
#include "hex2dec.h"
#include "gen_routines.h"
/**
 * main.c
 */
enum Status {
	NO, YES
};
typedef enum _SwitchState {
	idle, convert, pwm
} SwitchState;
SwitchState modeSelect = idle;

enum {
	none = 1, decSize = 6, binSize = 16, hexSize = 4
} maxSize = idle; //Value - 1 is the number of bits allowed to be inputted

char menu[] = { "\r\n Press 1 for decimal, 2 for binary(not funct), 3 for hex" };
extern char NewKeyPressed;
extern char buttonPress;
extern char FoundKey;

//Function Prototypes
void SendChar(char letter);
void ConfigureUART(void);
char GetChar(void);
void ConfigureGY521MPU6050(void);
void SendCharArray(char *Buffer);
void delay(unsigned int);
void PORT6_IRQHandler(void);
SwitchState CheckMode(char recieved);

void main(void) {
	char input;
	char DataBuffer[20];
	uint32_t current_size, decimal = 0; //buffer is of size 0 from size, decimal is stored
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	ConfigureCS();
	ConfigureUART();
	keypadconfiguration();
	buttonConfig();
	__enable_irq();
	NVIC->ISER[0] = (1 << EUSCIB0_IRQn);
	KeypadPort->OUT = (KeypadPort->OUT & ~KeypadOutputPins) //clear interrupt flags
	| (0b11110000 & KeypadOutputPins);
	P4->IFG &= ~P4->IFG;
	SendCharArray(menu);

	while (1) {
		while (modeSelect == idle) {
			if (NewKeyPressed) {
				input = FoundKey;
				CheckMode(input);
			}
		}
		if (modeSelect == convert) {
			if (buttonPress) {
				buttonPress = NO;
				if (maxSize == decSize) {
					decimal = convertDecimal(&DataBuffer, current_size);
				} else if (maxSize == hexSize) {
					decimal = convertHex(&DataBuffer, current_size);
				} else {
					decimal == convertHex(&DataBuffer, current_size);
				}
				current_size = 0; //buffer is of size 0 from size
			}
			if (NewKeyPressed) {
				input = FoundKey;
				NewKeyPressed = NO;
				if (input != '\x0') {
					if (current_size == 0) {
						SendCharArray("\r\n Key Found: ");
					}
					if (current_size < maxSize) { //limit hex input to 4 characters
						DataBuffer[current_size] = input;
						SendChar(DataBuffer[current_size]);
						current_size++;
					}
				}
			}
		}
	}
}

SwitchState CheckMode(char recieved) { //Check for each valid letter and assign actions to each
	if (recieved != '\x0') {
		if (recieved == '1') {
			modeSelect = convert;
			maxSize = decSize;
			SendCharArray("\r\n Input 6 digit decimal characters ");

		} else if (recieved == '2') {
			modeSelect = convert;
			maxSize = binSize;
			SendCharArray("\r\n Input 16 digit binary characters ");
			SendCharArray("\r\n not functional ");

		} else if (recieved == '3') {
			modeSelect = convert;
			maxSize = hexSize;
			SendCharArray("\r\n Input 4 digit hex characters ");

		} else if (recieved == '4') {
			modeSelect = idle;
			maxSize = none;
			SendCharArray("\r\n not functional ");

		} else {
			modeSelect = idle;
			maxSize = none;
			SendCharArray("\r\n not functional ");

		}
		NewKeyPressed = NO;
	}
	return maxSize;
} //End CheckMode

void PORT6_IRQHandler(void) {
	uint32_t status;
	if (buttonPort->IFG & button1) { //check for button 1
		if (modeSelect != idle) {
			buttonPress = YES;
		}

	}
	if (buttonPort->IFG & button2) { //check for button 2
		modeSelect = idle;
		SendCharArray(menu);
	}
	status = buttonPort->IFG;
	buttonPort->IFG &= ~status; //clear interrupt

} // end PORT6_IRQHandler(void)
