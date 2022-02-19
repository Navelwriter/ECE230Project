
 /*                MSP432P4111
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
 *            |              P5.0|---> wavegen
 *            |                  |
 ******************************************************************************/
#include "msp.h"
#include "keypadscan_subroutines.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "csHFXT.h"
#include "csLFXT.h"
#include "hex2dec.h"
#include "gen_routines.h"
#include "wavegen.h"

/**
 * main.c
 */
enum Status {
	NO, YES
};
typedef enum _SwitchState {
	idle, convert, wavegen
} SwitchState;
SwitchState modeSelect = idle;

enum {
	none = 1, decSize = 6, binSize = 16, hexSize = 4, pwmSize = 3
} maxSize = idle; //Value - 1 is the number of bits allowed to be inputted

char menu[] = { "\r\n Press 1 for decimal, 2 for binary, 3 for hex, 4 for pwm generator" };
extern char NewKeyPressed;
extern char buttonPress;
extern char FoundKey;
unsigned int current_size = 0;

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
	uint32_t decimal = 0; //buffer is of size 0 from size, decimal is stored
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
		if (NewKeyPressed && modeSelect != idle) {
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
		if (modeSelect == convert) {
			if (buttonPress) {
				buttonPress = NO;
				if (maxSize == decSize) {
					SendCharArray("\r\n decimal: "); //Print decimal
					printOutBuffer(&DataBuffer, current_size);
					decimal = convertDecimal(&DataBuffer[current_size], current_size);
					SendCharArray("\r\n\r\n Input 6 digit decimal characters ");
				} else if (maxSize == binSize) {
					decimal = convertBinary(&DataBuffer, current_size);
					SendCharArray("\r\n\r\n Input 16 digit binary characters ");
				} else if (maxSize == hexSize){
					decimal == convertHex(&DataBuffer, current_size);
					SendCharArray("\r\n\r\n Input 4 digit hex characters ");
				}
				current_size = 0; //buffer is of size 0 from size
			}
		}
		if(modeSelect == wavegen){
			if(NewKeyPressed){
				NewKeyPressed = NO;
				input = FoundKey;
				fprintf(decimal,"%d",input);
			}
			changeDutyCycle(decimal);
		}

	}
}

SwitchState CheckMode(char recieved) { //Check for each valid letter and assign actions to each
	if (recieved != '\x0') {
		if (recieved == '1') {
			modeSelect = convert;
			maxSize = decSize;
			SendCharArray("\r\n\r\n Input 6 digit decimal characters ");

		} else if (recieved == '2') {
			modeSelect = convert;
			maxSize = binSize;
			SendCharArray("\r\n\r\n Input 16 digit binary characters ");

		} else if (recieved == '3') {
			modeSelect = convert;
			maxSize = hexSize;
			SendCharArray("\r\n\r\n Input 4 digit hex characters ");

		} else if (recieved == '4') {
			modeSelect = wavegen;
			maxSize = pwmSize;
			configHFXT;
			wavegenConfig;
			SendCharArray("\r\n\r\n Input percent duty time in up to 3 digit decimal characters ");

		} else {
			modeSelect = idle;
			maxSize = none;
			SendCharArray("\r\n\r\n not functional ");

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
	if (buttonPort->IFG & button2) {
		//check for button 2
		if(modeSelect == wavegen){
			toggleWavegen;
		}
		modeSelect = idle;
		SendCharArray(menu);
		current_size = 0; //buffer is of size 0 from size

	}
	status = buttonPort->IFG;
	buttonPort->IFG &= ~status; //clear interrupt

} // end PORT6_IRQHandler(void)
