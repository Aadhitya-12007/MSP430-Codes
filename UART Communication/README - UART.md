MSP430G2553-to-MSP430G2553: Half-Duplex UART Ping-Pong
This Proteus schematic shows two MSP430G2553 microcontrollers wired for asynchronous serial communication at 9,600 baud.

Left device (U1) transmits the character ‘A’, waits for a reply, and toggles its LED (D1) when it receives ‘B’.

Right device (U2) echoes ‘B’ whenever it receives ‘A’ and toggles its own LED (D2) when ‘B’ arrives.

Transmitter / Echo (U1)
Initializes USCI_A0 for 9,600 baud @1 MHz (calibrated DCO).

Main loop idles in LPM0; all activity is ISR-driven.

When a byte is received (USCIAB0RX_VECTOR) it checks for ‘A’:

Toggles its LED.

Immediately transmits ‘B’ back.

Sender (U2)
Also configures USCI_A0 at 9,600 baud.

Main loop continuously sends ‘A’ with a software delay.

RX ISR looks for the reply ‘B’ and toggles its LED.

Because both MCUs use identical pin mux (P1SEL | P1SEL2 on P1.1/P1.2) they can be swapped without code changes; only the main-loop logic differs.

Data Flow Sequence
U2’s loop places ‘A’ in its TX buffer.

Green trace (TX from U2) carries the frame to U1’s RX.

U1’s ISR sees ‘A’, flips D1, then writes ‘B’ to its TX buffer.

Return frame travels on U1’s TX line to U2’s RX.
U2’s ISR toggles D2; loop repeats.

This “ping-pong” test confirms correct clocking, baud-rate generation, and interrupt handling on both chips without a PC or level shifter.
