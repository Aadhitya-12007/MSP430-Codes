// Code by Aadhitya G
// ADC using temperature sensor
#include <msp430g2553.h>

#define LED1 BIT1          // P1.1 for 20-40°C range
#define LED2 BIT2          // P1.2 for 50-80°C range
#define TEMP_SENSOR BIT0   // P1.0 (A0) for LM35 input

volatile unsigned int adc_value;
volatile float temperature;
volatile int temp_celsius;

void ADC_init(void) {
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE; // VCC/VSS ref, sample time, ADC on, interrupt enable
    ADC10CTL1 = INCH_0 + ADC10DIV_3;                     // Input channel A0, ADC10CLK/4
    ADC10AE0 |= TEMP_SENSOR;                              // Enable analog input on P1.0
}

void GPIO_init(void) {
    P1DIR |= LED1 + LED2;                                 // Set LED pins as output
    P1OUT &= ~(LED1 + LED2);                              // Turn off both LEDs initially
    P1DIR &= ~TEMP_SENSOR;                                // Set P1.0 as input for temperature sensor
}

unsigned int read_temperature(void) {
    ADC10CTL0 |= ENC + ADC10SC;                           // Enable conversion and start
    __bis_SR_register(LPM0_bits + GIE);                   // Enter LPM0, wait for conversion
    return ADC10MEM;                                      // Return ADC result
}

float convert_to_celsius(unsigned int adc_val) {
    // LM35: 10mV per °C, ADC reference = 3.3V, 10-bit ADC (1024 steps)
    // Voltage = (adc_val * 3.3V) / 1024
    // Temperature = Voltage / 0.01V per °C
    float voltage = ((float)adc_val * 3.3) / 1024.0;     // Convert ADC to voltage
    float temp = voltage / 0.01;                          // Convert voltage to temperature (°C)
    return temp;
}

void control_leds(int temp) {
    // Turn off both LEDs first
    P1OUT &= ~(LED1 + LED2);
    
    if(temp >= 20 && temp <= 40) {
        P1OUT |= LED1;                                    // Turn on LED1 for 20-40°C
    }
    else if(temp >= 50 && temp <= 80) {
        P1OUT |= LED2;                                    // Turn on LED2 for 50-80°C
    }
    // If temperature is outside both ranges, both LEDs remain off
}

void delay_ms(unsigned int ms) {
    volatile unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 1000; j++);
    }
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
    __bic_SR_register_on_exit(LPM0_bits);                 // Exit LPM0 when conversion complete
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                             // Stop watchdog timer
    
    // Clock configuration
    BCSCTL1 = CALBC1_1MHZ;                                // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ;
    
    GPIO_init();                                          // Initialize GPIO
    ADC_init();                                           // Initialize ADC
    __bis_SR_register(GIE);                               // Enable global interrupts
    
    while(1) {
        adc_value = read_temperature();                   // Read ADC value
        temperature = convert_to_celsius(adc_value);      // Convert to Celsius
        temp_celsius = (int)temperature;                  // Convert to integer
        
        control_leds(temp_celsius);                       // Control LEDs based on temperature
        
        delay_ms(1000);                                   // Wait 1 second before next reading
    }
}
