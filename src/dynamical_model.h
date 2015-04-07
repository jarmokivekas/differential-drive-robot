#ifndef DYNAMICAL_MODEL_H
#define DYNAMICAL_MODEL_H





// all physical length units are in meters

#define WHEEL_RADIUS 0.0707
#define WHEEL_SPACING 0.2000

/*
Used to describe the internal state of the robot's differential drive.
values are measured in degrees per second.
velo_X: rotational velocity of the corresponding wheel
*/
struct dynamic_implement {
    float velo_right;
    float velo_left;
};

// Used to describes higher-level commands regarding movement.
// velo_tanslation: the robot's velo forward, meters per second
// velo_rotation: the robot's rate of turning, degrees per second
struct dynamic_design {
    float velo_translation;
    float velo_rotation;
};


/*
Despite being quite simple functions, argumets are given as structs because there
are several (2) return values
*/
void dynamic_design_to_implement(struct dynamic_design *design, struct dynamic_implement *implement);
void dynamic_implement_to_design(struct dynamic_implement *implement, struct dynamic_design *design);

#endif /* end of include guard: DYNAMICAL_MODEL_H */
