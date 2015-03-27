// Jarmo Kivekas
// Bippolar stepper motor control module for use with 3717 stepper
// control IC for Atmel AVR


// each stepper tick is 1.8 degrees, total of 200 ticks per rotation
#define TICK_LEN 1.8
#define TICKS_PER_ROT 200

// direction definitions used for stepper_tick() direction argument
#define CLOCKWISE 0
#define ANTI_CLOCKWISE 1

#define PORT_MOTOR PORTB
#define PHASE_0A 0
#define PHASE_1A 1
#define PHASE_0B 2
#define PHASE_1B 3

// Data structure that defines a stepper motor for general use
struct stepper_t {
    // phase pins connected to the control IC. e.g PB2, PD4
    const char phaseA_pin;
    const char phaseB_pin;
    // which port the relevant pin is located in e.g &PORTB, &PORTC
    volatile uint8_t *phaseA_port;
    volatile uint8_t *phaseB_port;
    // current state of the motor (in the tick loop).
    // the value is a 2-bit integer where phaseB_pin (msb) phaseA_pin (lsb) 
    // i/o status are the bit values
    char state;
};




// motor: pointer to struct stepper_t of the motor that will be controlled
void stepper_tick(stepper_t *motor, char direction);


