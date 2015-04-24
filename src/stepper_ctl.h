#include <stdint.h>

/** 
@file Bipolar stepper motor control module for use with PL3717 stepper
control IC for Atmel AVR
 */


#ifndef STEPPER_CTL_H
#define STEPPER_CTL_H

// the amount of tick per motor rotation
#define STEPPER_TICK_NUM 200.0
// the amount of rotation in each tick
#define STEPPER_TICK_LEN 360.0/STEPPER_TICK_NUM

// direction definitions used for stepper_tick() direction argument
#define CLOCKWISE 0
#define ANTI_CLOCKWISE 1


#define PHASE_A_MASK 0x01
#define PHASE_B_MASK 0x02

/**
Data structure that defines a finite state machine used for controlling
a stepper motor
*/
struct stepper_state_machine {
    // phase pins connected to the control IC. e.g PB2, PD4
    uint8_t phaseA_pin;
    uint8_t phaseB_pin;
    // which port the relevant pin is located in e.g &PORTB, &PORTC
    volatile uint8_t *phaseA_port;
    volatile uint8_t *phaseB_port;
    // current state of the motor (in the tick loop).
    // the value is a 2-bit integer where phaseB_pin (msb) phaseA_pin (lsb) 
    // i/o status are the bit values
    uint8_t state;
};



/** 
PBL 3717 stepper motor driver interface.
moves motor one single step in the specified direction
@param mach: pointer to struct that specifies the motor state machine to rotate
@param direction: specifies direciton of motor tick, either CLOCKWISE or ANTI_CLOCKWISE
*/
void stepper_tick(struct stepper_state_machine *mach, char direction);


#endif /* end of include guard: STEPPER_CTL_H */
