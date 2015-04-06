/** 
 Main program running on the raspberry pi
 */


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>


#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include "hid_input.h"
#include "dynamical_model.h"


struct movement_command {
    /* multiplier used to set the sign of- or disable either
    if the velocities */
    int8_t trans_mul;
    int8_t rot_mul;
    /* magnitudes hold the absolute value, but no directional information */
    float trans_magn;
    float rot_magn;
};

/* Function prototypes */
void apply_keyboard_commands(char state_array[], struct movement_command *command);
void debug_dump_array(char state_array[], int len);
void debug_dump_commands(struct movement_command *command);
int uart_init(char* uart_device);


int main (int argc, char *argv[])
{
	// Setup argument check
	char *hid_device;
	char *uart_device;
	if ((argv[1] == NULL) || (argv[2] == NULL)){
    		printf("Usage: %s <hid device> <uart device>\n", argv[0]);
      		exit (EXIT_FAILURE);
	}else {
		hid_device  = argv[1];
		uart_device = argv[2];
	}
	
	/*
    The polling thread will read keyboard input, and change the key_state
    array accordingly. key_state is a shared global variable. 
    There is no need of a mutex mechanism, since the main thread will onlyread data.
    */
	pthread_t polling_thread_id;
	pthread_create(&polling_thread_id, NULL, hid_polling_loop, (void *)hid_device);
	
	int uart_fd = -1;
	if ((uart_fd = uart_init(uart_device)) == -1){
		fprintf(stderr, "Failed to open uart file: %s", uart_device);
		exit(EXIT_FAILURE);
		
	}
    
	struct movement_command command = {.rot_mul = 1,.trans_mul =1, .rot_magn=1.0,.trans_magn=1.0};
   	char msg_buffer [256];
	int msg_idx = 0; 
	while (1){
		msg_idx = 0;
		apply_keyboard_commands(key_state, &command);
		//debug_dump_array(key_state, NUM_INDEXED_KEYS);
		debug_dump_commands(&command);
		usleep(100000);
		//msg_idx = sprintf(msg_buffer, "$%g,%g#", command.rot_magn, command.trans_magn);
		msg_idx = sprintf(msg_buffer, "$%g,%g#", command.rot_magn, command.trans_magn);
		write(uart_fd, "$12.12#\n", 8);
		fprintf(stderr, "message:\t%s\nmsg_idx:\t%d\n", msg_buffer, msg_idx);
	}

	return 0;
}




void apply_keyboard_commands(char state_array[], struct movement_command *command){

    /* Just renaming the variable to make conditional statements below look shorter. */
    char *sa = state_array;
    
    /*
    State_array indexes should only contain the values 0x00 or 0x01. 
    This allows us to use bitwise logic operators to combine them, instead of 
    comparisons with == operator
    */
    
    
    /* 
    Handle forward/backwad keys.
    First check if both or neither button is pressed, then if just
    one or the other is pressed.
    */
    if (!(sa[IDX_FWD] ^ sa[IDX_BACK]))  {command->trans_mul = 0;} 
    else if ( sa[IDX_FWD]  )            {command->trans_mul = 1;}
    else if ( sa[IDX_BACK] )            {command->trans_mul = -1;}
    
    
    /*
    Handle right/left rotatation keys.
    Anti-clockwise rotation is positive.
    Clockwise rotation is considered negative.
    */
    if (!(sa[IDX_RIGHT] ^ sa[IDX_LEFT])){command->rot_mul = 0;}
    else if ( sa[IDX_LEFT]  )           {command->rot_mul = 1;}
    else if ( sa[IDX_RIGHT] )           {command->rot_mul = -1;}
    
    /* 
    TODO: there will be incositencies in the user experience if the
    delay in the control loop is changed. The effect the throttle has
    should be proportional to the length of the delay between calls to 
    this function
     */
    /* 
    Handle translation velocity throttle.
    First check that only one or the other is pressed.
    */
    if (sa[IDX_TRANS_INC] ^ sa[IDX_TRANS_DEC]){
        if      ( sa[IDX_TRANS_INC] )   {command->trans_magn *= 1.01;}
        else if ( sa[IDX_TRANS_DEC] )   {command->trans_magn *= 0.99;}
    }

    /* 
    Handle rotationional velocity throttle.
    */
    if (sa[IDX_ROT_INC] ^ sa[IDX_ROT_DEC]){
        if      ( sa[IDX_ROT_INC] )     {command->rot_magn *= 1.01;}
        else if ( sa[IDX_ROT_DEC] )     {command->rot_magn *= 0.99;}
    }
    
    
    /* TODO: make sure magnitudes are inside a sutable range. */
    
    return;
}






/**
Uart initialization rutine for RaspberryPi.
@param device: Null terminated string 
*/
int uart_init(char *device){
	/* file desctiptor for the uart file */
        int uart_fd = -1;
        // removed O_NDELAY from list
        uart_fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK| O_NDELAY);
        if (uart_fd == -1){
                printf("Error: Unable to open uart file descriptor");
                return -1;
        }
        fprintf(stderr, "file is opened\n ");
        // set file desctiptor flag to read-write (might be redundant)
        fcntl(uart_fd, F_SETFL, O_RDWR);

        // set correct options for the serial comms
        struct termios options;
        tcgetattr(uart_fd, &options);
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;         //<Set baud rate
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(uart_fd, TCIFLUSH);
        tcsetattr(uart_fd, TCSANOW, &options);

        fprintf(stderr, "uart settings set\n");
	return uart_fd;
}






/*
Used during development to dump the key state array onto stderr
*/
void debug_dump_array(char state_array[], int len){
    int i;
    for (i = 0; i < len;++i){
        fprintf(stderr, "\t%d", state_array[i]);
    }
    fprintf(stderr, "\n");
}


void debug_dump_commands(struct movement_command *command){
    fprintf(stderr, "\nrotation:\t%g\t%d\ntranslation:\t%g\t%d\n",
    command->rot_magn, command->rot_mul,
    command->trans_magn, command->trans_mul
    );
}

