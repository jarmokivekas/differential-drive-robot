#include "stepper_ctl.h"



void stepper_tick(struct stepper_state_machine *mach, dir){
    mach->state = dir ? mach->next[mach->state] : mach->prev[mach->state];
    *(mach->phaseA_port) ^= ((0x01 & mach->state) << mach->phaseA_pin);
    *(mach->phaseB_port) ^= ((0x02 & mach->state) << mach->phaseB_pin);
}