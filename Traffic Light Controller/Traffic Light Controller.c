// Code by Aadhitya G
//Traffic Light Controller using MSP430G2553

#include <msp430.h>

#define DELAY_1SEC 1000

void delay_sec(unsigned int sec) {
    volatile unsigned int i;
    while(sec--) {
        for(i = 0; i < DELAY_1SEC; i++);
    }
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    P1DIR |= BIT0 | BIT1 | BIT2 | BIT5 | BIT6 | BIT7;
    P2DIR |= BIT5 | BIT6 | BIT7;
    P3DIR |= BIT0 | BIT1 | BIT2;

    P1OUT |= BIT0 | BIT1 | BIT2;
    P1OUT &= ~(BIT5 | BIT6 | BIT7);
    P2OUT |= BIT5 | BIT6 | BIT7;
    P3OUT &= ~(BIT0 | BIT1 | BIT2);

    while(1) {
        P1OUT &= ~BIT0;
        P1OUT |= BIT1;
        P1OUT |= BIT2;

        P1OUT |= BIT5;
        P1OUT &= ~BIT6;
        P1OUT &= ~BIT7;

        P2OUT |= BIT5;
        P2OUT |= BIT6;
        P2OUT &= ~BIT7;

        P3OUT |= BIT0;
        P3OUT &= ~BIT1;
        P3OUT &= ~BIT2;

        delay_sec(30);

        P1OUT |= BIT0;
        P1OUT &= ~BIT1;
        delay_sec(3);
        P1OUT |= BIT1;
        P1OUT &= ~BIT2;

        P1OUT |= BIT5;
        P1OUT |= BIT7;
        delay_sec(25);

        P1OUT &= ~BIT7;
        P1OUT |= BIT6;
        delay_sec(3);
        P1OUT &= ~BIT5;
        P1OUT &= ~BIT6;

        P2OUT &= ~BIT5;
        P2OUT |= BIT6;
        P2OUT |= BIT7;

        delay_sec(15);

        P2OUT |= BIT5;
        P2OUT &= ~BIT6;
        delay_sec(3);
        P2OUT |= BIT6;
        P2OUT &= ~BIT7;

        P3OUT &= ~BIT0;
        P3OUT |= BIT2;
        delay_sec(20);

        P3OUT &= ~BIT2;
        P3OUT |= BIT1;
        delay_sec(3);
        P3OUT &= ~BIT1;
        P3OUT |= BIT0;
    }
}
