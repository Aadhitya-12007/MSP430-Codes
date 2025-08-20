// Code by Aadhitya G
//SLAVE
#include <msp430g2553.h>

volatile char led_state = 0;     // Current LED state
volatile char spi_received = 0;  // Flag for SPI data received

void SPI_init(void) {
    P1SEL  |= BIT5 + BIT6 + BIT7; // P1.5=CLK, P1.6=SOMI, P1.7=SIMO
    P1SEL2 |= BIT5 + BIT6 + BIT7;
    P1DIR  |= BIT6;               // SOMI as output
    P1DIR  &= ~(BIT4 + BIT5 + BIT7); // CS, CLK, SIMO as inputs
    
    UCB0CTL0 |= UCSYNC + UCCKPL + UCMSB; // SPI slave, CLK idle high
    UCB0CTL1 &= ~UCSSEL_2;        // Don't select clock (slave)
    UCB0CTL1 &= ~UCSWRST;         // Release for operation
    
    IE2 |= UCB0RXIE;              // Enable RX interrupt
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    char received_data = UCB0RXBUF; // Read received data
    
    if(received_data == 0xAA) {     // LED ON command
        P1OUT |= BIT0;              // Turn LED ON
        led_state = 1;
        UCB0TXBUF = 0xAA;           // Send confirmation
    }
    else if(received_data == 0x55) { // LED OFF command
        P1OUT &= ~BIT0;             // Turn LED OFF
        led_state = 0;
        UCB0TXBUF = 0x55;           // Send confirmation
    }
    else {
        UCB0TXBUF = 0xFF;           // Send error code
    }
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;     // Stop WDT
    
    // Clock setup
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    
    // LED setup
    P1DIR |= BIT0;                // P1.0 as output for LED
    P1OUT &= ~BIT0;               // LED OFF initially
    
    SPI_init();
    __bis_SR_register(GIE);       // Enable global interrupts
    
    while(1) {
        __bis_SR_register(LPM0_bits + GIE); // Low power mode
    }
}
