
#include <util/delay.h>
#include <avr/io.h>
#include <math.h>




// initialize avr ATmega326 timer0
// used for PWM speed control of brushed DC motor
void init_timer0(){
		
	TCCR0 |= (0<<CS02)|(0<<CS01)|(1<<CS00);		// no clock prescaler, only enable
}

// these were done using the atmega8 datasheet... TOU!
void init_timer(){
	// Timer 1 setup for PWM speed control of DC brushed motors.
	
	// set i/o pins conneted to timer as ouputs
	DDRB |= (1<<PB1)|(1<<PB2);
	TCCR1A |= 0x00;									// clear control register A to known state
	TCCR1B |= 0x00;									// clear control register B to known state

	// set timer counter registers to be cleared when reaching TOP (0xff)
	TCCR1A |= (0<<WGM13)|(1<<WGM12)|(0<<WGM11)|(1<<WGM10);
	TCCR1B |= (0<<WGM13)|(1<<WGM12)|(0<<WGM11)|(1<<WGM10);

	// use timers for pwm, toggle output pin at compare match
	TCCR1A |= (1<<COM1A1)|(0<<COM1A0);				// clear pin on compare match, set pin at BOTTOM (PB1/OC1A)
	TCCR1A |= (1<<COM1B1)|(0<<COM1B0);				// clear pin on compare match, set pin at BOTTOM (PB2/OC1B)

	TCCR1B |= (0<<CS12)|(0<<CS11)|(1<<CS10);		// no clock prescaler, only enable
}

int main(){
	init_timer();
	int i;
	for (i = 0; ; ++i){
		OCR1AL += 1					// lower output compare register for timer 1A
		_delay_ms(100);				// delay until next sample is needed
	}	
	return 0;
}

