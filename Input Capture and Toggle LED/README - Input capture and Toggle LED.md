The schematic shows a simple demonstration on a TI MSP430G2553 microcontroller:

Input pin P1.2 is wired to a momentary push-button.

Output pin P1.0 drives an orange LED (D1).

Both the switch and LED share the same ground reference.

When the button causes an edge on P1.2, Timer_A captures the event and the interrupt service routine (ISR) toggles the LED—no polling or software debouncing required.

How It Works
Continuous Timer: Timer_A counts SMCLK indefinitely (MC_2).

Input Capture: Channel 1 (CCR1) latches the timer value on both rising and falling edges of P1.2 (CM_3).

Interrupt: Each capture sets CCIFG, routing execution to TIMER0_A1_ISR.

LED Toggle: The ISR flips P1.0, so every press and release of the switch changes the LED state.

Hardware Debounce: Because capture triggers on fast hardware edges, the LED may flicker if the switch bounces;
a real design can add an RC filter or change CM_3 to capture only one edge.
