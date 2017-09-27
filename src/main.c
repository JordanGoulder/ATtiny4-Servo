#include <avr/io.h>
#include <util/delay.h>

#define MIN_PULSE   (60)
#define MAX_PULSE   (250)
#define STEP        (1)

int main(void)
{
    /* Make Pin B0 (OC0A) an output */
    DDRB |= _BV(DDB0);

    /* 
     * Configure PWM
     * 
     * Clear OC0A on Compare Match, Set OC0A at BOTTOM
     * Fast PWM with TOP = ICR0
     * Clock select = Clkio / 8
     */
    TCCR0A = _BV(COM0A1) | _BV(WGM01);
    TCCR0B = _BV(WGM03) | _BV(WGM02) | _BV(CS01);

    /* 
     * Set TOP
     * 
     * Fpwm = Fclkio / (N * (1 + TOP))
     * TOP = (Fclkio / (Fpwm * N )) - 1
     * 
     * Fclkio = F_CPU Hz
     * Fpwm = 50 Hz
     * N = 8
     */
    ICR0 = 2499;

    /* Continuously sweep the servo between the min and max pulse */
    uint16_t pulse_width = MIN_PULSE;
    int8_t direction = 1;

    while (1) {
        OCR0A = pulse_width;

        pulse_width += STEP * direction;

        if (pulse_width > MAX_PULSE) {
            pulse_width = MAX_PULSE;
            direction = -1;
        }

        if (pulse_width < MIN_PULSE) {
            pulse_width = MIN_PULSE;
            direction = 1;
        }
    }

    return 0;
}

