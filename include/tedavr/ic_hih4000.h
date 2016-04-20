#pragma once

#include <inttypes.h>

//Converts an ADC value to the equivilent relative humidity
uint8_t ic_HIH4000_adcXrh(uint8_t const adc);