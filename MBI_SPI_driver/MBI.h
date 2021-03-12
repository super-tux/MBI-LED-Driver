/*
 * MBI.h
 *
 * Created: 11/03/2021 14:28:46
 *  Author: Valentin
 */ 


#ifndef MBI_H_
#define MBI_H_

#include <avr/io.h>
#include "SPI.h"

typedef struct RGB {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} RGB_t;

void MBI_init(void);

void MBI_send_row(RGB_t row[16]);


#endif /* MBI_H_ */