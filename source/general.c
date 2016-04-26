#include "../include/tedavr/general.h"

void general_SetBitFlag_u8(volatile uint8_t*const byte, uint8_t const bit_significance, uint8_t const state) {
	*byte = ((*byte & _BV(bit_significance)) | ((state ? 1 : 0) << bit_significance));
}

uint8_t general_GetBitFlag_u8(volatile uint8_t const*const byte, uint8_t const bit_significance) {
	return(((*byte) & _BV(bit_significance)) ? 1 : 0);
}