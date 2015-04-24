#include <avr/io.h>         // Special function register definitions, etc
#include <util/delay.h>     // for _delay_ms()

#include <stdlib.h>
#include <stdint.h>
#include <string.h>         // for memcpy()

#include "./dynamical_model.h"
#include "./stepper_ctl.h"
#include "./avr_comms.h"



/* This array is modified inside the USART receive interrupt, declare volatile*/
/* this will be accessed as extern from avr_comms.c */
volatile uint16_t GLOBAL_motor_tick_period[2] = {1000, 1000};
volatile char GLOBAL_motor_direction = 0x00;


/**
Initialize 16-bit timer1 for controlling the motor tick timings
The timer is used in 'normal mode' as a simple counter clock.
XXX: access counter value in 16-bit register TCNT1 (read and write)
XXX: Timer OVerflow flag TOV1 is located in the Interrupt Flag Register TIFR1 
*/
void TIMER1_init(){    
    /*
    Enable the timer by setting the clock prescaler.
    The timer is used as a global clock, overflows every 4.2 seconds.
    The lenght of a single counter tick is 64 micro seconds.
     */
    TCCR1B |= (1<<CS12)|(1<<CS10);
    /* That's it */
}






int main(int argc, char const *argv[]) {
    
    /* PB0 pin high enables radio comms using Xino-RF serial radio*/
    DDRB  |= (1<<PB0);
    PORTB |= (1<<PB0);

    /* set stepper control pins as ouputs */
    DDRD |= (1<<PD2)|(1<<PD4)|(1<<PD7);
    DDRB |= (1<<PB4);

    /* Initialize hardware peripherals */
    TIMER1_init();    
    USART0_init();
    
    /* Initialize stepper motor control structs */
    struct stepper_state_machine left_stepper = {
        .phaseA_pin = PD2,
        .phaseB_pin = PD4,
        .phaseA_port = &PORTD,
        .phaseB_port = &PORTD,
        .state = 0
    };
    
    struct stepper_state_machine right_stepper = {
        .phaseA_pin = PD7,
        .phaseB_pin = PB4,
        .phaseA_port = &PORTD,
        .phaseB_port = &PORTB,
        .state = 0
    };
    
    /* store stepper struct pointers in array allow iteration with for loop */
    struct stepper_state_machine * motors[] = {&right_stepper, &left_stepper};
    /* The number of timer cycles between stepper motor ticks */
    uint16_t motor_delay[2] = {100,100};

    /* enable global interrupts for USART receive interrupt */
    sei();
    /*** Main program loop ***/
    while (1) {
        
        
        /** Iterate over the motors and rotate one tick if necessary **/
        /* store a momentary counter value to avoid unexpected side-effects */
        uint16_t timer_value = TCNT1;
        char overflow = TIFR1 & (1<<TOV1);
        /* clear the overflow flag*/
        TIFR1 |= (1<<TOV1);
        unsigned char i;
        for (i = 0; i < 2; i++) {
            int32_t time_remaining = motor_delay[i] - (timer_value + 0xffff*overflow);
            /* tick if we have waited long enough or there has been a timer overflow */
            if (time_remaining <= 0){
                /* TODO: get the right direction */
                stepper_tick(motors[i], CLOCKWISE);
                motor_delay[i] += GLOBAL_motor_tick_period[i];
            }
        }
        
        
    }
    return 0;
}

