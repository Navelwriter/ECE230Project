/*
 * hex2dec.h
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





/*
 * convertHex
 * Input: databuffer address, size of input in buffer
 * Output: decimal integer
 * Description: Calls functions necessary for hex to dec and to binary conversions
 */
uint32_t convertHex(char *DataBuffer,uint32_t current_size);

/*
 * convertDecimal
 * Input: databuffer address, size of input in buffer
 * Output: decimal integer
 * Description: Calls functions necessary for dec to hex and to binary conversions
 */
uint32_t convertDecimal(char *DataBuffer,uint32_t current_size);

/*
 * hextoDec
 * Input: databuffer address, size of input in buffer
 * Output: decimal integer
 * Description: Given buffer address and size, returns the raw decimal as char array
 */
uint32_t hextoDec(char *Buffer, uint32_t size);

/*
 * hextoDec
 * Input: decimal integer
 * Output: Terminal Output
 * Description: Given decimal, prints out binary value to terminal
 */
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
