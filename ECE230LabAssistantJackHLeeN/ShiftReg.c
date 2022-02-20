#include "ShiftReg.h"
#include "msp.h"
#include "csHFXT.h"

volatile uint8_t TimerCount;
uint32_t period = 2000;
uint8_t outchar = 0b10001001;
//order             abcdefgdp

void initShiftReg(void)
{
    TimerCount = 0;
    TIMER_A1->CCR[0] = period/2;
    TIMER_A1->CCR[1] = period;
    TIMER_A1->CCTL[1] = 0x0010;
    TIMER_A1->CTL = 0b1000010110;
    NVIC->ISER[0] |= (1)<<TA1_N_IRQn;


}
void writechar(uint8_t input){
    outchar = input;
}
void TA1_N_IRQHandler (void){
    TimerCount++;
    if(TimerCount == 100){ // on set
            TimerCount = 0;
    }
    if((TimerCount == 16)){
        P3->OUT |= BIT6;
    } else {
        P3->OUT &= ~BIT6;
    }
    if((TimerCount < 20) && (TimerCount%2 == 0)){
        uint8_t outbit = (outchar >> (TimerCount/2));
        uint8_t outbit2 = (outbit & 0b1);
        if(outbit2 == 0b1){
            P3->OUT |= BIT5;
        } else {
            P3->OUT &= ~BIT5;
        }
        //P3->OUT ^= BIT5;
    }

    P3->OUT ^= BIT3;
    TIMER_A1->CTL &= ~TIMER_A_CTL_IFG;
    TIMER_A1->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
}



