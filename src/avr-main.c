#define BAUD 9600
#include <avr/io.h>         // Special function register definitions, etc
#include <avr/interrupt.h>  // for interrupt service routine prototypes 
#include <util/setbaud.h>   // for definions related to usart baud rates
#include <util/delay.h>     // for _delay_ms()

#include <stdlib.h>
#include <stdint.h>
#include <string.h>         // for memcpy()

#include "./dynamical_model.h"
#include "./stepper_ctl.h"

// defines for initializing the ATmega USART peripheral
// Baudrate 115200 in Asynchronous normal mode
#define FOSC F_CPU // Clock Speed
#define COMMS_UBRR ((FOSC/(16UL *BAUD))-1)


/* This array is modified inside the USART receive interrupt, declare volatile*/
volatile uint16_t motor_tick_period[2] = {1000, 1000};



void uart_putchar(char c) {
    /* Busy-wait until data register empty. */
    while ((UCSR0A & (1<<UDRE0)) == 0){};
    UDR0 = c;
}

char uart_getchar(void) {
    /* Wait until data is present in the buffer */
    while ((UCSR0A & (1<<RXC0))  == 0){};
    return UDR0;
}





/*
UBRR: Usart Baud Rate Register
Initalize Usast periheprel for radio comms
*/
void USART0_init(){
    // Set baud rate
    UBRR0H = (unsigned char)(COMMS_UBRR>>8);
    UBRR0L = (unsigned char)COMMS_UBRR;
    // enable usart receiver and transmitter
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
    
    // enable the receive complete interrupt
    UCSR0B |= (1<<RXCIE0);
    
    // usart mode select UMSEL0[0,1] = 0 (default asychronouse io mode)
    // usart parity bit mode UMP0[0,1] = (default no parity bit mode)
    
    // set frame format to use 1 stop bit
    UCSR0C |= (0<<USBS0);
    // set frame format to 8-bit character size
    UCSR0C |= (0<<UCSZ02)|(1<<UCSZ01)|(1<<UCSZ00);
}



/**
Initialize 16-bit timer1 for controlling the motor tick timings
The timer is used in 'normal mode' as a simple counter clock.
XXX: access counter value in 16-bit register TCNT1 (read and write)
XXX: Timer OVerflow flag TOV1 is located in the Interrupt Flag Register TIFR1 
*/
void TIMER1_init(){    
    /* Enable the timer by setting the clock prescaler */
    TCCR1B |= (1<<CS12)|(1<<CS10); // prescaler value is 1024
    /* That's it */
}



/** 
 Interrupt service routine for the USART receive character  interrupt
 */
ISR(USART_RX_vect){
    cli();
    /* static variable will be initialized only once*/
    static uint16_t data_buffer[2];
    static char data_idx = 5;
    /** store command values, convert data to uint16_t **/
    if (data_idx <= 3){
        data_buffer[data_idx/2] += UDR0 << (8 * ((data_idx)%2));
        data_idx++;
        uart_putchar('A' + data_idx);
    }
    
    /* new if statement to allow fall-through */
    if( data_idx == 4 ){
        motor_tick_period[0] = data_buffer[0];
        motor_tick_period[1] = data_buffer[1];
        data_idx++;
    }
    else if(data_idx >= 5){
        /* expect start of tranmission, clear data buffer*/
        if (UDR0 == 'a'){
            data_buffer[0] = 0;
            data_buffer[1] = 0;
            data_idx = 0;
            uart_putchar('a');
        }
        /* malformed data */
        else{
            uart_putchar('e');
        }
    }
    
    sei();
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
        char i;
        for (i = 0; i < 2; i++) {
            int32_t time_remaining = motor_delay[i] - (timer_value + 0xffff*overflow);
            /* tick if we have waited long enough or there has been a timer overflow */
            if (time_remaining <= 0){
                /* TODO: get the right direction */
                stepper_tick(motors[i], CLOCKWISE);
                motor_delay[i] += motor_tick_period[i];
            }
        }
        
        
    }
    return 0;
}

