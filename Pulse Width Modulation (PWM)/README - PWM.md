This schematic demonstrates generating two independent PWM signals on MSP430G2553 using Timer_A:

D1 (LED-AQUA): Connected to P1.6 (TA0.1), receives 25% duty cycle PWM.

D2 (LED-AQUA): Connected to P1.7 (TA0.2), receives 75% duty cycle PWM.

Hardware Description
Microcontroller: MSP430G2553

LEDs D1 and D2: Anodes connected to P1.6 and P1.7; cathodes grounded.

Pins P1.6 and P1.7 configured as Timer_A PWM channels via the P1SEL register.

Firmware Flow
Watchdog timer stopped to prevent resets.

P1.6 and P1.7 set as outputs and peripheral select for Timer_A PWM.

Timer_A configuration:

TA0CCR0 = 999 sets the PWM period (1000 clock cycles, ~1ms at 1MHz SMCLK).

TA0CCR1 = 250: PWM output on P1.6 (25% duty cycle; HIGH for 250 counts, LOW for 750 counts).

TA0CCR2 = 750: PWM output on P1.7 (75% duty cycle; HIGH for 750 counts, LOW for 250 counts).

Both TA0CCTL1 and TA0CCTL2 use OUTMOD_7 (Reset/Set), which generates standard PWM pulses.

Timer_A runs in up mode (MC_1) with SMCLK source (TASSEL_2).

Summary
D1 LED blinks dimmer (on for 25% of period).

D2 LED blinks brighter (on for 75% of period).
