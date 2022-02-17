/*
 * gen_routines.c
 *
 *  Created on: Feb 16, 2022
 *      Author: noah1
 */

#include <msp.h>
#include <stdio.h>

/* Configure MCLK/SMCLK source to DCO, with DCO = 12MHz */
void ConfigureCS(void){
	CS->KEY = CS_KEY_VAL;                // Unlock CS module for register access
	CS->CTL0 = 0;                           // Reset tuning parameters
	CS->CTL0 = CS_CTL0_DCORSEL_3; // Set DCO to 12MHz (nominal, center of 8-16MHz range)
	CS->CTL1 = CS_CTL1_SELA_2 |             // Select ACLK = REFO
			CS_CTL1_SELS_3 |                // SMCLK = DCO
			CS_CTL1_SELM_3;                 // MCLK = DCO
	CS->KEY = 0;                      // Lock CS module from unintended accesses
}//end ConfigureCS(void)


