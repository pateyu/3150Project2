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

void delay_ms(unsigned int ms) {
	while(ms--) {
		_delay_ms(1);
	}
}

int main(void) {
	
	DDRD = 0b00011111;
	

	
	uint8_t counter = 0;
	PORTD = 0xFF;  

	
	DDRA &= ~((1 << PA0) | (1 << PA2));
	PORTA |= (1 << PA0) | (1 << PA2);

	while (1) {
		
		if (!(PINA & (1 << PA0))) {
			
			counter++;

		
			PORTD = ~counter;

		
			delay_ms(DELAY_MS);

			while(!(PINA & (1 << PA0))) {
				delay_ms(DELAY_MS); 
			}
		}

		if (!(PINA & (1 << PA2))) {
			counter--;

			PORTD = ~counter;

	
			delay_ms(DELAY_MS);

		
			while(!(PINA & (1 << PA2))) {
				delay_ms(DELAY_MS); 
			}
		}
	}

	return 0;
}
