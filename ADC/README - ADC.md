This project implements temperature-based LED indication using an MSP430G2553 microcontroller, an LM35 temperature sensor, and two LEDs. The microcontroller reads analog temperature data from the LM35 sensor via its ADC10 module, processes the value to determine actual temperature in °C, and controls two LEDs depending on the measured temperature range.

Circuit Explanation
Referencing the attached Proteus image:

LM35 Temperature Sensor (U2):

VOUT (Pin 2 of U2) is connected to P1.0 (Pin 1 of U1) on the MSP430G2553. This provides the analog voltage proportional to temperature (10mV/°C) as input to the microcontroller's ADC channel A0.

LEDs (D1 and D2):

D1 (LED-BLUE) is controlled by P1.1 (Pin 2 of U1) and D2 (LED-BLUE) by P1.2 (Pin 3 of U1). Each LED's cathode is connected to ground. Their anodes connect to the respective GPIO pins on the MCU, ideally through current-limiting resistors (not shown, but recommended).

Power and Ground:

Both the LM35 and MSP430G2553 share common VCC/AVCC and GND/AVSS connections to a 3.3V supply.

Code Functionality
ADC Initialization (ADC_init): Configures the ADC10 to use channel A0 (P1.0) with interrupt enabled for conversion completion.

GPIO Initialization (GPIO_init): Sets P1.1 and P1.2 as outputs (for LEDs), and P1.0 as input (for LM35 data).

Temperature Reading (read_temperature): Initiates ADC conversion and waits for completion, then returns the 10-bit digital value.

Conversion to Celsius (convert_to_celsius): Converts the ADC value to a voltage, then to a temperature in Celsius:

Uses the formula:

Voltage
=
ADC
×
3.3
1024
Voltage= 
1024
ADC×3.3
 
Temperature (°C)
=
Voltage
0.01
Temperature (°C)= 
0.01
Voltage
 
This matches the LM35 output (10mV/°C).

LED Control (control_leds):

If temperature is between 20°C and 40°C, LED1 (P1.1) is turned ON.

If temperature is between 50°C and 80°C, LED2 (P1.2) is turned ON.

For all other temperatures, both LEDs remain OFF.

Continuous Operation (main):

The microcontroller repeatedly reads the temperature, processes the value, drives the LEDs appropriately, and waits for 1 second before the next reading.

How It Works in Proteus Simulation
Set the LM35 “Temperature” property: Changing this value in Proteus will vary the output voltage.

Observe LEDs:

If temperature is set between 20°C and 40°C, D1 (connected to P1.1) should glow.

If temperature is between 50°C and 80°C, D2 (connected to P1.2) should glow.

In all other cases, both D1 and D2 remain off.

Real-Time Response: The system updates LED status every second based on the current measured temperature.

Key Points
Analog-to-Digital Conversion: The MSP430G2553 converts the LM35’s analog output to a digital value, which is mathematically translated to Celsius.

LED Indication: Two discrete indicators (LEDs) provide immediate feedback for specific temperature ranges.
