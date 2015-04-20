
CFLAGS = -Wall
PIFLAGS = -pthread
AVRFLAGS = -DF_CPU=16000000L
CC = gcc

doc/raportti.pdf: doc/raportti.tex
	pdflatex $^ --output-directory=doc/

key-read: ./src/key-read.c
	gcc -Wall ./src/key-read.c -o ./bin/$@

avr-main: ./src/avr-main.c
	avr-gcc -Os -mmcu=atmega328p -DF_CPU=16000000L -c ./src/avr-main.c    -o ./bin/avr-main.o
	avr-gcc -Os -mmcu=atmega328p -DF_CPU=16000000L -c ./src/stepper_ctl.c -o ./bin/stepper_ctl.o
	avr-gcc -Os -mmcu=atmega328p -DF_CPU=16000000L ./bin/stepper_ctl.o ./bin/avr-main.o -o ./bin/avr-main
	

avr-flash:
	avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:./bin/avr-main:e

rpi-main: \
	bin/rpi_main.o\
	bin/dynamical_model.o\
	bin/hid_input.o
	$(CC) -pthread $(CFLAGS) -o $@ $^

bin/dynamical_model_test: \
	bin/dynamical_model_test.o\
	bin/dynamical_model.o
	$(CC) $(CFLAGS) -o $@ $^
	
bin/hid_input_test: \
	bin/hid_input_test.o\
	bin/hid_input.o
	$(CC) -pthread $(CFLAGS) -o $@ $^
	
	
bin/%.o: src/%.c
	$(CC) -pthread $(CFLAGS) -c -o $@ $^
bin/%_test.o: test/%_test.c
	$(CC) -pthread $(CFLAGS) -c -o $@ $^

#--------CLEANUP--------
clean:
	rm -rf bin/* *.out
