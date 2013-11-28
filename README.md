## ATmega8 AVR

### Compile
    avr-gcc -mmcu=atmega8 -O -g -Wall -o avr.o avr.c

### Upload to ATmega8 AVR
    avr-objcopy -Osrec avr.o avr.sr
    uisp -dprog=stk200 --erase --upload if=avr.sr

#### To open a serial terminal on a LTH control PC, connected to ATmeaga8 AVR
    simcom -38400 /dev/ttyS0


### Doc
http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html#avr_signames

#### If USB porta yanked
    java.io.IOException: Input/output error in nativeavailable
      at gnu.io.RXTXPort.nativeavailable(Native Method)
      at gnu.io.RXTXPort$SerialInputStream.read(RXTXPort.java:1429)
      at gnu.io.RXTXPort$SerialInputStream.read(RXTXPort.java:1341)
      at TwoWaySerialComm$SerialReader.run(TwoWaySerialComm.java:43)
