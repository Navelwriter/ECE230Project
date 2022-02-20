/*
 * ShiftReg.h
 *
 *  Created on: Feb 19, 2022
 *      Author: jackh
 */

#ifndef SHIFTREG_H_
#define SHIFTREG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "msp.h"

#define Master_Clock            100 // period in us

extern void initShiftReg(void);

extern void writechar(uint8_t input, uint8_t spot, uint8_t display);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif



#endif /* SHIFTREG_H_ */
