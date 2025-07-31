//Code by Aadhitya G
// Program for Interrupts using Two Switches and Two LEDs

#include <msp430.h>

#define SW1     BIT7
#define LED1    BIT0
#define SW2     BIT6
#define LED2    BIT6

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= (LED1 | LED2);
    P2DIR &= ~(SW1 | SW2);
    P2REN |= (SW1 | SW2);
    P2OUT |= (SW1 | SW2);
    P2IES &= ~(SW1 | SW2);        
    P2IE |= (SW1 | SW2);          
    P2IFG &= ~(SW1 | SW2);        
    __bis_SR_register(GIE);
    while(1);
}
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    volatile unsigned long i;
    if (P2IFG & SW1) {
        P1OUT ^= LED1;
        for (i = 0; i < 10000; i++);
        P2IFG &= ~SW1;
    }
    if (P2IFG & SW2) {
        P1OUT ^= LED2;
        for (i = 0; i < 10000; i++);
        P2IFG &= ~SW2;
    }
}
