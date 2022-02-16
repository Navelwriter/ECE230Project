/*
 * hex2dec.h
 *
 *  Created on: Feb 14, 2022
 *      Author: leeni
 */

#ifndef HEX2DEC_H_
#define HEX2DEC_H_
#include "msp.h"

#ifdef __cplusplus
extern "C" {
#endif


#include <math.h>
#include <stdio.h>
uint32_t hextoDec(char *Buffer, uint32_t size);
void dectoBinary(int n);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* HEX2DEC_H_ */
