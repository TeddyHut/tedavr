#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

	typedef struct IC_WS2812 {
		volatile uint8_t *TCCRxA;
		volatile uint8_t *TCCRxB;
		volatile uint8_t *OCxA;
		volatile uint8_t *OCxB;
	} IC_WS2812;

#ifdef __cplusplus
}
#endif