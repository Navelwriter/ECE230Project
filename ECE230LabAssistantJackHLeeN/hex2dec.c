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

volatile char CharBuffer[30];
const char *bin[16] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110",
		"0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };

//Function Prototypes
void SendChar(char letter);
void SendCharArray(char *Buffer);
void printOutBuffer(char Buffer[], uint32_t size);

uint32_t convertDecimal(char *Buffer, uint32_t size) {
	__disable_irq();
	uint32_t base = 1; //Base value to 1
	uint32_t decVal = 0;
	unsigned int count;
	for (count = 0; count <= size; count++) { //LSB first
		if (*Buffer >= '0' && *Buffer <= '9') {
			decVal += (*Buffer - 48) * base;
			base = base * 10;
		}
		Buffer--;
	}
	memset(&CharBuffer[0], 0, sizeof(CharBuffer)); //Clear the characterbuffer
	dectoBinary(decVal);								//Print binary
	sprintf(CharBuffer, "\r\n Hex: %x", decVal); 			//Print Hex
	SendCharArray(&CharBuffer);
	__enable_irq();
	return decVal;

}

uint32_t convertBinary(char *DataBuffer, uint32_t current_size) {
	__disable_irq();
	uint32_t decVal;
	decVal = bintoDec(&DataBuffer[current_size], current_size);
	sprintf(CharBuffer, "\r\n decimal: %d", decVal);
	SendCharArray(&CharBuffer);
	dectoBinary(decVal);
	sprintf(CharBuffer, "\r\n Hex: %x", decVal); 			//Print Hex
	SendCharArray(&CharBuffer);
	__enable_irq();
	return decVal;
}

uint32_t convertHex(char *DataBuffer, uint32_t current_size) {
	__disable_irq();
	uint32_t decVal,count;
	decVal = hextoDec(&DataBuffer[current_size], current_size);
	sprintf(CharBuffer, "\r\n decimal: %d", decVal);
	SendCharArray(&CharBuffer);
	dectoBinary(decVal);
	SendCharArray("\r\n Hex: ");
	for (count = 0; count < current_size; count++) {
		SendChar(DataBuffer[count]);
	}
	__enable_irq();
	return decVal;
} 			//end convertHex

uint32_t hextoDec(char *Buffer, uint32_t size) {
	uint32_t base = 1; //Base value to 1
	uint32_t decVal = 0;
	uint32_t count;
	for (count = 0; count <= size; count++) { //LSB first
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
	unsigned int mask = 32768;   //mask = [1000 0000 0000 0000]
	while (mask > 0) {
		if ((num & mask) == 0)
			SendChar('0');
		else
			SendChar('1');
		mask = mask >> 1;  // Right Shift
	}
}

uint32_t bintoDec(char *Buffer, uint32_t size) {
	uint32_t base = 1; //Base value to 1
	uint32_t decVal = 0;
	unsigned int count;
	for (count = 0; count <= size; count++) { //LSB first
		if (*Buffer == '0') {
			base = base * 2;
		} else if (*Buffer == '1') {
			decVal += base;
			base = base * 2;
		}
		Buffer--;
	}
	return decVal;

}

