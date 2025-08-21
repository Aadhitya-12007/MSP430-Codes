#include <msp430g2553.h>
#define SLAVE_ADDR 0x48
#define LED    BIT0     // P1.0

void I2C_init(void) {
    P1SEL  |= BIT6 + BIT7;
    P1SEL2 |= BIT6 + BIT7;

    UCB0CTL1 |= UCSWRST;            // Enable SW reset
    UCB0CTL0 = UCMODE_3 + UCSYNC;   // I2C Slave, synchronous mode
    UCB0I2COA = SLAVE_ADDR;         // Own address
    UCB0CTL1 &= ~UCSWRST;           // Clear SW reset, resume operation
    IE2 |= UCB0RXIE;                // Enable RX interrupt
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void) {
    if(UCB0RXBUF == 0x55) {
        P1OUT |= LED;               // Light LED on 0x55 receipt
    } else {
        P1OUT &= ~LED;              // Otherwise, LED off
    }
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= LED;
    P1OUT &= ~LED;

    I2C_init();
    __bis_SR_register(GIE);         // Enable global interrupts

    while (1);                      // Stay here, wait for I2C events
}
