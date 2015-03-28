
// Used to desctibe the state of the robots' sensors
// IR_distance_X values are read from 10-bit ADC channel
struct sensor_state_t {
    uint16_t IR_distance_left;
    uint16_t IR_distance_right;
};

