/*
 * main.c
 *
 * Created: 11/13/2023 4:54:34 PM
 *  Author: Shreyas
 */ 

#define F_CPU 1000000UL

#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>


#define DELAY_MS 1000
#define BUZZER_PIN PE4
#define LIGHT_UP_DELAY 500 // Time in milliseconds to light up each LED
#define BUZZER_ON_TIME 50 // Time in milliseconds the buzzer is on
#define BUZZER_OFF_TIME 50 // Time in milliseconds the buzzer is off
#define BUZZER_REPEATS 60 // Number of times the buzzer will be activated

void delay_ms(unsigned int ms) {
	while(ms--) {
		_delay_ms(1);
	}
}

void activateBuzzerMultipleTimes() {
	for (int i = 0; i < BUZZER_REPEATS; i++) {
		PORTE |= (1 << BUZZER_PIN); // Turn on buzzer
		delay_ms(BUZZER_ON_TIME); // Buzzer on duration
		PORTE &= ~(1 << BUZZER_PIN); // Turn off buzzer
		delay_ms(BUZZER_OFF_TIME); // Buzzer off duration
	}
}

bool handlePA4Press(void) {
	bool correct = false;
	// Function to handle PA3 press - lights sequence and button press sequence
	uint8_t sequenceState = 0;

	PORTD &= ~(1 << PD4); // Light up PD0
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD4); // Turn off PD0

	PORTD &= ~(1 << PE5); // Light up PD1
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PE5); // Turn off PD1

	PORTD &= ~(1 << PD3); // Light up PD2
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD3); // Turn off PD2

	// Wait for the sequence: PA0, PA1, PA0
	while(sequenceState != 3) {
		if(sequenceState == 0 && !(PINA & (1 << PA4))) {
			sequenceState = 1;
			while(!(PINA & (1 << PA4))) {
				delay_ms(DELAY_MS);
			}
		}

		if(sequenceState == 1 && !(PINA & (1 << PA5))) {
			sequenceState = 2;
			while(!(PINA & (1 << PE6))) {
				delay_ms(DELAY_MS);
			}
		}

		if(sequenceState == 2 && !(PINA & (1 << PA3))) {
			sequenceState = 3; // Correct sequence entered
			correct = true;
			while(!(PINA & (1 << PA6))) {
				delay_ms(DELAY_MS);
			}
		}
	}

	// Light up all LEDs after correct sequence
	//
	return correct;
	
}

int main(void) {
	DDRD = 0b00011111; // Set PD0 to PD4 as output for counter
	DDRE |= (1 << BUZZER_PIN); // Set PE4 as output for the buzzer
	PORTE &= ~(1 << BUZZER_PIN); // Initially turn off the buzzer

	PORTD = 0xFF;

	DDRA &= ~((1 << PA0) | (1 << PA1) | (1 << PA2) | (1 << PA4)); // Configure PA0, PA1, PA2, and PA4 as input
	PORTA |= (1 << PA0) | (1 << PA1) | (1 << PA2) | (1 << PA4); // Enable pull-up resistors

	uint8_t counter = 0;
	
	while (1) {
		// Up/Down Counter Logic
		if (!(PINA & (1 << PA0))) {
			counter++;
			PORTD = ~counter;

			if (counter > 31) {
				activateBuzzerMultipleTimes(); // Activate the buzzer 50 times
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
				activateBuzzerMultipleTimes(); // Activate the buzzer 50 times
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
			PORTD |= ((1 << PD0) | (1 << PD1) | (1 << PD2)); // Turn off all LEDs

			delay_ms(DELAY_MS);
			while(!(PINA & (1 << PA2))) {
				delay_ms(DELAY_MS);
			}
		}

		// PA3 Press Logic
		if (!(PINA & (1 << PA4))) {
			if (handlePA4Press()== true){ // Call the function to handle PA3 press
			/*while(!(PINA & (1 << PA4))) {
				delay_ms(DELAY_MS);
				}*/
			PORTD &= ~((1 << PD0) | (1 << PD1) | (1 << PD2));
			}
		}
	}
	return 0;
}
