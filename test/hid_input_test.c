/** 
 Main program running on the raspberry pi
 */


#include <stdio.h>
#include <pthread.h>
#include "../src/hid_input.h"
#include "../src/dynamical_model.h"

void debug_dump_array();

void translate_key_commands(char state_array[], struct dynamic_design *des){
    return;
}


void debug_dump_array(char state_array[], int len){
    int i;
    for (i = 0; i < len;++i){
        fprintf(stderr, "\t%d", state_array[i]);
    }
    fprintf(stderr, "\n");
}

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
	
	/* the polling thread will read keyboard input, and change the key_state
    array accordingly. key_state is a shared global variable */
	pthread_t polling_thread_id;
	pthread_create(&polling_thread_id, NULL, hid_polling_loop, (void *)device);
	
	
	while (1){
		
		debug_dump_array(key_state, NUM_INDEXED_KEYS);
		usleep(10000);
	}

	return 0;
}
