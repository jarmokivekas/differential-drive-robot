#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>


#define FLOAT_STR_MAX_LEN
#define MSG_BUFFER_LEN 255
#define MSG_START "$"
#define MSG_END "#"

int main(int argc, char const *argv[]){
	
	unsigned char velo_l = atoi(argv[2]);
	printf("volecity is %d", velo_l);
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
	
	char message_buffer[MSG_BUFFER_LEN];
	
	
	// ^0.1231231,234.12341$
	float a = 0.2341;
	
	
	
	char data[4];
	data[0] = 'A';
	data[3] = 'B';
	char i;
	for(i = 0; i < 256; ++i){
		usleep(5000);
		data[1] = i;
		data[2] = i<<2 ; 
		write(uart_fd, data, 4);
		char recvData[1];
		int len = 0;
	//	while(len = read(uart_fd, recvData, 1*sizeof(char))){
	//		write(1, recvData, 1*sizeof(char));
	//	}	
	}
	usleep(500000);
	
	close(uart_fd);
	return 0;


}

