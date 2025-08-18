// Code by Aadhitya G

#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P1DIR &= ~BIT2;
    P1REN |= BIT2;
    P1OUT |= BIT2;
    P1SEL |= BIT2;
    P1SEL2 &= ~BIT2;
    TA0CTL = TASSEL_2 | MC_2 | TACLR;
    TA0CCTL1 = CM_3 | CCIS_0 | SCS | CAP | CCIE;
    __enable_interrupt();
    while(1);
}
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void) {
    if (TA0IV == TA0IV_TACCR1) {
        P1OUT ^= BIT0;
        TA0CCTL1 &= ~CCIFG;
    }
}
