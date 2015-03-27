

////////////////////////////////
// Define constant parameters //
////////////////////////////////


// all length units are in millimetes

// motor is a astrosym 17PM stepper motor
motor_length = 034;
motor_width = 42;
motor_height = 42;




module base_plane(){
	circle(100);
}



module motor_attachment(){
    cube(size = [motor_length, motor_width, motor_height], center = true);
}

module example001()
{
	function r_from_dia(d) = d / 2;

	module rotcy(rot, r, h) {
		rotate(90, rot)
			cylinder(r = r, h = h, center = true);
	}

	difference() {
		sphere(r = r_from_dia(size));
		rotcy([0, 0, 0], cy_r, cy_h);
		rotcy([1, 0, 0], cy_r, cy_h);
		rotcy([0, 1, 0], cy_r, cy_h);
	}

	size = 50;
	hole = 25;

	cy_r = r_from_dia(hole);
	cy_h = r_from_dia(size * 2.5);
}

// example001();
// base_plane();
motor_attachment();
