#pragma once

#ifndef TEDAVR_DEVICE_DUALSHOCK2_DEFINITION_CUSTOM

#elif

#include "../../tedavr_device/dualshock2_definition.h"

#endif

#ifndef TEDAVR_DEVICE_DUALSHOCK2_EXPRESSION_CUSTOM

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__)
#include "dualshock2/avr_atmega328p.h"
#endif

#elif

#include "../../tedavr_device/dualshock2_expression.h"

#endif
