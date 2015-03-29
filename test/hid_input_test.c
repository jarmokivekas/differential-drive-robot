#include <stdio.h>
#include "../src/hid_input.h"


int main (int argc, char *argv[])
{
	// Setup argument check
	char *device;
	if (argv[1] == NULL){
    		printf("Usage: %s <input device>\n", argv[0]);
      		exit (EXIT_FAILURE);
    }else{
		device = argv[1];
	}
	
	hid_polling_loop((void *)device);

	return;
}
