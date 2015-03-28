
CFLAGS = -Wall

doc/raportti.pdf: doc/raportti.tex
	pdflatex $^

key-read: ./src/key-read.c
	gcc ./src/key-read.c -o ./bin/key-read

avr-main: ./src/avr-main.c
	avr-gcc -Os -mmcu=atmega328p -DF_CPU=16000000L ./src/avr-main.c -o ./bin/avr-main
	

avr-flash:
	avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:./bin/avr-main:e


bin/dynamical_model_test: \
	bin/dynamical_model_test.o\
	bin/dynamical_model.o
	$(CC) $(CFLAGS) -o $@ $^
	

	
bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $^
bin/%_test.o: test/%_test.c
	$(CC) $(CFLAGS) -c -o $@ $^

#--------CLEANUP--------
clean:
	rm -rf bin/*.o *.out
