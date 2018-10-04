#!/bin/bash
avr-gcc -c -mmcu=attiny13 -I. -g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=blink_led_t13.lst  -std=gnu99 blink_led_t13.c -o blink_led_t13.o

avr-gcc -mmcu=attiny13 -I. -g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=blink_led_t13.o  -std=gnu99 blink_led_t13.o   --output blink_led_t13.elf -Wl,-Map=blink_led_t13.map,--cref -lm

avr-objcopy -O ihex -R .eeprom blink_led_t13.elf blink_led_t13.hex

#avrdude -p t13 -P /dev/ttyACM0 -c avrisp -b 19200 -U flash:w:$1.hex 
avrdude -pattiny13 -cusbasp -b19200 -Uflash:w:blink_led_t13.hex 

/storage/Development/avr/tiny13/attiny13-lamp-controller/blink_led_t13.c