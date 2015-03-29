#include "./hid_input.h"




int hid_open_device(char *device){
	int fd;
  	if ((fd = open (device, O_RDONLY)) == -1){
    	perror("Unable to open device for reading");
		exit(EXIT_FAILURE);
	}
	return fd;
}




void hid_translate_state_change(uint16_t button, char new_state, char state_array[]){
	switch(button){
		case KEY_W:
			//fprintf(stderr,"Keypress: forward velocity\n");
			state_array[IDX_W] = new_state;
			break;
		case KEY_A:
			//fprintf(stderr,"Keypress: anti-clockwise rotation\n");
			state_array[IDX_A] = new_state;
			break;
		case KEY_S:
			//fprintf(stderr,"Keypress: backward velocity\n");
			state_array[IDX_S] = new_state;
			break;
		case KEY_D:
			//fprintf(stderr,"Keypress: clockwise rotation\n");
			state_array[IDX_D] = new_state;
			break;
		case KEY_SPACE:
			//fprintf(stderr,"Keypress: allstop");
			state_array[IDX_SPACE] = new_state;
			break;
		default:
			break;
	}
	fprintf(stderr, "\n%d\t%d\t%d\t%d\t%d\t", state_array[0], state_array[1], state_array[2], state_array[3], state_array[4]);
}


void hid_clear_key_state(char key_state[]){
	int i;
	for (i = 0; i < NUM_INDEXED_KEYS; i++) {
		key_state[i] = 0;
	}
}


void *hid_polling_loop(void *device){

	int hid_fd = hid_open_device(device);
	int packet_size = sizeof (struct input_event);
	struct input_event event;
	char button_state_change = 0;	//< bool, true when state change has happened 
	char key_state[NUM_INDEXED_KEYS];
	
	hid_clear_key_state(key_state);
	
  	while (1){
		// this is a blocking read operation, it will wait until a full event
		// packet has been read from the device
      	if (read(hid_fd, &event, packet_size) < packet_size){
         		perror("Failed to read keyboard data from device\n");
				exit(EXIT_FAILURE);
		}
		
	   	//fprintf (stderr, "\ndebug: %d\t%d\t%d\n", event.type, event.code, event.value);

		
		// Event frames that start with EV_MSC packet indicate button
		// state changes
		if (event.type == EV_MSC && event.code == MSC_SCAN) {
			button_state_change = 1;
		}
		
		// After an EV_MSC packet, an EV_KEY packet describes how the state 
		// of the button was changed. The .code field specifies which button
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

