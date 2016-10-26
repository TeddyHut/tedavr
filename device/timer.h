#pragma once

#ifndef TEDAVR_DEVICE_TIMER_DEFINITION_CUSTOM

#define TIMER_INTERVAL 0.001

#elif

#include "../../tedavr_device/timer_definition.h"

#endif

#ifndef TEDAVR_DEVICE_TIMER_EXPRESSION_CUSTOM

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__)
#include "timer/avr_atmega328p.h"
#endif

#elif

#include "../../tedavr_device/timer_expression.h"

#endif
