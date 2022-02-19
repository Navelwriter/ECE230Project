/*
 * uart_routines.c
 * Baud Rate 57600 1stop, even 1
 *
 *  Created on: Jan 28, 2022
 *      Author: Noah Lee
 */
#include <msp.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define UARTport P1
#define UARTpins   BIT2 | BIT3
#define SYSTEMCLOCK 12000000    //Hz
#define BAUDRATE    38400   //bits per seconds (Hz)
#define ClockPrescalerValue SYSTEMCLOCK/(16*BAUDRATE)
#define FirstModulationStage 0
#define SecondModulationStage 73

void ConfigureUART(void) {
	/* Configure UART pins */
	UARTport->SEL0 |= UARTpins; // set 2-UART pins as secondary function
	UARTport->SEL1 &= ~(UARTpins);

	/* Configure UART
	 *  Asynchronous UART mode, 8O1 (8-bit data, even parity, 1 stop bit),
	 *  LSB first, SMCLK clock source
	 */
	EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset to configure eUSCI

	EUSCI_A0->CTLW0 |= 0b1100000010000000; //bits 15-14 for parity and bits 7-6 SMCLK

	/* Baud Rate calculation
	 * Refer to Section 24.3.10 of Technical Reference manual
	 * BRCLK = 12000000, Baud rate = 57600 */

	EUSCI_A0->BRW = ClockPrescalerValue;

	EUSCI_A0->MCTLW = (SecondModulationStage << 8) + (FirstModulationStage << 4)
			+ 1;    //enalble oversampling
	EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;    // Initialize eUSCI
	EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;        // Clear eUSCI RX interrupt flag
	EUSCI_A0->IE |= EUSCI_A_IE_RXIE;            // Enable USCI_A0 RX interrupt
} //end ConfigureUART(void)

void SendChar(char letter) { //Single character to terminal
	while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG))
		;
	EUSCI_A0->TXBUF = letter;
} //end SendChar
void SendAll(char *Buffer, unsigned int length) {
	unsigned int count;
	for (count = 0; count <= length; count++) {
		// Check if the TX buffer is empty first
		while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG))
			;
		EUSCI_A0->TXBUF = Buffer[count];
	}   //end for()
}

void SendCharArray(char *Buffer) {
	unsigned int count;
	for (count = 0; count < strlen(Buffer); count++) {
		// Check if the TX buffer is empty first
		while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG))
			;
		EUSCI_A0->TXBUF = Buffer[count];
	}   //end for()
} // end SendCharArray(char *Buffer)

//get a ASCII character from UART
//this is a blocking call
char GetChar(void) {
//non blocking call
	if ((EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG) == EUSCI_A_IFG_RXIFG)
		return EUSCI_A0->RXBUF;
	else
		return NULL;
} //end GetChar(void)

void printOutBuffer(char Buffer[], uint32_t size) {
	uint32_t count;
	for (count = 0; count < size; count++) {
		SendChar(Buffer[count]);
	}
}//end printOutBuffer
// UART interrupt service routine
void EUSCIA0_IRQHandler(void) {
	if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG) {
		// Check if the TX buffer is empty first
		while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG))
			;

		// Echo the received character back
		//  Note that reading RX buffer clears the flag and removes value from buffer
		EUSCI_A0->TXBUF = EUSCI_A0->RXBUF;
	}
}
