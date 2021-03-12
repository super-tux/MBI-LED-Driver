/*
 * MBI_SPI_driver.c
 *
 * Created: 3/8/2021 9:07:21 AM
 * Author : VXS10011
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "MBI.h"

// generate a PWM clok
void clock_init(void) {
	
}


int main(void)
{
	SPI_init();
	
	RGB_t buffer[16];
	
    for(uint8_t j = 0; ; j++) {
		for(uint8_t i = 0; i < 16; i++) {
			buffer[i].red = j;
			buffer[i].blue = j;
			buffer[i].green = j;
		}
		MBI_send_row(buffer);
		_delay_ms(50);
	}
}

