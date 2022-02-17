/*
 * hex2dec.c
 *
 *  Created on: Feb 14, 2022
 *      Author: leeni
 */

#include <msp.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <hex2dec.h>

char CharBuffer[30];
const char *bin[16] = {"0000", "0001", "0010", "0011", "0100", "0101","0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110","1111"};

//Function Prototypes
void SendChar(char letter);
void SendCharArray(char *Buffer);

uint32_t convertDecimal(char *Buffer, uint32_t size){
	uint32_t base = 1; //Base value to 1
	uint32_t decVal = 0;
	unsigned int count;
	for(count = 0; count <= size; count++) { //LSB first
		if (*Buffer >= '0' && *Buffer <= '9') {
			decVal += (*Buffer - 48) * base;
			base = base * 10;
		}
		Buffer++;
	}
	dectoBinary(decVal);
	return decVal;

}


uint32_t convertHex(char *DataBuffer,uint32_t current_size){
	uint32_t dec;
	dec = hextoDec(&DataBuffer[current_size], current_size );
	sprintf(CharBuffer,"\r\n decimal: %6.4d", dec);
	SendCharArray(&CharBuffer);
	dectoBinary(dec);
	return dec;
}//end convertHex

uint32_t hextoDec(char *Buffer, uint32_t size) {
	__disable_irq(); //Disable interrupts until done
	uint32_t base = 1; //Base value to 1
	uint32_t decVal = 0;
	unsigned int count;
	for(count = 0; count <= size; count++) { //LSB first
		if (*Buffer >= '0' && *Buffer <= '9') {
			decVal += (*Buffer - 48) * base;
			base = base * 16;
		} else if (*Buffer >= 'A' && *Buffer <= 'F') {
			decVal += (*Buffer - 55) * base;
			base = base * 16;
		}
		Buffer--;
	}

	return decVal;
}


void dectoBinary(int num) {
	SendCharArray("\r\n binary: ");
	unsigned int mask=32768;   //mask = [1000 0000 0000 0000]

	while(mask > 0)
	   {
	   if((num & mask) == 0 )
		   SendChar('0');
	   else
		   SendChar('1');
	  mask = mask >> 1 ;  // Right Shift
	   }
}

