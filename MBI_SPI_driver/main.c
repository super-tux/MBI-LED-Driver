/*
 * MBI_SPI_driver.c
 *
 * Created: 3/8/2021 9:07:21 AM
 * Author : VXS10011
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void counter_init(void) {
	// make ext clock pin T5 input
	DDRL &= ~(1<<PL2);
	// Arduino Pin 47
	
	// configure Timer5A
	// CS = 111 -> External Clock, falling edge
	TCCR5B |= (1<<CS52) | (1<<CS51);
	// WGM = 010 -> Clear Timer on Compare match
	TCCR5B |= (1<<WGM52);
	
	// interrupt on compare match
	TIMSK5 |= (1<<OCIE5B);
	
	// enable interrupts
	sei();
}

void SPI_init(void) {
	// make pins output
	DDRB |= (1<<PB2)	// MOSI
		  | (1<<PB1)	// SCK
		  | (1<<PB7)	// LED
		  | (1<<PB0);	// SS
	
	// SS pin must be output BEFORE enabling SPI
	SPCR = (1<<SPE)	// Enable SPI
		 | (1<<MSTR)	// Master mode
		 | (1<<SPR1);	// clock rate = F_CPU/16
		 
	// Arduino Pinout:
	// MOSI =	51
	// SCK =	52
	
	// make LE pin output
	DDRF = 0xFF;
	// Arduino Pin A0
	
	// pull LE high
	PORTF = (1<<PF0);
	
	counter_init();
}

// send one word, pull latch high for the last n bits
void SPI_send_word(uint16_t word, uint8_t latch_bits) {
	// set LE pin low
	PORTF &= ~(1<<PF0);
	// start timer
		
	// number to count to
	OCR5B = 15-latch_bits;
	TCNT5 = 0;
	
	// shift right by 8 bits to access upper byte
	SPDR = (word>>8);
	
	// wait until Transmission is done
	while(!(SPSR & (1<<SPIF)))
		asm("nop");
	
	// send lower byte 
	SPDR = word;
	// wait until Transmission is done
	while(!(SPSR & (1<<SPIF)))
		asm("nop");
}

typedef struct RGB {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} RGB_t;

void LED_send_row(RGB_t row[16]) {
	uint16_t w;
	for(uint8_t i = 0; i < 15; i++) {
		w = row[i].red;
		// square all brightness values to make them appear more linear
		w *= w;
		SPI_send_word(w, 0);
		w = row[i].green;
		w *= w;
		SPI_send_word(w, 0);
		w = row[i].blue;
		w *= w;
		SPI_send_word(w, 1); 
	}
	w = row[15].red;
	w *= w;
	SPI_send_word(w, 0);
	w = row[15].green;
	w *= w;
	SPI_send_word(w, 0);
	w = row[15].blue;
	w *= w;
	SPI_send_word(w, 3);
}

ISR(TIMER5_COMPB_vect) {
	// pull latch pins high
	PORTF |= (1<<PF0);
}



int main(void)
{
	SPI_init();
	
	RGB_t buffer[16];
	for(uint8_t i = 0; i < 16; i++) {
		buffer[i].red = 255 * (i % 2);
		buffer[i].blue = i;
		buffer[i].green = i * 8;
	}
	
	uint16_t i = 0;
    while(1) {
		LED_send_row(buffer);
		_delay_ms(10);
	}
}

