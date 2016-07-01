#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__AVR_ATtiny84__)
#include "timer/avr_attiny84.h"
	
#elif defined(__INTELLISENSE__)
#include "timer/intellisense.h"

#endif

#ifdef __cplusplus
}
#endif