avr-gcc -mmcu=atmega16 -O -g -Wall -o avr.o avr.c
avr-objcopy -Osrec avr.o avr.sr
uisp -dprog=stk200 --erase --upload if=avr.sr
