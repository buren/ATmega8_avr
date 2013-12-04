## ATmega16 AVR

### Compile
    avr-gcc -mmcu=atmega16 -O -g -Wall -o avr.o avr.c

### Upload to ATmega16 AVR
    avr-objcopy -Osrec avr.o avr.sr
    uisp -dprog=stk200 --erase --upload if=avr.sr


### Doc
http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html#avr_signames

#### If USB porta yanked
    java.io.IOException: Input/output error in nativeavailable
      at gnu.io.RXTXPort.nativeavailable(Native Method)
      at gnu.io.RXTXPort$SerialInputStream.read(RXTXPort.java:1429)
      at gnu.io.RXTXPort$SerialInputStream.read(RXTXPort.java:1341)
      at TwoWaySerialComm$SerialReader.run(TwoWaySerialComm.java:43)
