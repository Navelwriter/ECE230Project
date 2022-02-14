/*
 * hex2dec.c
 *
 *  Created on: Feb 14, 2022
 *      Author: leeni
 */
#ifndef HEX2DEC_H_
#define HEX2DEC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <msp.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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
	__enable_irq(); //Disable interrupts until done

	return decVal;
}

#endif /* HEX2DEC_H_ */
