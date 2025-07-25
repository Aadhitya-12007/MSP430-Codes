//Code by Aadhitya G
//MSP430G2553 - Two Switches and Two LEDs Bare Metal code
#include <msp430.h>

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer

    // Setup LED outputs
    P1DIR |= BIT0 | BIT1;        // P1.0 and P1.1 as outputs
    P1OUT &= ~(BIT0 | BIT1);     // Both LEDs OFF initially

    // Setup Switch1 (P2.2, pull-up, negative logic)
    P2DIR &= ~BIT2;              // P2.2 as input
    P2REN |= BIT2;               // Enable resistor
    P2OUT |= BIT2;               // Pull-up (reads '1' when open)

    // Setup Switch2 (P2.3, pull-down, positive logic)
    P2DIR &= ~BIT3;              // P2.3 as input
    P2REN |= BIT3;               // Enable resistor
    P2OUT &= ~BIT3;              // Pull-down (reads '0' when open)

    volatile unsigned int i;
    while (1) {
        // Switch1: pressed → P2IN2 = 0
        if (!(P2IN & BIT2))      // Negative logic: 0 means pressed
            P1OUT |= BIT1;       // Turn ON LED at P1.1 (positive logic)
        else
            P1OUT &= ~BIT1;      // Else OFF

        // Switch2: pressed → P2IN3 = 1
        if (P2IN & BIT3)         // Positive logic: 1 means pressed
            P1OUT &= ~BIT0;      // Negative logic LED ON means output '0'
        else
            P1OUT |= BIT0;       // Else LED OFF (output '1')

        for (i = 50000; i; --i);
    }
}
