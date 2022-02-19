/*
 * wavegen.c
 *
 *  Created on: Feb 18, 2022
 *      Author: Noah Lee
 */
#include <msp.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "csHFXT.h"
#include "csLFXT.h"
volatile char CharBuffer[30];

#define WAVEGEN_PORT P5
#define WAVEGEN_BIT BIT0
#define SMCLK 48000000 //Hz
#define TimerA1Prescaler 4 //Timer A prescaler
#define TimerA1Clock  SMCLK/TimerA1Prescaler
#define Frequency 200.00	 //Hz
#define dutycycle TimerA1Clock/Frequency

void wavegenConfig(void) {
	WAVEGEN_PORT->DIR |= WAVEGEN_BIT;            // set P2.4 as output​
	WAVEGEN_PORT->SEL0 |= WAVEGEN_BIT;           // P2.4 set to TA0.1​
	WAVEGEN_PORT->SEL1 &= ~WAVEGEN_BIT;		 // Option 0b01​
    // Configure TIMER_A1 and CCRs ​
    // Set Period in CCR0 register​
	TIMER_A1->CCR[0] = Frequency - 1;
    // Set high pulse-width in CCR1 register (determines duty cycle)​
	TIMER_A1->CCR[1] = (dutycycle / 2) - 1;
    // Configure CCR1 for Compare mode with Set/Reset output​
    //          - sets when TIMER_A1 == CCR1​
    //          - resets when TIMER_A1 == CCR0​
	TIMER_A1->CCTL[1]=0x0160;
    // Configure TIMER_A1 in Up Mode with source SMCLK prescale 4:1​
    //      Tick rate will be (48MHz/4) with rollover at value in CCR0​
	TIMER_A1->CTL=0b0000001010010000;
}

void changeDutyCycle(unsigned int duty){
    // Set high pulse-width in CCR1 register (determines duty cycle)​
	TIMER_A1->CCR[1] = duty * (Frequency / 100) - 1;
	fprintf(CharBuffer,"\r\n Now outputting at %d percent duty cycle", duty);
	SendCharArray(&CharBuffer);
	NVIC->ISER[0] |= (1) << TA1_0_IRQn; //enable interrupt
}
void TA1_0_IRQHandler(void) {
	/* Not necessary to check which flag is set because only one IRQ
	 *  mapped to this interrupt vector     */
	if (TIMER_A1->CCTL[0] & TIMER_A_CCTLN_CCIFG) {
		TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  //clear interrupt flag
	}
}

void toggleWavegen(void){
	TIMER_A1->CTL ^= 0b0000000000010000;     // toggle TIMER
	NVIC->ISER[0] ^= (1) << TA1_0_IRQn; //toggle interrupt

}
