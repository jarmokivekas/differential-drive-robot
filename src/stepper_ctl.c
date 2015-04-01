#include "stepper_ctl.h"


/**
This function was initially implemented usgin switch/case conditional logic.
Implementing it as a state machine has the advantage of it being smaller 
in size, and takes a constant amount of time to execute
 */
void stepper_tick(struct stepper_state_machine *mach, char direction){
    /* 
    Define jump tables which specify in which order the states need to occur in.
    Declared as static to prevent reallocation of memory at each function call.
    */
    static const char next[4] = {1,3,0,2};
    static const char prev[4] = {2,0,3,1};
    /* 
    Transfer to the succeeding state
    */
    mach->state = direction ? next[mach->state] : prev[mach->state];
    /* 
    The .state field serves as a bitfield for the i/o pin statuses required 
    by the stepper driver IC.
    
    */
    if (PHASE_A_MASK & mach->state) {
        *(mach->phaseA_port) |=  (1 << mach->phaseA_pin);
    } else {
        *(mach->phaseA_port) &= ~(1 << mach->phaseA_pin);
    }
    
    if (PHASE_B_MASK & mach->state) {
        *(mach->phaseB_port) |=  (1 << mach->phaseB_pin);
    } else {
        *(mach->phaseB_port) &= ~(1 << mach->phaseB_pin);
    }
}