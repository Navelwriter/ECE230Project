#include "msp.h"
#include "keypadscan_subroutines.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "csHFXT.h"
#include "csLFXT.h"
#include "hex2dec.h"
/**
 * main.c
 */

//Function Prototypes
void SendChar(char letter);
void ConfigureUART(void);
char GetChar(void);
void ConfigureGY521MPU6050(void);
void SendCharArray(char *Buffer);
void SendPrompt(void);
void delay(unsigned int);
void printOutBuffer(char Buffer[], uint32_t size);
uint32_t convertHex(char *DataBuffer,uint32_t current_size);

char CharBuffer[25];
uint32_t maxSize = 3; //Global int to store how many chars to store for each data type, hex,bin,dec. It's 4 chars for hex

enum Status {
	NO, YES
};
typedef enum _SwitchState {
	idle, hex, pwm
} SwitchState;
SwitchState dmmFunction;

extern char NewKeyPressed;
extern char buttonPress;
extern char FoundKey;

void main(void) {
	char input;
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	/* Configure MCLK/SMCLK source to DCO, with DCO = 12MHz */
	CS->KEY = CS_KEY_VAL;                // Unlock CS module for register access
	CS->CTL0 = 0;                           // Reset tuning parameters
	CS->CTL0 = CS_CTL0_DCORSEL_3; // Set DCO to 12MHz (nominal, center of 8-16MHz range)
	CS->CTL1 = CS_CTL1_SELA_2 |             // Select ACLK = REFO
			CS_CTL1_SELS_3 |                // SMCLK = DCO
			CS_CTL1_SELM_3;                 // MCLK = DCO
	CS->KEY = 0;                      // Lock CS module from unintended accesses

	ConfigureUART();
	keypadconfiguration();
	buttonConfig();
	__enable_irq();
	NVIC->ISER[0] |= (1) << TA1_0_IRQn;
	NVIC->ISER[0] = (1 << EUSCIB0_IRQn);
	KeypadPort->OUT = (KeypadPort->OUT & ~KeypadOutputPins)
			| (0b11110000 & KeypadOutputPins);
	char DataBuffer[20];
	uint32_t current_size,decimal = 0; //buffer is of size 0 from size, decimal is stored
	while (1) {
		if (buttonPress) {
			buttonPress = NO;
			decimal = convertHex(&DataBuffer,current_size);
			current_size = 0; //buffer is of size 0 from size
		}
		if (NewKeyPressed) {
			input = FoundKey;
			NewKeyPressed = NO;
			if (input != '\x0') {
				if (current_size == 0) {
					SendCharArray("\r\n Key Found: ");
				}
				if (current_size <= maxSize) { //limit hex input to 4 characters
					DataBuffer[current_size] = input;
					SendChar(DataBuffer[current_size]);
					current_size++;
				}
			}
		}
	}
}

/*
 * PrintOutBuffer
 * Input: character buffer, int size
 * Output: none
 * Description: Prints out the inputted value after pressing button
 */
void printOutBuffer(char Buffer[], uint32_t size) {
	SendCharArray("\r\n\r\n final value: ");
	uint32_t count;
	for (count = 0; count < size; count++) {
		SendChar(Buffer[count]);
	}
}//end printOutBuffer()

/*
 * convertHex
 * Input: databuffer address, size of input in buffer
 * Output: decimal integer
 * Description: Carrys out functions necessary for hex to dec and to binary conversions
 */
uint32_t convertHex(char *DataBuffer,uint32_t current_size){
	uint32_t dec;
	printOutBuffer(DataBuffer, current_size);
	dec = hextoDec(&DataBuffer[current_size], current_size );
	sprintf(CharBuffer,"\r\n decimal: %6.4d", dec);
	SendCharArray(&CharBuffer);
	sprintf(CharBuffer,"\r\n binary: ");
	SendCharArray(&CharBuffer);
	dectoBinary(dec);
	return dec;
}//end convertHex



