


// define static protocol data, uses legacy ascii for fun.
#define MSG_START 0x02      // ascii start of text (STX)
#define MSG_END 0x03        // ascii end of text   (ETX)


#define MOTOR_LEFT_PWM 10
#define MOTOR_RIGHT_PWM 11



struct internal_state {
    float wheel_velocity_left;
    float wheel velocity_right;
};

