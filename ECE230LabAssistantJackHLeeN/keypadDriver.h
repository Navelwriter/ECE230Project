/*
 * keypadDriver.h
 *
 *  Created on: Feb 7, 2022
 *      Author: leeni
 */

#ifndef KEYPADDRIVER_H_
#define KEYPADDRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "msp.h"

const uint16_t pins[] = { row1, row2, row3, row4, col5, col6, col7, col8, NULL };

/*!
 * \brief Initialize the input pins necessary for the keypad
 *
 * \return None
 */
extern void initKeypad(void);

/*!
 * \brief Reads input by going through array and their pin values, as well as using recursion
 *
 * \return None
 */
extern char readKeypad(uint16_t nextPin);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* PWMRGBLED_H_ */

#endif /* KEYPADDRIVER_H_ */
