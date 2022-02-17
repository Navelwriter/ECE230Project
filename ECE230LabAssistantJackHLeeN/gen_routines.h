/*
 * gen_routines.h
 *
 *  Created on: Feb 16, 2022
 *      Author: noah1
 */

#ifndef GEN_ROUTINES_H_
#define GEN_ROUTINES_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <msp.h>


/* ConfigureCS(void)
 * Configure MCLK/SMCLK source to DCO, with DCO = 12MHz
 * */

void ConfigureCS(void);



//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* GEN_ROUTINES_H_ */
