


key-read: ./src/key-read.c
	gcc ./src/key-read.c -o ./bin/key-read

avr-main: ./src/avr-main.c
	avr-gcc -Os -mmcu=atmega328p -DF_CPU=16000000L ./src/avr-main.c -o ./bin/avr-main
	

avr-flash:
	avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:./bin/avr-main:e
