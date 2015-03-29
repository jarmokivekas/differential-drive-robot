#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
// #include <termios.h>
// #include <signal.h>

// define array indexes for the button_state array.
#define IDX_W 0
#define IDX_A 1
#define IDX_S 2
#define IDX_D 3
#define IDX_SPACE 4



int main (int argc, char *argv[])
{
	int hid_fd;
	char *device;

	// Setup argument check
	if (argv[1] == NULL){
    		printf("Usage: %s <input device>\n", argv[0]);
      		exit (EXIT_FAILURE);
    } else {
		device = argv[1];
	}

  	//Open device descriptor for read operations
  	if ((hid_fd = open (device, O_RDONLY)) == -1){
    		fprintf(stderr, "Unable to open device for reading: %s\n", device);
			exit(EXIT_FAILURE);
	}

	int packet_size = sizeof (struct input_event);
	struct input_event event;
	char button_state_change = 0;	//< bool, true when state change has happened 
	char button_new_state = 0;
	char button_effected = -1;		//< default to keycode that does not exist
	char key_state[6];
	
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
		else if (event.type == EV_KEY) {
			button_new_state = event.value;
			button_effected  = event.code;
		}
		// Event frames end with a synchronization packet, it contains no data.
		// Variables are cleared to know state before parsing next event frame.
		else if (event.type == EV_SYN) {
			button_new_state = 0;
			button_state_change = 0;
			button_effected = -1;
		}




		if(button_state_change){
			//button_state_change = 0;
			switch(button_effected){
				case KEY_W:
					//fprintf(stderr,"Keypress: forward velocity\n");
					key_state[IDX_W] = button_new_state;
					break;
				case KEY_A:
					//fprintf(stderr,"Keypress: anti-clockwise rotation\n");
					key_state[IDX_A] = button_new_state;
					break;
				case KEY_S:
					//fprintf(stderr,"Keypress: backward velocity\n");
					key_state[IDX_S] = button_new_state;
					break;
				case KEY_D:
					//fprintf(stderr,"Keypress: clockwise rotation\n");
					key_state[IDX_D] = button_new_state;
					break;
				case KEY_SPACE:
					//fprintf(stderr,"Keypress: allstop");
					key_state[IDX_SPACE] = button_new_state;
					break;
				default:
					break;
			}
			//button_new_state = 0;
			fprintf(stderr, "\n%d\t%d\t%d\t%d\t%d\t", key_state[0], key_state[1], key_state[2], key_state[3], key_state[4]);
		}

	}
  	return 0;
} 
