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
#include <termios.h>
#include <signal.h>

// define array indexes for the button_state array.
#define IDX_W 0
#define IDX_A 1
#define IDX_S 2
#define IDX_D 3
#define IDX_SPACE 4


void handler (int sig)
{
	printf ("nexiting...(%d)n", sig);
	exit (0);
}

void perror_exit (char *error)
{
	perror (error);
	handler (9);
}

int main (int argc, char *argv[])
{
	struct input_event ev[1];
	int fd, rd, value, size = sizeof (struct input_event);
	char name[256] = "Unknown";
	char *device = NULL;

	//Setup check
	if (argv[1] == NULL){
    		printf("Usage: %s <input device>", argv[0]);
      		exit (0);
    	}
	printf("setup done\n");
  	if ((getuid ()) != 0){
    		fprintf(stderr, "WARNING: not running as root");
	}
  	if (argc > 1){
    		device = argv[1];
	}
  	//Open Device
  	if ((fd = open (device, O_RDONLY)) == -1){
    		printf ("%s is not a vaild device\n", device);
	}
	printf("opened device file: %s \n", device);
  	//Print Device Name
  	ioctl (fd, EVIOCGNAME (sizeof (name)), name);
  	printf ("Reading From : %s (%s)\n", device, name);

	printf("about to enter loop\n");


	char button_state_change = 0;
	char button_new_state = 0;
	char inpacket[10];
	char key_state[6];
	
  	while (1){
      	if ((rd = read (fd, ev, size)) < size){
         		perror_exit ("read()");
		}
	   	//fprintf (stderr, "%d\t%d\t%d\n", ev[0].type, ev[0].code, ev[0].value);

		switch(ev[0].type){
			case EV_SYN:
				fprintf(stderr, "sync\n");
				break;
			case EV_KEY:
				button_new_state = ev[0].value;
			case EV_MSC:
				button_state_change = 1;
				break;
			default:
				break;
		}

		if(button_state_change){
			button_state_change = 0;
			switch(ev[0].code){
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
			button_new_state = 0;
			fprintf(stderr, "%d\t%d\t%d\t%d\t%d\t\n", key_state[0], key_state[1], key_state[2], key_state[3], key_state[4]);
		}

	}
  	return 0;
} 
