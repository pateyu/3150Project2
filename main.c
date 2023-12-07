/*
 * main.c
 *
 * Created: 11/13/2023 4:54:34 PM
 *  Author: Yug/Shreyas
 */ 

#define F_CPU 1000000UL

#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define DELAY_MS 1000
#define BUZZER_PIN PE4
#define LIGHT_UP_DELAY 5000 // Time in milliseconds to light up each LED
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

void WrongMark(void) {
	// Set PD0, PD2, PD5, PD7 as outputs
	uint8_t temp_PINE = PINE;
	uint8_t temp_PIND = PIND;
	DDRD |= (1 << PD0) | (1 << PD2) | (1 << PD5) | (1 << PD7);

	// Set PE5 as output
	DDRE |= (1 << PE5);

	// Turn on PD0, PD2, PD5, PD7, and PE4 (Active Low)
	PORTD &= ~((1 << PD0) | (1 << PD2) | (1 << PD5) | (1 << PD7));
	PORTE &= ~(1 << PE5);
	
	delay_ms(10000);
	
	PORTD |= ((1 << PD0) | (1 << PD2) | (1 << PD5) | (1 << PD7));
	PORTE |= (1 << PE5); 
	
	DDRE = temp_PINE;
	DDRD = temp_PIND;
	
}

void correctMark(void) {
	// Function to light up check mark
	uint8_t temp_PINE = PINE;
	uint8_t temp_PIND = PIND;
	DDRD |= (1 << PD3) | (1 << PD2) | (1 << PD5);
	DDRE |= (1 << PE5);
	
	PORTD &= ~((1 << PD3) | (1 << PD5) | (1 << PD2));
	PORTE &= ~(1<<PE5); //correct to middle mark
	
	delay_ms(10000);
	
	PORTD |= ((1 << PD3) | (1 << PD5) | (1 << PD2));
	PORTE |= (1<<PE5); //correct to middle mark
	
	DDRE = temp_PINE;
	DDRD = temp_PIND;
}
	
//Game-level 1 - Yug
bool handlePA3Press(void) {
	// Function to handle PA3 press - lights sequence and button press sequence
	uint8_t sequenceState = 0;
	PORTD &= ~(1 << PD0); // Light up PD0
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD0); // Turn off PD0

	PORTD &= ~(1 << PD1); // Light up PD1
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD1); // Turn off PD1

	PORTD &= ~(1 << PD2); // Light up PD2
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD2); // Turn off PD2

	// Wait for the sequence: PA0, PA1, PA0
	while(sequenceState < 3) {
		if(sequenceState == 0 && !(PINA & (1 << PA0))) {
			sequenceState++;
			/*
			while(!(PINA & (1 << PA0))) {
				delay_ms(DELAY_MS);
			}
			*/
		}
		else if(sequenceState == 0 && !(PINA & (1 << PA1)) | !(PINA & (1 << PA2))) {
			return false;
			/*
			while(!(PINA & (1 << PA0))) {
				delay_ms(DELAY_MS);
			}
			*/
		}

		else if (sequenceState == 1 && !(PINA & (1 << PA1))) {
			sequenceState++;
			/*
			while(!(PINA & (1 << PA1))) {
				delay_ms(DELAY_MS);
			}
			*/
		}

		else if (sequenceState == 2 && !(PINA & (1 << PA2))){
			sequenceState++; // Correct sequence entered
			/*
			while(!(PINA & (1 << PA2))) {
				delay_ms(DELAY_MS);
			}
			*/
			return true; //pass the test
			
		}
	}
	// Light up all LEDs after correct sequence
	//
	return false;
}

//Game-level-2-Parineeta
bool handlePA6Press(void)
{
	// Function to handle PA6 press - lights sequence and button press sequence
	uint8_t sequenceState = 0;
	
	PORTD &= ~(1 << PD0); // Light up PD0
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD0); // Turn off PD0

	PORTD &= ~(1 << PD1); // Light up PD1
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD1); // Turn off PD1

	PORTD &= ~(1 << PD2); // Light up PD2
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD2); // Turn off PD2

	PORTD &= ~(1 << PD4); // Light up PD5
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD4); // Turn off PD5

	PORTD &= ~(1 << PD7); // Light up PD8
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD7); // Turn off PD8

	PORTD &= ~(1 << PD6); // Light up PD7
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD6); // Turn off PD7

	PORTD &= ~(1 << PD5); // Light up PD6
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD5); // Turn off PD6

	PORTD &= ~(1 << PD3); // Light up PD3
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD3); // Turn off PD3

	// Wait for the sequence: PA0, PA1, PA2, PA5, PA8, PA7, PA6, PA3
	while(sequenceState < 8)
	{
		if(sequenceState == 0 && !(PINA & (1 << PA0)))
		{
			sequenceState++;
		}
		
		else if(sequenceState == 0 && !(PINA & (1 << PA1)) | !(PINA & (1 << PA2)) | !(PINA & (1 << PA5)) | !(PINA & (1 << PA7)) | !(PINA & (1 << PA6)) | !(PINA & (1 << PA3))){
			return false;
		}
				
		else if(sequenceState == 1 && !(PINA & (1 << PA1)))
		{
			sequenceState++;
		}

		else if(sequenceState == 2 && !(PINA & (1 << PA2)))
		{
			sequenceState++;
		}

		else if(sequenceState == 3 && !(PINA & (1 << PA4)))
		{
			sequenceState++;
		}

		else if(sequenceState == 4 && !(PINA & (1 << PA7)))
		{
			sequenceState++;
		}

		else if(sequenceState == 5 && !(PINA & (1 << PA6)))
		{
			sequenceState = 6;
		}

		else if(sequenceState == 6 && !(PINA & (1 << PA5)))
		{
			sequenceState++;
		}

		else if(sequenceState == 7 && !(PINA & (1 << PA3)))
		{
			sequenceState++;
			return true;
		}
		

	// Light up all LEDs after correct sequence
	//PORTD &= ~((1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD5) | (1 << PD8) | (1 << PD7) | (1 << PD6) | (1 << PD3));
	}
	return false;
}

bool handlePA7Press(void)
{
	// Function to handle PA7 press - lights sequence and button press sequence
	uint8_t sequenceState = 0;

	PORTD &= ~(1 << PD1); // Light up PD1
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD1); // Turn off PD1

	PORTD &= ~(1 << PD7); // Light up PD8
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD7); // Turn off PD8

	PORTD &= ~(1 << PD6); // Light up PD7
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD6); // Turn off PD7

	PORTD &= ~(1 << PD5); // Light up PD6
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD5); // Turn off PD6

	PORTD &= ~(1 << PD1); // Light up PD1
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD1); // Turn off PD1

	// Wait for the sequence: PA1, PA8, PA7, PA6, PA1
	while(sequenceState < 5)
	{
		if(sequenceState == 0 && !(PINA & (1 << PA1)))
		{
			sequenceState = 1;
			while(!(PINA & (1 << PA1)))
			{
				delay_ms(DELAY_MS);
			}
		}
		else if(sequenceState == 0 && !(PINA & (1 << PA2)) | !(PINA & (1 << PA7)) | !(PINA & (1 << PA6)) | !(PINA & (1 << PA5)))
		{
			return false;
		}
		else if(sequenceState == 1 && !(PINA & (1 << PA7)))
		{
			sequenceState = 2;
			while(!(PINA & (1 << PA7)))
			{
				delay_ms(DELAY_MS);
			}
		}

		else if(sequenceState == 2 && !(PINA & (1 << PA6)))
		{
			sequenceState = 3;
			while(!(PINA & (1 << PA6)))
			{
				delay_ms(DELAY_MS);
			}
		}

		else if(sequenceState == 3 && !(PINA & (1 << PA5)))
		{
			sequenceState = 4;
			while(!(PINA & (1 << PA5)))
			{
				delay_ms(DELAY_MS);
			}
		}

		else if(sequenceState == 4 && !(PINA & (1 << PA1)))
		{
			sequenceState = 5;
			return true;
			while(!(PINA & (1 << PA1)))
			{
				delay_ms(DELAY_MS);
			}
		}
	}

	// Light up all LEDs after correct sequence
	//PORTD &= ~((1 << PD1) | (1 << PD8) | (1 << PD7) | (1 << PD6) | (1 << PD1));
	return true;
}

bool handlePA5Press(void)
{
	// Function to handle PA8 press - lights sequence and button press sequence
	uint8_t sequenceState = 0;

	PORTD &= ~(1 << PD6); // Light up PD7
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD6); // Turn off PD7

	PORTD &= ~(1 << PD4); // Light up PD5
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD4); // Turn off PD5

	PORTD &= ~(1 << PD1); // Light up PD1
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD1); // Turn off PD1

	PORTD &= ~(1 << PD3); // Light up PD3
	delay_ms(LIGHT_UP_DELAY);
	PORTD |= (1 << PD3); // Turn off PD3

	// Wait for the sequence: PA7, PA5, PA1, PA3
	while(sequenceState != 4)
	{
		if(sequenceState == 0 && !(PINA & (1 << PA6)))
		{
			sequenceState = 1;
			while(!(PINA & (1 << PA6)))
			{
				delay_ms(DELAY_MS);
			}
		}
		
		else if(sequenceState == 0 && !(PINA & (1 << PA4)) | !(PINA & (1 << PA1)) | !(PINA & (1 << PA3)))
		{
			return false;
		}
		
		else if(sequenceState == 1 && !(PINA & (1 << PA4)))
		{
			sequenceState = 2;
			while(!(PINA & (1 << PA4)))
			{
				delay_ms(DELAY_MS);
			}
		}

		else if(sequenceState == 2 && !(PINA & (1 << PA1)))
		{
			sequenceState = 3;
			while(!(PINA & (1 << PA1)))
			{
				delay_ms(DELAY_MS);
			}
		}

		else if(sequenceState == 3 && !(PINA & (1 << PA3)))
		{
			sequenceState = 4;
			return true;
			while(!(PINA & (1 << PA3)))
			{
				delay_ms(DELAY_MS);
			}
		}
	}

	// Light up all LEDs after correct sequence
	//PORTD &= ~((1 << PD7) | (1 << PD5) | (1 << PD1) | (1 << PD1));
	return false;
}

int main(void) {
	DDRD = 0b00011111; // Set PD0 to PD4 as output for counter
	DDRE |= (1 << BUZZER_PIN); // Set PE4 as output for the buzzer
	PORTE &= ~(1 << BUZZER_PIN); // Initially turn off the buzzer

	PORTD = 0xFF;

	DDRA &= ~((1 << PA0) | (1 << PA1) | (1 << PA2) | (1 << PA3)) | (1 << PA4) | (1 << PA5) | (1 << PA6) | (1 << PA7); // Configure PA0, PA1, PA2, and PA3 as input
	PORTA |= (1 << PA0) | (1 << PA1) | (1 << PA2) | (1 << PA3)| (1 << PA4) | (1 << PA5) | (1 << PA6) | (1 << PA7); // Enable pull-up resistors

	uint8_t counter = 0;
	
	while (true) {
		// Up/Down Counter Logic
		if (!(PINA & (1 << PA0))) {
			counter++;
			PORTD = ~counter;

			if (counter > 31) {
				counter = 0; // Reset counter if it exceeds 31
				activateBuzzerMultipleTimes(); // Activate the buzzer 50 times
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
		if (!(PINA & (1 << PA3))) {
			bool passed = handlePA3Press(); // Call the function to handle PA3 press
			if (passed){
				correctMark();
				activateBuzzerMultipleTimes();
				
				/*
				PORTD &= ~((1 << PD0) | (1 << PD1) | (1 << PD2));
				delay_ms(LIGHT_UP_DELAY);
				PORTD |= ((1 << PD0) | (1 << PD1) | (1 << PD2));
				delay_ms(LIGHT_UP_DELAY);
				activateBuzzerMultipleTimes();
				PORTD &= ~((1 << PD0) | (1 << PD1) | (1 << PD2));
				delay_ms(LIGHT_UP_DELAY);
				PORTD |= ((1 << PD0) | (1 << PD1) | (1 << PD2));
				*/
			}
			else{
				//marcella's code (cross mark)
				WrongMark();
				activateBuzzerMultipleTimes();
				//continue;
			}
		}
		
		else if (!(PINA & (1 << PA6)))
		{
			bool passed_2 = handlePA6Press();
			if (passed_2){
				correctMark();
				activateBuzzerMultipleTimes();
			}
			else{
				WrongMark();
				activateBuzzerMultipleTimes();
			}
		}
		
		else if (!(PINA & (1 << PA7)))
		{
			bool passed_3 = handlePA7Press();
			if (passed_3){
				correctMark();
				activateBuzzerMultipleTimes();
			}
			else{
				WrongMark();
				activateBuzzerMultipleTimes();
			}
				
		}
		
		else if (!(PINA & (1 << PA5)))
		{
			bool passed_4 = handlePA5Press();
			if (passed_4){
				correctMark();
				activateBuzzerMultipleTimes();
			}
			else{
				WrongMark();
				activateBuzzerMultipleTimes();
			}
			
		}
	}
	return 0;
}
