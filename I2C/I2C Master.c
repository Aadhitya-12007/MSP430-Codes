#include <msp430g2553.h>
#define SLAVE_ADDR 0x48    // 7-bit I2C slave address

void I2C_init(void) {
    P1SEL  |= BIT6 + BIT7;      // Assign I2C pins to USCI_B0 (SCL, SDA)
    P1SEL2 |= BIT6 + BIT7;

    UCB0CTL1 |= UCSWRST;        // Software reset enabled
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;        // Use SMCLK, keep reset
    UCB0BR0 = 12;               // Baudrate = SMCLK / 12 ~100kHz (if SMCLK = 1.2MHz)
    UCB0BR1 = 0;
    UCB0I2CSA = SLAVE_ADDR;     // Set slave address
    UCB0CTL1 &= ~UCSWRST;       // Clear reset, enable I2C
}

void I2C_write_byte(unsigned char data) {
    while (UCB0CTL1 & UCTXSTP);     // Ensure stop condition sent
    UCB0CTL1 |= UCTR + UCTXSTT;     // I2C TX, start condition
    while (!(IFG2 & UCB0TXIFG));    // Wait for TX int flag
    UCB0TXBUF = data;               // Put data in buffer
    while (!(IFG2 & UCB0TXIFG));    // Wait for TX int flag
    UCB0CTL1 |= UCTXSTP;            // I2C stop condition
    IFG2 &= ~UCB0TXIFG;             // Clear flag
    while (UCB0CTL1 & UCTXSTP);     // Wait for stop
}

void delay(void) {
    volatile unsigned int i;
    for (i = 0; i < 50000; i++);
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop WDT
    BCSCTL1 = CALBC1_1MHZ;      // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ;

    I2C_init();

    while (1) {
        I2C_write_byte(0x55);   // Send 0x55 to the slave repeatedly
        delay();
    }
}
