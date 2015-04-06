#define BAUD 9600
#define F_CPU 16000000L
// Special function register definitions, etc
#include <avr/io.h>
// for interrupt service routine prototypes 
#include <avr/interrupt.h>
// for definions related to usart baud rates
#include <util/setbaud.h>
// for _delay_ms()
#include <util/delay.h>

#include <stdlib.h>
#include <stdint.h>

// defines for initializing the ATmega USART peripheral
// Baudrate 115200 in Asynchronous normal mode
#define FOSC 16000000L // Clock Speed
#define COMMS_UBRR ((FOSC/(16UL *BAUD))-1)


void uart_putchar(char c) {
    /* Wait until data register empty. */
    while ((UCSR0A & (1<<UDRE0)) == 0){};
    UDR0 = c;
}

char uart_getchar(void) {
    /* Wait until data is present in the buffer */
    while ((UCSR0A & (1<<RXC0))  == 0){};
    return UDR0;
}


// ubrr: Usart Baud Rate Register
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

float a;
char got_float;
int main(int argc, char const *argv[]) {
    /* enable interrupts globally */
    sei();
    /* Initialize usart0, used for radio comms to the controlling device */
    USART0_init();
    DDRB = 0xff;
    
    a = atof("1.23");
    _delay_ms(1000);
    if (a = 1.23){
        PORTB ^= 0xff; 
    }
    _delay_ms(1000);
    

    _delay_ms(1);
    UDR0 = 55;
    char received_byte = 'A';
    while (1) {
        PORTB ^= (1<<PB5);
        _delay_ms(100);
        if (got_float){
            ;
        }
        /*
        delay until data has been received and is ready to be read
        */
        //while ((UCSR0A & (1 << RXC0)) == 0){};
        
    //    received_byte = UDR0;
        /* 
        delay until the serial data register is ready to be written to
        */
        //while ((UCSR0A & (1 << UDRE0)) == 0){};
        
        /* transimt the received byte back to sender */
    //    UDR0 = 54;
    }
    return 0;
}


/** 
 Interrupt service routine for the USART receive character  interrupt
 */
ISR(USART_RX_vect){
    cli();
    static 
    PORTB ^= 0xff;
    char rec;
    rec = UDR0;
    
    UDR0 = ++rec;
    UDR0 = 'a';
    while ((UCSR0A & (1 << UDRE0)) == 0){};
    
    UDR0 = 'b';
    sei();
}