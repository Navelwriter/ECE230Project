#include "msp.h"
#include "csHFXT.h"
#include "csLFXT.h"
#include "ShiftReg.h"

// abcdefgDP
#define Zero 0b11111100
#define Six 0b10111110
#define Nine 0b11100110
#define A 0b11101110
#define F 0b10001110
#define CLkMasks (BIT3 | BIT6)
#define Display1 (BIT7) //DEC
#define Display2 (BIT5) //METER
#define Displays (BIT5 | BIT7)
#define StrobePins1 (BIT3 | BIT4 | BIT5) // Port 2 dec
#define StrobePins2 (BIT4 | BIT5 | BIT6) // Port 6

//void ConfigureCS(void)
//{
//    CS->KEY = CS_KEY_VAL; // Unlock CS module for register access
//    CS->CTL0 = 0; // Reset tuning parameters
//    CS->CTL0 = CS_CTL0_DCORSEL_3; // Set DCO to 12MHz (nominal, center of 8-16MHz range)
//    CS->CTL1 = CS_CTL1_SELA_2 | // Select ACLK = REFO
//            CS_CTL1_SELS_3 | // SMCLK = DCO
//            CS_CTL1_SELM_3; // MCLK = DCO
//    CS->KEY = 0; // Lock CS module from unintended accesses
//} //end ConfigureCS(void)
/**
 * main.c
 */

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
    ConfigureCS();
    initShiftReg();
//NVIC->ISER[0] |= (1)<<TA1_0_IRQn;
    __enable_irq();
    P3->SEL0 &= ~Displays; // Set LED1 pin to GPIO function
    P3->SEL1 &= ~Displays;
    P3->OUT &= ~Displays; // Clear LED1 to start
    P3->DIR |= Displays; // Set P1.0/LED1 to output
    P3->SEL0 &= ~CLkMasks; // Set LED1 pin to GPIO function
    P3->SEL1 &= ~CLkMasks;
    P3->OUT &= ~CLkMasks; // Clear LED1 to start
    P3->DIR |= CLkMasks;

    P6->SEL0 &= ~StrobePins2; // Set LED1 pin to GPIO function
    P6->SEL1 &= ~StrobePins2;
    P6->OUT &= ~StrobePins2; // Clear LED1 to start
    P6->DIR |= StrobePins2;

    P2->SEL0 &= ~StrobePins1; // Set LED1 pin to GPIO function
    P2->SEL1 &= ~StrobePins1;
    P2->OUT &= ~StrobePins1; // Clear LED1 to start
    P2->DIR |= StrobePins1;
//    writechar(Six, 1, 1);
//    writechar(Nine, 2, 1);
//    writechar(0b01101110, 3, 1);
//    writechar(0b10011110, 1, 2);
//    writechar(0b10011100, 2, 2);
//    writechar(0b10011110, 3, 2);
//    writechar(0b0, 4, 2);
//    writechar(0x0, 0, 2);
    writechar(0b00000001, 0, 1);
    writechar(0b00000010, 1, 1);
    writechar(0b00000100, 2, 1);
    writechar(0b00001000, 3, 1);
    writechar(0b00010000, 4, 1);

    while (1)
    {

    }
}
