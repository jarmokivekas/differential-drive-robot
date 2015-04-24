#include <arv/io.h>         // SFR definitions
#include <avr/interrupt.h>  // for interrupt service routine prototypes 
#include "./avr_comms.h"






void USART0_putchar(char c) {
    /* Busy-wait until data register empty. */
    while ((UCSR0A & (1<<UDRE0)) == 0){};
    UDR0 = c;
}

char USART0_getchar(void) {
    /* Wait until data is present in the buffer */
    while ((UCSR0A & (1<<RXC0))  == 0){};
    return UDR0;
}





/*
UBRR: Usart Baud Rate Register
Initalize USART peripheral for radio comms
*/
void USART0_init(){
    // Set baud rate
    UBRR0H = (unsigned char)(COMMS_UBRR>>8);
    UBRR0L = (unsigned char)COMMS_UBRR;
    // enable usart receiver and transmitter
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
    
    // enable the receive complete interrupt
    UCSR0B |= (1<<RXCIE0);
    
    // usart mode select UMSEL0[0,1] = 0   (default asychronouse io mode)
    // usart parity bit mode UMP0[0,1] = 0 (default no parity bit mode)
    
    // set frame format to use 1 stop bit
    UCSR0C |= (0<<USBS0);
    // set frame format to 8-bit character size
    UCSR0C |= (0<<UCSZ02)|(1<<UCSZ01)|(1<<UCSZ00);
}






/** 
 Interrupt service routine for the USART receive character  interrupt
 */
ISR(USART_RX_vect){
    cli();
    /* static variable will be allocated only once*/
    static char byte_idx = 8;
    static struct command_message msg_struct;
    static char *msg_array = &msg_struct;

    /** store id, speeds and direction **/
    if (byte_idx <= 6){
        msg_array[byte_idx] = UDR0;
        msg_struct.checksum ^= UDR0;
        byte_idx++;
    }
    
    /* simple integrity check to prevent accepting non-intended data */
    else if (byte_idx == 7){
        /* use command data if the checksum is correct */
        if (UDR0 == msg_struct.checksum) {
            GLOBAL_motor_direction      = msg_struct.direction;
            GLOBAL_motor_tick_period[0] = msg_struct.tick_period_right;
            GLOBAL_motor_tick_period[1] = msg_struct.tick_period_left;
        }
        else{USART0_putchar('f');} /* malformed data*/
        byte_idx++;
    }
    
    /* wait for transmission to start */
    else if (byte_idx >= 8){
        if (UDR0 == 'a'){
            msg_struct.checksum = 0;
            byte_idx = 0;
            USART0_putchar('a');
        }
        else{USART0_putchar('e');} /* malformed or non relevant data */
    }
    
    sei();
}
