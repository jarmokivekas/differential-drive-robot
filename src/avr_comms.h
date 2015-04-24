#ifndef AVR_COMMS_H
#define AVR_COMMS_H

#include <avr/interrupt.h>  // for interrupt service routine prototypes 
#include <util/setbaud.h>   // for definitons related to usart baud rates

// defines for initializing the ATmega USART peripheral
#define FOSC F_CPU // Clock Speed
#define BAUD 9600
#define COMMS_UBRR ((FOSC/(16UL *BAUD))-1)



struct command_message {
    /* unique identifier for the robot */
    uint16_t id,
    /* timer delay lengths for the motor speeds */
    uint16_t tick_period_right,
    uint16_t tick_period_left,
    /* bitmap to determine direction of the motors */
    char direction,
    /* xor of all the bytes so far in the message */
    char checksum
} __attribute__(packed);



void USART0_init(void);
void uart_putchar(char c);
char uart_getchar(void);

/* The prototype for ISR(USART_RX_vect) is located in avr/interrupt.h */



#endif /* end of include guard: AVR_COMMS_H */
