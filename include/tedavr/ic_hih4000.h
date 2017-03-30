#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

//Converts an ADC value to the equivilent relative humidity
uint8_t ic_HIH4000_adcXrh(uint8_t const adc);

#ifdef __cplusplus
}
#endif
