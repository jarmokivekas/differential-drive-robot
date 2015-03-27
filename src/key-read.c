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
    		printf("Please specify (on the command line) the path to the dev event interface devicen");
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
	char toggle_next = 0;
	char inpacket[10];
  	while (1){
      		if ((rd = read (fd, ev, size)) < size){
         		perror_exit ("read()");
		}
	   	//fprintf (stderr, "%d\t%d\t%d\t%d\n", ev[0].time, ev[0].type, ev[0].code, ev[0].value);
		if(toggle_next){
			toggle_next = 0;
			switch(ev[0].code){
				case KEY_W:
					fprintf(stderr,"Keypress: forward velocity\n");
					break;
				case KEY_A:
					fprintf(stderr,"Keypress: anti-clockwise rotation\n");
					break;
				case KEY_S:
					fprintf(stderr,"Keypress: backward velocity\n");
					break;
				case KEY_D:
					fprintf(stderr,"Keypress: clockwise rotation\n");
					break;
				case KEY_SPACE:
					fprintf(stderr,"Keypress: allstop")
				default:
					fprintf(stderr,"Keypress: disregarded\n");
					break;
			}
		}
		switch(ev[0].type){
			case EV_SYN:
				fprintf(stderr, "sync\n");
				break;
			case EV_MSC:
				toggle_next = 1;
				break;
			default:
				break;
		}
	}
  	return 0;
} 
