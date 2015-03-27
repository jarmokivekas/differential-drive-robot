


// define static protocol data, uses legacy ascii for fun.
#define MSG_START 0x02      // ascii start of text (STX)
#define MSG_END 0x03        // ascii end of text   (ETX)


#define MOTOR_LEFT_PWM 10
#define MOTOR_RIGHT_PWM 11


// Used to describe the internal state of the robots' actuatort.
// wheel_velocity_X values measured in degrees per second.
typedef struct actuator_state_t {
    float wheel_velocity_left;
    float wheel velocity_right;
} actuator_state_t;

// Used to desctibe the state of the robots' sensors
// IR_distance_X values are read from 10-bit ADC channel
typedef struct sensor_state_t {
    uint16_t IR_distance_left;
    uint16_t IR_distance_right;
} sensor_state_t;

// Used for defining an i/o pin.
// <avr/io.h> defines PINX and PORTB as "volatile uint8_t *"
// store values with address of -operator, eg. {.pin = &P5, .port = &PORTB}
typedef struct pin_t {
    volatile uint8_t *pin;
    volatile uint8_t *port
} pin_t;


// Motor control pins are requred to be in the same PORTx so that
// assignments can be done simultaneously

typedef struct dc_motor_t {
    volatile uint8_t *pin1;
    volatile uint8_t *pin2;
    volatile uint8_t *port;
}