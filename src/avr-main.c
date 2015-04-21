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


/* pin mapping definition, so they are easily changed later if needed */

// stepper motor control pins. Naming scheme: PIN Stepper [Left|Right] [A|B])
#define PINSLA PD2
#define PINSLB PD4
#define PINSRA PD7
#define PINSRB PB4
// stepper motor  control ports
#define PORTSLA PORTD
#define PORTSLB PORTD
#define PORTSRA PORTD
#define PORTSRB PORTB
//serial radio enable
#define PINRADIO PB0






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
*/
void TIMER1_init(){
    /*
    The default timer configuration on startup is fairy close to what
    is needed for controlling the motor tick timing, not all setting need adjusting
    */
    
    /* Enable the timer by setting the clock prescaler */
    TCCR1B |= (1<<CS10); // prescaler value is 1
    /* That's it */
    
    /*
    XXX: access counter value in 16-bit register TCNT1 (read and write)
    XXX: OVerflow flag TOV1 is located in the Interrupt Flag Register TIFR1 
    */
}



/** 
 Interrupt service routine for the USART receive character  interrupt
 */
ISR(USART_RX_vect){
    cli();
    /* parse data here*/
    /* echo received character */
    uart_putchar(UDR0);
    sei();
}



/**
calculate how long between stepper tick for specifed velocity
The function relies on 
velo_angular: rotation speed of the motor, in degrees/second
*/
inline int tick_len_ms(float velo_angular){
    return 360.0/STEPPER_TICK_NUM/velo_angular;
}







// buffer will be cast to float pointers
char *velo_right_buffer;
char *velo_left_buffer;
/* signal to main program loop that there is data to be read */
char serial_data_availabe;
int main(int argc, char const *argv[]) {
    
    /* PB0 pin high enables radio comms using Xino-RF serial radio*/
    DDRB  |= (1<<PINRADIO);
    PORTB |= (1<<PINRADIO);
    
    /* Initialize usart0, used for radio comms to the controlling device */
    USART0_init();
    
    /* enable interrupts globally */
    sei();
    
    /*debugging code */
    DDRB = 0xff;
    float a = atof("1.23");
    _delay_ms(1000);
    if (a = 1.23){
        PORTB ^= 0xff; 
    }
    
    
    
    /* Initialize stepper motor control structs */
    
    struct stepper_state_machine right_stepper = {
        .phaseA_pin = PINSRA,
        .phaseB_pin = PINSRB,
        .phaseA_port = &PORTSRA,
        .phaseB_port = &PORTSRB,
        .state = 0
    };
    
    struct stepper_state_machine left_stepper = {
        .phaseA_pin = PINSLA,
        .phaseB_pin = PINSLB,
        .phaseA_port = &PORTSLA,
        .phaseB_port = &PORTSLB,
        .state = 0
    };
    
    /* store stepper struct pointers in array allow iteration with for loop */
    struct stepper_state_machine * motors[] = {&right_stepper, &left_stepper};
    /* holds data about the current desired state of the robot */
    struct dynamic_implement impl = {.velo_right = 0.0, .velo_left = 0.0};
    /* The number of timer cycles between stepper motor ticks */
    uint16_t motor_delay[2];
    uint16_t motor_tick_period[2];

    
    /*** Main program loop ***/    
    while (1) {
        
        
        /* alter delay length if there are new commands received */
        if (serial_data_availabe){
            // read the speed data
            // delay proper about of time, and then tick motor
            motor_tick_period[0] =(int) (1000 * STEPPER_TICK_LEN/impl.velo_right);
            motor_tick_period[1] =(int) (1000 * STEPPER_TICK_LEN/impl.velo_left);
        }
    
        
        /** Iterate over the motors and rotate one tick if necessary **/
        /* store a momentary counter value to avoid unexpected side-effects */
        int timer_value = STEPPER_TIME_COUNTER;
        int i;
        for (i = 0; i < 2; i++) {
            uint16_t time_remaining = motor_delay[i] -= timer_value;
            /* tick if we have waited long enough*/
            if ( (time_remaining <= 0) && (!TOV1) ){
                stepper_tick(motors[i], CLOCKWISE);
                motor_delay[i] = motor_tick_period[i];
            }
            
        }
        
        
        
        
        STEPPER_TIMER_DELAY |= delay_min;
        stepper_tick(&left_stepper, CLOCKWISE);
        
    }
    return 0;
}

