#include "./hid_input.h"
#include <string.h>



char *hid_get_key_state(){
	// key_state is a file scoped variable
	return key_state;
}


int hid_open_device(char *device){
	int fd;
  	if ((fd = open (device, O_RDONLY)) == -1){
    	perror("Unable to open device for reading");
		exit(EXIT_FAILURE);
	}
	return fd;
}




/* 
XXX: If this switch gets too large, it should propably be replaced
my a more formal data structure (e.g a hash map).
*/
void hid_translate_state_change(uint16_t button, char new_state, char state_array[]){
	switch(button){
		case KEY_W: state_array[IDX_FWD]  = new_state; break;
		case KEY_A: state_array[IDX_LEFT] = new_state; break;
		case KEY_S: state_array[IDX_BACK] = new_state; break;
		case KEY_D: state_array[IDX_RIGHT]= new_state; break;
		//case KEY_UP: state_array[IDX_FWD]  = new_state; break;
		//case KEY_LEFT: state_array[IDX_LEFT] = new_state; break;
		//case KEY_DOWN: state_array[IDX_BACK] = new_state; break;
		//case KEY_RIGHT: state_array[IDX_RIGHT]= new_state; break;
		case KEY_R: state_array[IDX_TRANS_INC] = new_state; break;
		case KEY_F:	state_array[IDX_TRANS_DEC] = new_state; break;
		case KEY_TAB: state_array[IDX_ROT_INC] = new_state; break;
		case KEY_CAPSLOCK: state_array[IDX_ROT_DEC] = new_state; break;
		case KEY_SPACE: state_array[IDX_ALLSTOP] = new_state; break;
		default: break;
	}
}

// TODO: replace this function with memset() form <string.h>
void hid_clear_key_state(char key_state[]){
	int i;
	for (i = 0; i < NUM_INDEXED_KEYS; i++) {
		key_state[i] = 0;
	}
}

/*
argument is void* not char* because the function is intended to be called through prthread_create, which passes all argument as a
single void*
*/
void *hid_polling_loop(void *device){

	int hid_fd = hid_open_device(device);

	int packet_size = sizeof (struct input_event);
	struct input_event event;

	// boolean, set to true when state change has happened 
	char button_state_change = 0;

	// static char key_state[] is declared in file scope

	hid_clear_key_state(key_state);
	
  	while (1){
		// this is a blocking read operation, it will wait until a full event
		// packet has been read from the device
      	if (read(hid_fd, &event, packet_size) < packet_size){
         		perror("Failed to read keyboard data from device\n");
				exit(EXIT_FAILURE);
		}
		
		
		// Event frames that start with EV_MSC packet indicate button state changes
		if (event.type == EV_MSC && event.code == MSC_SCAN) {
			button_state_change = 1;
		}
		
		// After an EV_MSC packet (i.e a state change indicator), an EV_KEY packet describes how the state of the button was changed (.value). The .code field specifies which button was changed
		else if (event.type == EV_KEY && button_state_change) {
			// write key event into key_state array
			hid_translate_state_change(event.code, event.value, key_state);	
			
		}
		
		// Event frames end with a synchronization packet, it contains no data.
		// Variables are cleared to know state before parsing next event frame.
		else if (event.type == EV_SYN) {
			button_state_change = 0;
		}

	}
  	return NULL;
} 

