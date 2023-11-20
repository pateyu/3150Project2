/*
 * main.c
 *
 * Created: 11/13/2023 4:54:34 PM
 *  Author: Yug
 */ 


#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>

#define DELAY_MS 1000
#define BUZZER_PIN PE4 

void delay_ms(unsigned int ms) {
    while(ms--) {
        _delay_ms(1);
    }
}

int main(void) {
    DDRD = 0b00011111; // Set PD0 to PD4 as output for counter
    DDRE |= (1 << BUZZER_PIN); // Set PE4 as output for the buzzer
    PORTE &= ~(1 << BUZZER_PIN); // Initially turn off the buzzer

    PORTD = 0xFF;

    DDRA &= ~((1 << PA0) | (1 << PA1) | (1 << PA2)); // Configure PA0, PA1, and PA2 as input
    PORTA |= (1 << PA0) | (1 << PA1) | (1 << PA2); // Enable pull-up resistors

    uint8_t counter = 0;
    
    while (1) {
        if (!(PINA & (1 << PA0))) {
            counter++;
            PORTD = ~counter;

            if (counter > 31) {
                PORTE |= (1 << BUZZER_PIN); // Turn on buzzer
                delay_ms(DELAY_MS);
                PORTE &= ~(1 << BUZZER_PIN); // Turn off buzzer
                counter = 0; // Reset counter if it exceeds 31
            }

            delay_ms(DELAY_MS);
            while(!(PINA & (1 << PA0))) {
                delay_ms(DELAY_MS);
            }
        }

        if (!(PINA & (1 << PA1))) {
            counter--;
            PORTD = ~counter;

            if (counter == 255) { // Underflow condition (counter < 0)
                PORTE |= (1 << BUZZER_PIN); // Turn on buzzer
                delay_ms(DELAY_MS);
                PORTE &= ~(1 << BUZZER_PIN); // Turn off buzzer
                counter = 31; // Reset counter if it goes below 0
            }

            delay_ms(DELAY_MS);
            while(!(PINA & (1 << PA1))) {
                delay_ms(DELAY_MS);
            }
        }

        if (!(PINA & (1 << PA2))) { // Clear board button pressed
            counter = 0; // Reset the counter
            PORTD = ~counter; // Update the display

            delay_ms(DELAY_MS);
            while(!(PINA & (1 << PA2))) {
                delay_ms(DELAY_MS);
            }
        }
    }

    return 0;
}
