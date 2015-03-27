#include <avr/io.h>
#include <util/delay.h>
#include <serial.h>

#define F_CPU 16000000L

// defines for initializing the ATmega USART peripheral
// Baudrate 115200 in Asynchronous normal mode
#define FOSC 16000000L // Clock Speed
#define BAUD 115200
#define COMMS_UBRR (FOSC/16/BAUD-1)

// ubrr: Usart Baud Rate Register
void USART_Init( unsigned int ubrr){
    // Set baud rate
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    // enable usart receiver and transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
// set frame format, 8 data, 2 stop bit
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

int main(int argc, char const *argv[]) {
    DDRB = 0xff;

    while (1) {
        struct robot_commands_t commands;
        
        comms_recv_message(&commads);
        comms_parse_state(message, &commands);
        
        dynamics_command_to_state();
        velocity_right

        PORTB ^= (1 << PB4);
        _delay_ms(1000);

    }
    return 0;
}