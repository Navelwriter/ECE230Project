#include "msp.h"
#include "keypadscan_subroutines.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "csHFXT.h"
#include "csLFXT.h"
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

enum Status {NO, YES};
extern char NewKeyPressed;
extern char FoundKey;

void main(void)
{
	char letter;
	uint32_t count;
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
	kepadconfiguration();
    __enable_irq();
	NVIC->ISER[0] |= (1) << TA1_0_IRQn;

	NVIC->ISER[0] |= (1) << TA1_0_IRQn;
	NVIC->ISER[0] = (1 << EUSCIB0_IRQn);
    //KeypadPort->OUT=(KeypadPort->OUT & ~KeypadOutputPins) | (0b11110000 & KeypadOutputPins);
	char DataBuffer[25];
	size_t current_size = 0; //buffer is of size 0 from size
	uint32_t delay;
	while(1){
		letter = GetChar(); //Check for input
		//if(NewKeyPressed){
			//letter = FoundKey;
           // NewKeyPressed=NO;
		if(letter != NULL){
            if(letter == 'l'){
            	SendCharArray("\r\n final value: ");
            	for(delay = 0; delay <= 3000; delay++){}
            	for(count = 0; count <= current_size; count++){
            		SendChar(DataBuffer[count]);
            	}
            	current_size = 0; //buffer is of size 0 from size
            }
            else{
                if(current_size == 0){
                    SendCharArray("\r\n Key Found: ");

                }
            	DataBuffer[current_size] = letter;
                if(current_size <= 24){
                	current_size++;
                }
    			SendChar(letter);


            }
		}
	}
}
