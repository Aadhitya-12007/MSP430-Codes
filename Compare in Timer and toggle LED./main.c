#include <msp430.h>

void main(void) {
    WDTCTL   = WDTPW | WDTHOLD;
    P1DIR   |= BIT0;
    P1OUT   &= ~BIT0;

    TA0CCR0  = 1'000'000 - 1;
    TA0CCTL0 = CCIE;
    TA0CTL   = TASSEL_2 | MC_1 | TACLR;
    __enable_interrupt();
    for(;;);
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void) {
    P1OUT ^= BIT0;
}
