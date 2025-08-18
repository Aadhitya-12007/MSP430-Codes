//Code by Aadhitya G
// UART Transmitter

#include <msp430g2553.h>

char tx_data = 'B';

void UART_init(void) {
    P1SEL  |= BIT1 + BIT2;       // P1.1 = RXD, P1.2 = TXD
    P1SEL2 |= BIT1 + BIT2;
    
    UCA0CTL1 |= UCSSEL_2;        // SMCLK
    UCA0BR0 = 104;               // 1MHz, 9600 baud
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;           // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;        // Initialize USCI state machine
    
    IE2 |= UCA0RXIE;             // Enable USCI_A0 RX interrupt
}

void UART_send_char(char c) {
    while (!(IFG2 & UCA0TXIFG)); // Wait for TX buffer ready
    UCA0TXBUF = c;               // Send character
}

void delay(void) {
    volatile unsigned int i;
    for(i = 50000; i > 0; i--);  // Simple delay
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    char received = UCA0RXBUF;   // Read received character
    if(received == 'A') {
        P1OUT ^= BIT0;           // Toggle LED when 'A' received
        UART_send_char(tx_data); // Send 'B' back
    }
}

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;    // Stop WDT
    
    // Clock setup
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    
    P1DIR |= BIT0;               // P1.0 output for LED
    P1OUT &= ~BIT0;              // LED off initially
    
    UART_init();                 // Initialize UART
    __bis_SR_register(GIE);      // Enable global interrupts
    
    while(1) {
        // Wait for incoming data
        __bis_SR_register(LPM0_bits + GIE); // Low power mode
    }
}
