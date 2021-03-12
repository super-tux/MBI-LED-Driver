/*
 * SPI.h
 *
 * Created: 11/03/2021 14:40:37
 *  Author: Valentin
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void SPI_init(void);
void counter_init(void);

void SPI_send_word(uint16_t word, uint8_t latch_bits);

#endif /* SPI_H_ */