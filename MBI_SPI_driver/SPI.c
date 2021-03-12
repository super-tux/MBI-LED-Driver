/*
 * SPI.c
 *
 * Created: 11/03/2021 14:41:15
 *  Author: Valentin
 */ 

#include "SPI.h"


void SPI_init(void) {
	// make pins output
	DDRB |= (1<<PB2)	// MOSI
	| (1<<PB1)	// SCK
	| (1<<PB7)	// LED
	| (1<<PB0);	// SS
	
	// SS pin must be output BEFORE enabling SPI
	SPCR = (1<<SPE)	// Enable SPI
	| (1<<MSTR)	// Master mode
	| (1<<SPR0);	// clock rate = F_CPU/16
	
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

// send one word, pull latch high for the last n bits
void SPI_send_word(uint16_t word, uint8_t latch_bits) {
	// set LE pin low
	PORTF &= ~(1<<PF0);
	// start timer
	
	if(latch_bits != 0) {
		OCR5B = 14-latch_bits;
		TCNT5 = 0;
	}
	
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

ISR(TIMER5_COMPB_vect) {
	// pull latch pins high
	PORTF |= (1<<PF0);
}
