//dynamical_model_test.c
#include <stdio.h>
#include "../src/dynamical_model.h"


int main(int argc, char const *argv[]) {
    struct dynamic_design des = {0.0, 0.0};
    struct dynamic_implement impl = {1.0, 1.0};
    
    printf("Mechanical characteristics:\nwheel radius:\t%g\nwheel spacing:\t%g\n",WHEEL_RADIUS, WHEEL_SPACING );
    printf("### impl_to_design:\n");
    
    dynamic_implement_to_design(&impl, &des);
    printf("straight forward:\ntranslation:\t%g\nrotation:\t%g\n\n", des.velo_translation, des.velo_rotation);

    impl.velo_right = -1.0;
    dynamic_implement_to_design(&impl, &des);
    printf("o-turn clockwise:\ntranslation:\t%g\nrotation:\t%g\n\n", des.velo_translation, des.velo_rotation);

    impl.velo_left =-1.0;
    dynamic_implement_to_design(&impl, &des);
    printf("straight backward:\ntranslation:\t%g\nrotation:\t%g\n\n", des.velo_translation, des.velo_rotation);

    impl.velo_right = 0;
    dynamic_implement_to_design(&impl, &des);
    printf("u-turn a-clk backward:\ntranslation:\t%g\nrotation:\t%g\n\n", des.velo_translation, des.velo_rotation);

    printf("### design_to_impl:\n" );
    des.velo_rotation = 0;
    des.velo_translation = 0.5;
    dynamic_design_to_implement(&des, &impl);
    printf("forward 0.5 m/s:\nRight:\t%g\nLeft:\t%g\n\n", impl.velo_right, impl.velo_left);

    des.velo_rotation = 0;
    des.velo_translation = -0.5;
    dynamic_design_to_implement(&des, &impl);
    printf("backward 0.5 m/s:\nRight:\t%g\nLeft:\t%g\n\n", impl.velo_right, impl.velo_left);
    
    des.velo_rotation = -360;
    des.velo_translation = -0.0;
    dynamic_design_to_implement(&des, &impl);
    printf("360 deg/s clockwise:\nRight:\t%g\nLeft:\t%g\n\n", impl.velo_right, impl.velo_left);

    return 0;
}