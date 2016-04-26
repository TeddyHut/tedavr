#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <avr/sfr_defs.h>
#include <inttypes.h>

void general_SetBitFlag_u8(volatile uint8_t*const byte, uint8_t const bit_significance, uint8_t const state);	//Could rename that to bit_shift
uint8_t general_GetBitFlag_u8(volatile uint8_t const*const byte, uint8_t const bit_significance);				//Someone tell my why I'm making it a pointer.

#ifdef __cplusplus
}
#endif
