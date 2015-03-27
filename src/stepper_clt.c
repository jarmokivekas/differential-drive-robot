


// PBL 3717 stepper motor driver interface.
// moves motor one single step in the specified direction
// motor: specifies the motor to rotate, struct defined in stepper_ctl.h
// direction: specifies direciton of motor, #defines in stepper_ctl.h

void stepper_tick(stepper_t *motor, char direction){

    if (direction == CLOCKWISE) {
        switch (*motor.state) {
            case 0:
                *motor.phaseA_port |=  (1 << *motor.phaseA_pin);
                *motor.state = 1;
                break;
            case 1:
                *motor.phaseB_port |=  (1 << *motor.phaseB_pin);
                *motor.state = 3;
                break;
            case 2:
                *motor.phaseB_port &= ~(1 << *motor.phaseB_pin);
                *motor.state = 0;
                break;
            case 3:
                *motor.phaseA_port &= ~(1 << *motor.phase0_pin);
                *motor.state = 2;
                break;
            default:
                // set motor to a known state, this should never happen
                *motor.phaseA_port &= ~(1 << *motor.phaseA_pin);
                *motor.phaseB_port &= ~(1 << *motor.phaseB_pin);
                *motor.state = 0;
                break;
        }
    }
    
    else /*(direction == ANTI_CLOCKWISE)*/ {
        switch (*motor.state) {
            case 1:
                *motor.phaseA_port &= ~(1 << *motor.phaseA_pin);
                *motor.state = 0;
                break;
            case 3:
                *motor.phaseB_port &= ~(1 << *motor.phaseB_pin);
                *motor.state = 1;
                break;
            case 0:
                *motor.phaseB_port |=  (1 << *motor.phaseB_pin);
                *motor.state = 2;
                break;
            case 2:
                *motor.phaseA_port |=  (1 << *motor.phase0_pin);
                *motor.state = 3;
                break;
            default:
                // set motor to a known state, this should never happen
                *motor.phaseA_port &= ~(1 << *motor.phaseA_pin);
                *motor.phaseB_port &= ~(1 << *motor.phaseB_pin);
                *motor.state = 0;
                break;
        }
    }
}