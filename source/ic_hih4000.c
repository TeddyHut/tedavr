#include "../include/tedavr/ic_hih4000.h"

uint8_t ic_hih4000_adcXrh(uint8_t const adc) {
	uint16_t v0 = 250 * adc;
	v0 /= 391;
	v0 -= 27;
	return((uint8_t)v0);
}