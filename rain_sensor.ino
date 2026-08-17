#include <avr/io.h>

int main(void)
{
    unsigned int sensorData;
    unsigned char ledBrightness;

    /* A0 / ADC0 as input */
    DDRC &= ~(1 << PC0);

    /* D3 / OC2B as output */
    DDRD |= (1 << PD3);

    /* ADC initialization
       AVCC as reference
       ADC0 selected
    */
    ADMUX = (1 << REFS0);

    /* Enable ADC
       Prescaler = 128
    */
    ADCSRA = (1 << ADEN) |
             (1 << ADPS2) |
             (1 << ADPS1) |
             (1 << ADPS0);

    /* Timer2 Fast PWM
       D3 = OC2B
    */
    TCCR2A = (1 << COM2B1) |
             (1 << WGM21) |
             (1 << WGM20);

    /* Timer2 prescaler = 64 */
    TCCR2B = (1 << CS22);

    while (1)
    {
        /* Start ADC conversion */
        ADCSRA |= (1 << ADSC);

        /* Wait for conversion */
        while (ADCSRA & (1 << ADSC));

        /* Read ADC value */
        sensorData = ADC;

        /* Convert 0-1023 to 255-0 */
        ledBrightness =
            ((1023 - sensorData) * 255UL) / 1023;

        /* Set LED brightness */
        OCR2B = ledBrightness;
    }

    return 0;
}
