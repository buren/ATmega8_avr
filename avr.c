/**
 * AVR program for control of the DC-servo process.
 *
 * User communication via the serial line. Commands:
 * s: start controller
 * t: stop controller
 *
 * * To compile for the ATmega8 AVR:
 *   avr-gcc -mmcu=atmega8 -O -g -Wall -o avr.o avr.c
 *
 * * To compile for the ATmega16 AVR:
 *   avr-gcc -mmcu=atmega16 -O -g -Wall -o avr.o avr.c
 *
 * To upload to the AVR:
 * avr-objcopy -Osrec avr.o avr.sr
 * uisp -dprog=stk200 --erase --upload if=avr.sr
 *
 * To view the assembler code:
 * avr-objdump -S avr.o
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>


char buffer[10];
int bytes_to_read = 0;
int buffer_pos = 0;

/**
 * Write a character on the serial connection
 */
static inline void put_char(char ch){
  while ((UCSRA & 0x20) == 0) {};
  UDR = ch;
}

/**
 * Write 10-bit output using the PWM generator
 */
static inline void writeOutput(int16_t val) {
  val += 539;
  OCR1AH = (uint8_t) (val>>8);
  OCR1AL = (uint8_t) val;
}

/**
 * Read 10-bit input using the AD converter
 */
static inline int16_t readInput(char chan) {
  uint8_t low, high;
  ADMUX = 0xc0 + chan;             /* Specify channel (0 or 1) */
  ADCSRA |= 0x40;                  /* Start the conversion */
  while (ADCSRA & 0x40);           /* Wait for conversion to finish */
  low = ADCL;                      /* Read input, low byte first! */
  high = ADCH;                     /* Read input, high byte */
  return ((high<<8) | low) - 512;  /* 10 bit ADC value [-512..511] */
}

/**
 * Interrupt handler for receiving characters over serial connection.
 * UART_RXC_vect => USART Receive Complete.
 */
ISR(USART_RXC_vect){
    char c = UDR; //read char from serial port

    if (bytes_to_read > 0)
    {
        buffer[buffer_pos++] = c;
        --bytes_to_read;
        if (bytes_to_read == 0)
        {
            int16_t data;
            switch (buffer[0])
            {
            case 'R':
                data = readInput(buffer[1]);
                put_char('S');
                put_char(buffer[1]);
                put_char(data >> 8);
                put_char(data & 0xff);
                break;
            case 'W':
                data = buffer[1] | (buffer[2] << 8);
                writeOutput(data);
                put_char('A');
                put_char('A');
                put_char('A');
		put_char('A');
                break;
            }
        }
    }
    else
    {
        switch (c)
        {
        case 'R':                        /* Read value */
            buffer_pos = 0;
            buffer[buffer_pos++] = 'R';
            bytes_to_read = 1;
            break;
        case 'W':                        /* Write value */
            buffer_pos = 0;
            buffer[buffer_pos++] = 'W';
            bytes_to_read = 2;
            break;
        }
    }
}

/**
 * Main program
 */
int main(){

  //DDRB = 0x02;    /* Enable PWM output for ATmega8 */
  DDRD = 0x20;    /* Enable PWM output for ATmega16 */
  DDRC = 0x30;    /* Enable time measurement pins */
  ADCSRA = 0xc7;  /* ADC enable */

  TCCR1A = 0xf3;  /* Timer 1: OC1A & OC1B 10 bit fast PWM */
  TCCR1B = 0x09;  /* Clock / 1 */

  /* Configure serial communication */
  UCSRA = 0x00;   /* USART: */
  UCSRB = 0x98;   /* USART: RXC enable, Receiver enable, Transmitter enable */
  UCSRC = 0x86;   /* USART: 8bit, no parity */
  UBRRH = 0x00;   /* USART: 57600 @ 14.7456MHz */
  UBRRL = 15;     /* USART: 57600 @ 14.7456MHz */
  //UBRRH = 0x00;   /* USART: 38400 @ 14.7456MHz */
  //UBRRL = 23;     /* USART: 38400 @ 14.7456MHz */

  TIMSK = 1<<OCIE2; /* Start periodic timer */

  sei();          /* Enable interrupts */

  while (1);
}
