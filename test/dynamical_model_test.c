//dynamical_model_test.c
#include <stdio.h>
#include "../src/dynamical_model.h"


int main(int argc, char const *argv[]) {
    struct dynamic_design des = {0.0, 0.0};
    struct dynamic_implement impl = {1.0, 1.0};
    
    
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



    return 0;
}