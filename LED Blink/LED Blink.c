//Code by Aadhitya G
//MSP430G2553 - LED Blink Bare Metal code
#include <msp430.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;   

    P2SEL &= ~(BIT0 | BIT1);  
    P2SEL2 &= ~(BIT0 | BIT1); 

    P2DIR |= (BIT0 | BIT1); 
    P2OUT &= ~(BIT0 | BIT1); 

    volatile unsigned int i;
    while (1) {
        P2OUT ^= (BIT0 | BIT1); 
        for (i = 0xFFFF; i; --i); 
    }

    return 0;
}
