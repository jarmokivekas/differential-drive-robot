
// all physical length units are in millimeters

#define WHEEL_RADIUS 70.7
#define WHEEL_SPACING 200.0


// Used to describe the internal state of the robot's differential drive.
// velo_X: rotational velocity of the corresponding wheel
// values are measured in degrees per second.
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


void dynamic_design_to_implement(struct dynamic_design *design, struct dynamic_implement *implement);
void dynamic_implement_to_design(struct dynamic_implement *implement, struct dynamic_design *design);