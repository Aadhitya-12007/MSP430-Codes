// Code by Aadhitya G
//MASTER
#include <msp430g2553.h>

char led_status = 0;  // 0 = OFF, 1 = ON
char spi_data = 0x00;
char feedback = 0x00;

void UART_init(void) {
    P1SEL  |= BIT1 + BIT2;       // P1.1 = RXD, P1.2 = TXD
    P1SEL2 |= BIT1 + BIT2;
    
    UCA0CTL1 |= UCSSEL_2;        // SMCLK
    UCA0BR0 = 104;               // 1MHz, 9600 baud
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;           // Modulation
    UCA0CTL1 &= ~UCSWRST;        // Initialize USCI
}

void SPI_init(void) {
    P1SEL  |= BIT5 + BIT6 + BIT7; // P1.5=CLK, P1.6=SOMI, P1.7=SIMO
    P1SEL2 |= BIT5 + BIT6 + BIT7;
    P1DIR  |= BIT4 + BIT5 + BIT7; // CS, CLK, SIMO as outputs
    P1OUT  |= BIT4;               // CS high (idle)
    
    UCB0CTL0 |= UCMST + UCSYNC + UCCKPL + UCMSB; // Master, SPI, CLK idle high
    UCB0CTL1 |= UCSSEL_2;         // SMCLK
    UCB0BR0  = 0x02;              // Divide by 2
    UCB0BR1  = 0;
    UCB0CTL1 &= ~UCSWRST;         // Release for operation
}

void UART_send_string(char* str) {
    while(*str) {
        while (!(IFG2 & UCA0TXIFG)); // Wait for TX ready
        UCA0TXBUF = *str++;           // Send character
    }
}

char SPI_exchange(char data) {
    P1OUT &= ~BIT4;               // CS low (select slave)
    __delay_cycles(10);
    
    while (!(IFG2 & UCB0TXIFG));  // Wait for TX ready
    UCB0TXBUF = data;             // Send data
    
    while (!(IFG2 & UCB0RXIFG));  // Wait for RX ready
    char received = UCB0RXBUF;    // Read received data
    
    __delay_cycles(10);
    P1OUT |= BIT4;                // CS high (deselect slave)
    return received;
}

void delay_ms(unsigned int ms) {
    volatile unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 1000; j++);
    }
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;     // Stop WDT
    
    // Clock setup
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    
    // Button setup (P1.3 with internal pull-up)
    P1DIR &= ~BIT3;               // P1.3 input
    P1REN |= BIT3;                // Enable pull-up/down
    P1OUT |= BIT3;                // Pull-up
    
    UART_init();
    SPI_init();
    
    UART_send_string("SPI Master Ready\r\n");
    
    while(1) {
        // Check button press (active low)
        if(!(P1IN & BIT3)) {
            delay_ms(50);             // Debounce
            if(!(P1IN & BIT3)) {      // Confirm press
                
                // Toggle LED command
                led_status = !led_status;
                spi_data = led_status ? 0xAA : 0x55; // ON command : OFF command
                
                // Send SPI command and get feedback
                feedback = SPI_exchange(spi_data);
                
                // Display on Virtual Terminal
                if(feedback == 0xAA) {
                    UART_send_string("LED is ON\r\n");
                } else if(feedback == 0x55) {
                    UART_send_string("LED is OFF\r\n");
                } else {
                    UART_send_string("Communication Error\r\n");
                }
                
                // Wait for button release
                while(!(P1IN & BIT3));
                delay_ms(50);
            }
        }
        delay_ms(10);
    }
}
