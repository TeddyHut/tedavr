#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/sfr_defs.h>

#include "general.h"

//Refer to https://www.nxp.com/documents/data_sheet/74HC_HCT595.pdf

typedef enum IC_74HC595_Pin_e {
	IC_74HC595_Pin_sero,					//Serial data					(out)
	IC_74HC595_Pin_clr,						//Register clear (active low)	(out)
	IC_74HC595_Pin_shck,					//Shift-register clock input	(out)
	IC_74HC595_Pin_stck,					//Storage-register clock input	(out)
	IC_74HC595_Pin_seri,					//Serial data input				(in)
	IC_74HC595_Pin_oe						//Output enable (active low)	(out)
}IC_74HC595_Pin_e;

typedef struct IC_74HC595 {
	uint8_t flag_function_virtual : 1;		//Default = 1; Keep virtual copy of register contents
	uint8_t flag_connected_seri : 1;		//Default = 1; Seri is connected to a pin
	uint8_t data_shift_sero : 3;
	uint8_t data_shift_clr : 3;
	uint8_t data_shift_shck : 3;
	uint8_t data_shift_stck : 3;
	uint8_t data_shift_seri : 3;
	uint8_t data_shift_oe : 3;
uint8_t: 0;
	size_t data_register_size;			//Size of register array in bytes
	volatile uint8_t *data_port_sero_p;
	volatile uint8_t *data_port_clr_p;
	volatile uint8_t *data_port_shck_p;
	volatile uint8_t *data_port_stck_p;
	volatile uint8_t *data_port_seri_p;
	volatile uint8_t *data_port_oe_p;
	void *data_register_virtual;			//Virtual register contents
}IC_74HC595;

//Set the pin -pin- to the value of -state-
void ic_74HC595_setPin(IC_74HC595 const *const ic_74hc595, IC_74HC595_Pin_e const pin, uint8_t const state);
//Get the state of the pin -pin-
uint8_t ic_74HC595_getPin(IC_74HC595 const *const ic_74hc595, IC_74HC595_Pin_e const pin);
//Clear 74HC595 contents to 0
void ic_74HC595_clear(IC_74HC595 *const ic_74hc595);
//Setup 74hc595 according to default values
void ic_74HC595_defaultSetup(IC_74HC595 *const ic_74hc595);
//Enable or disable outputs according to -state-
void ic_74HC595_enableOutput(IC_74HC595 const *const ic_74hc595, uint8_t const state);
//Latch stored register contents to parallel output
void ic_74HC595_latchOutput(IC_74HC595 const *const ic_74hc595);
//Enable or disable master reset according to -state-
void ic_74HC595_clearContents(IC_74HC595 const *const ic_74hc595);
//Push a -byte- into the register stack. Returns popped data
uint8_t ic_74HC595_push_pop_byte(IC_74HC595 const *const ic_74hc595, uint8_t const byte);
//Push and pop data from the register stack. Data returned will be placed in -buffer-
void ic_74HC595_push_pop(IC_74HC595 const *const ic_74hc595, void *const buffer, size_t const buffer_size);
//Push a -byte- into the stack
void ic_74HC595_push_byte(IC_74HC595 const *const ic_74hc595, uint8_t const byte);
//Push data into the regiter stack
void ic_74HC595_push(IC_74HC595 const *const ic_74hc595, void const*const buffer, size_t const buffer_size);
//Pop a -byte- from the stack
uint8_t ic_74HC595_pop_byte(IC_74HC595 const *const ic_74hc595);
//Pop data from the register stack
void ic_74HC595_pop(IC_74HC595 const *const ic_74hc595, void *const buffer, size_t const buffer_size);
//Allocates memory to be used for virtual stack
void ic_74HC595_allocateVirtualMemory(IC_74HC595 *const ic_74hc595);
