#include "dynamical_model.h"


// translates state information from a unicycle model to a differential drive
// dynamical model
// design: pointer to state information to be traslated
// implement: pointer to where information is translated to
void dynamic_design_to_implement(struct dynamic_design_t *design, struct dynamic_implement_t *implement){
    float v_t = design->velo_translation;
    float v_o = design->velo_rotation;
    // see the documentaton for an explanation of these formulas
    implement->velo_right = (2*v_t + v_o*WHEEL_SPACING) / (2*WHEEL_RADIUS);
    implement->velo_left  = (2*v_t - v_o*WHEEL_SPACING) / (2*WHEEL_RADIUS);
    return;
}

// translates state information from a to a differential drive model to a
// unicycle dynamical model
// implement: pointer to state information to be traslated
// design: pointer to where information is translated to
void dynamic_implement_to_design(struct dynamic_implement_t *implement, struct dynamic_design_t *design){
    float v_r = implement->velo_right;
    float v_l = implement->velo_left;
    // see the documentaton for an explanation of these formulas
    design->velo_translation = (WHEEL_RADIUS/2) * (v_r + v_l);
    design->velo_rotation    = (WHEEL_RADIUS/WHEEL_SPACING) * (v_r - v_l);
    return;
}