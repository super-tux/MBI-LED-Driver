/*
 * MBI.c
 *
 * Created: 11/03/2021 14:32:35
 *  Author: Valentin
 */ 


#include "MBI.h"


void MBI_send_row(RGB_t row[16]) {
	uint16_t w;
	for(uint8_t i = 0; i < 15; i++) {
		w = row[i].blue;
		// square all brightness values to make them appear more linear
		w *= w;
		SPI_send_word(w, 0);
		w = row[i].red;
		w *= w;
		SPI_send_word(w, 0);
		w = row[i].green;
		w *= w;
		SPI_send_word(w, 1);
	}
	w = row[15].blue;
	w *= w;
	SPI_send_word(w, 0);
	w = row[15].red;
	w *= w;
	SPI_send_word(w, 0);
	w = row[15].green;
	w *= w;
	SPI_send_word(w, 3);
}
