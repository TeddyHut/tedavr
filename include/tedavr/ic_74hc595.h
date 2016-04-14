#pragma once

#include <stddef.h>
#include <inttypes.h>
#include <avr/sfr_defs.h>

//Refer to https://www.nxp.com/documents/data_sheet/74HC_HCT595.pdf

typedef struct {
	uint8_t flag_function_virtual : 1;		//Default = 1; Keep virtual copy of register contents
	uint8_t data_shift_q7s : 3;				//Serial data output
	uint8_t data_shift_mr : 3;				//Master reset (active low)
	uint8_t data_shift_shcp : 3;			//Shift-register clock input
	uint8_t data_shift_stcp : 3;			//Storage-register clock input
	uint8_t data_shift_oe : 3;				//Ouput enable (active low)
uint8_t: 0;
	size_t data_register_size;			//Size of register array in bytes
	volatile uint8_t *data_port_q7s_p;		//The port on which q7s is located
	volatile uint8_t *data_port_mr_p;		//The port on which mr is located
	volatile uint8_t *data_port_shcp_p;		//The port on which shcp is located
	volatile uint8_t *data_port_stcp_p;		//The port on which stcp is located
	volatile uint8_t *data_port_oe_p;		//The port on which oe is located
	void *data_register_virtual;			//Virtual register contents
}IC_74HC595;

//Clear 74HC595 contents to 0
void ic_74HC595_clear(IC_74HC595 *const ic_74hc595);
//Setup 74hc595 according to default values
void ic_74HC595_defaultSetup(IC_74HC595 *const ic_74hc595);
//Enable or disable outputs according to -state-
void ic_74HC595_enableOutput(IC_74HC595 const*const ic_74hc595, uint8_t const state);
//Latch stored register contents to parallel output
void ic_74HC595_latchOutput(IC_74HC595 const*const ic_74hc595);
//Enable or disable master reset according to -state-
void ic_74HC595_masterReset(IC_74HC595 const*const ic_74hc595, uint8_t const state);
//Push a -byte- into the register stack. Returns popped data
uint8_t ic_74HC595_push_pop_byte(IC_74HC595 *const ic_74hc595, uint8_t const byte);
//Push and pop data from the register stack. Data returned will be placed in -buffer-
void ic_74HC595_push_pop(IC_74HC595 *const ic_74hc595, void *const buffer, size_t const buffer_size);
//Push data into the regiter stack
void ic_74HC595_push(IC_74HC595 *const ic_74hc595, void const*const buffer, size_t const buffer_size);
//Pop data from the register stack
void ic_74HC595_pop(IC_74HC595 *const ic_74hc595, void *const buffer, size_t const buffer_size);
//Allocates memory to be used for virtual stack
void ic_74HC595_allocateVirtualMemory(IC_74HC595 *const ic_74hc595);
