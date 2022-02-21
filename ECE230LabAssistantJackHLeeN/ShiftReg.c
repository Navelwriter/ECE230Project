#include "ShiftReg.h"
#include "msp.h"
#include "csHFXT.h"

#define StrobePins1 (BIT3 | BIT4 | BIT5) // Port 2 dec
#define StrobePins2 (BIT4 | BIT5 | BIT6) // Port 6 meter
#define DisplayDec  BIT7 //Port 3
#define DisplayMeter BIT5
#define RemCLKOUT   BIT6
#define CLKOUT      BIT3
volatile uint8_t TimerCount;
volatile uint8_t StrobeCount = 4;
volatile uint8_t StrobeSpot;
volatile const uint8_t StrobeOut2[] = {(BIT4), //1               //001
                                      (BIT5),  //2               //010
                                      (BIT5 | BIT6),  //5        //011
                                      (BIT4 | BIT6), //4        //101
                                      (BIT4 | BIT5)  //3        //110

};
volatile const uint8_t StrobeOut1[] = {(0),                 //000
                                       (BIT3),              //100
                                       (BIT4),              //010
                                       (BIT4 | BIT5),       //110
                                       (BIT5)               //001

};
uint32_t period = 3007/3;
uint8_t outchar1 = 0b0;
uint8_t outchar2 = 0b0;
uint8_t outchars1[] = {(0b0),
                      (0b0),
                      (0b0),
                      (0b0),
                      (0b0)

};
uint8_t outchars2[] = {(0xFF),
                      (0b0),
                      (0b0),
                      (0b0),
                      (0b0)

};
//order             abcdefgdp
uint8_t outbit;
uint8_t outbit2;
uint8_t outbit3;
uint8_t outbit4;

void initShiftReg(void)
{
    StrobeSpot = 0;
    TimerCount = 0;
    TIMER_A1->CCR[0] = period/2;
    TIMER_A1->CCR[1] = period;
    TIMER_A1->CCTL[1] = 0x0010;
    TIMER_A1->CTL = 0b1000010110;
    NVIC->ISER[0] |= (1)<<TA1_N_IRQn;





}
void writechar(uint8_t input, uint8_t spot, uint8_t display){
    if(spot > 4 || display > 2 || display == 0){
        return;
    } else {
        if(display == 1){
            outchars1[spot] = input;
        } else {
            outchars2[spot] = input;
        }

    }
}
void TA1_N_IRQHandler (void){
    TimerCount++;
    if(TimerCount == 75){ // on set
            TimerCount = 0;
            StrobeSpot++;
            if(StrobeCount < StrobeSpot){
                StrobeSpot = 0;
            }


    }
    if((TimerCount == 16)){


        P6->OUT &= ~StrobePins2;
        P2->OUT &= ~StrobePins1;
        P3->OUT |= RemCLKOUT;
        P6->OUT |= StrobeOut2[StrobeSpot];
        P2->OUT |= StrobeOut1[StrobeSpot];

    } else {
        P3->OUT &= ~RemCLKOUT;
    }
    if((TimerCount < 20) && (TimerCount%2 == 0)){
        outchar1 = outchars1[StrobeSpot];
        outbit = (outchar1 >> (TimerCount/2));
        outbit2 = (outbit & 0b1);

        outchar2 = outchars2[StrobeSpot];
        outbit3 = (outchar2 >> (TimerCount/2));
        outbit4 = (outbit3 & 0b1);

        if(outbit2 == 0b1){
            P3->OUT |= DisplayDec;
        } else {
            P3->OUT &= ~DisplayDec;
        }
        if(outbit4 == 0b1){
            P3->OUT |= DisplayMeter;
        } else {
            P3->OUT &= ~DisplayMeter;
        }

    }

    P3->OUT ^= CLKOUT;
    TIMER_A1->CTL &= ~TIMER_A_CTL_IFG;
    TIMER_A1->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
}

void TA2_N_IRQHandler (void){

}
