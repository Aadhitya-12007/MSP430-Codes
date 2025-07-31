//Code by Aadhitya G
//Sample Program to explain the flow of interrupts

#include <msp430.h>
#define SW      BIT7                    // Switch -> P2.7
#define LED     BIT0                    //  LED -> P1.0
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;  //! Stop Watchdog
    PM5CTL0 &= ~LOCKLPM5;
    P1DIR |= LED;               // Set LED pin -> Output
    P2DIR &= ~SW;             // Set SW pin -> Input
    P2REN |= SW;        // Enable Resistor for SW pin
    P2OUT |= SW;         // Select Pull Up for SW pin
    P2IES  &= ~SW;  // Select Interrupt on Rising Edge
    P2IE |= SW;           // Enable Interrupt on SW pin
    __bis_SR_register(GIE);   // Enable CPU Interrupt
    while(1);}
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{    if(P2IFG & SW)          // If SW is Pressed
    {        P1OUT ^= LED;       // Toggle  LED
        volatile unsigned long i;
        for(i = 0; i<10000; i++);       //delay
        P2IFG &= ~SW;  }}  // Clear SW interrupt flag
