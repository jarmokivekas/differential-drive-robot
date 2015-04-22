#include <avr/io.h>
#include <util/delay.h>
#include "../src/stepper_ctl.h"

#define PLSA PB5
#define PLSB PB3

int main(void) {

    DDRB = 0xff;
    

    struct stepper_state_machine left_stepper = {
        .phaseA_pin = PB4,
        .phaseB_pin = PB5,
        .phaseA_port = &PORTB,
        .phaseB_port = &PORTB,
        .state = 0
    };
    
    for (;;) {
        stepper_tick(&left_stepper, CLOCKWISE);
        _delay_ms(200);
    }
    return 0;
}