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

#define WAVEGEN_PORT P2
#define WAVEGEN_BIT BIT4
#define SMCLK 12000000 //Hz
#define TimerA1Prescaler 1 //Timer A prescaler
#define TimerA1Clock  SMCLK
#define Frequency 200	 //Hz
#define dutycycle TimerA1Clock/Frequency
void SendCharArray(char *Buffer);

void wavegenConfig(void) {
	WAVEGEN_PORT->DIR |= WAVEGEN_BIT;            // set P2.7 as output
	WAVEGEN_PORT->SEL0 |= WAVEGEN_BIT;           // P2.7 set to TA0.1
	//WAVEGEN_PORT->SEL1 &= ~WAVEGEN_BIT;		 // Option 0b01
	//WAVEGEN_PORT->OUT &= ~WAVEGEN_BIT;		 // Option 0b01
	TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    // Configure TIMER_A1 and CCRs
    // Set Period in CCR0 register
	TIMER_A1->CCR[0] = dutycycle - 1;
    // Set high pulse-width in CCR1 register (determines duty cycle)
	TIMER_A1->CCR[1] = (dutycycle / 2) - 1;
    // Configure CCR1 for Toggle when it counts up to ccr0
	TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_3 | TIMER_A_CCTLN_CCIE;
    //      Tick rate will be (48MHz/4) with rollover at value in CCR0
	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK |  TIMER_A_CTL_MC__UP; //ACLK in contin
	NVIC->ISER[0] |= (1 << (TA0_0_IRQn));
}
void changeDutyCycle(unsigned int duty){
    // Set high pulse-width in CCR1 register (determines duty cycle)
	TIMER_A1->CCR[1] = duty * (Frequency / 100) - 1;
	sprintf(CharBuffer,"\r\n Now outputting at %u percent duty cycle", duty);
	SendCharArray(&CharBuffer);
}
void TA0_0_IRQHandler(void) {
	/* Not necessary to check which flag is set because only one IRQ
	 *  mapped to this interrupt vector     */
	TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
}

void toggleWavegen(void){
	TIMER_A1->CTL ^= 0b0000000000010000;     // toggle TIMER
	NVIC->ISER[0] ^= (1) << ((TA0_0_IRQn) & 31); //toggle interrupt

}
