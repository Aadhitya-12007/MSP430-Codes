// Code by Aadhitya G
// 25% and 75% PWM 
#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   

   
    P1DIR |= BIT6 | BIT7;
    P1SEL |= BIT6 | BIT7;     

   
    TA0CCR0 = 999;            

    
    TA0CCR1 = 250;            
    TA0CCR2 = 750;              

    
    TA0CCTL1 = OUTMOD_7;        
    TA0CCTL2 = OUTMOD_7;
    TA0CTL = TASSEL_2 + MC_1 + TACLR;
    while (1)
    {
        __no_operation();
    }
}
