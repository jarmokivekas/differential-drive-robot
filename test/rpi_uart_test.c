#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>

#define FLOAT_STR_MAX_LEN
#define MSG_BUFFER_LEN 255
#define MSG_START "$"
#define MSG_END "#"

int main(int argc, char const *argv[]){
	
	int uart_fd = -1;
	// removed O_NDELAY from list
	uart_fd = open(argv[1], O_RDWR | O_NOCTTY | O_NONBLOCK| O_NDELAY);
	if (uart_fd == -1){
		printf("Error: Unable to open uart file descriptor");
		return -1;	
	}

	// set file desctiptor flag to read-write (might be redundant)
	fcntl(uart_fd, F_SETFL, O_RDWR); 

	// set correct options for the serial comms
	struct termios options;
	tcgetattr(uart_fd, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart_fd, TCIFLUSH);
	tcsetattr(uart_fd, TCSANOW, &options);
	
	
	uint16_t data[2];
	char  msg_start[1] = {'a'};
	int i;
	for(i = 0; i < 64255; ++i){
		usleep(50000);
		data[0] = atoi(argv[2]);
		data[1] = i  <<1; 
		write(uart_fd,"a", 1);
		write(uart_fd, data, 2*sizeof(uint16_t));
		fprintf(stdout, "%d, %d\n", data[0], data[1]);
	//	while(len = read(uart_fd, recvData, 1*sizeof(char))){
	//		write(1, recvData, 1*sizeof(char));
	//	}	
	}	
	usleep(500000);
	
	close(uart_fd);
	return 0;


}

