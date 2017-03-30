#include "../include/tedavr/ic_74hc595.h"

static void risingEdgePulse(IC_74HC595 const *const ic_74hc595, IC_74HC595_Pin_e const pin) {
	ic_74HC595_setPin(ic_74hc595, pin, 0);
	ic_74HC595_setPin(ic_74hc595, pin, 1);
	ic_74HC595_setPin(ic_74hc595, pin, 0);
}

static void fallingEdgePulse(IC_74HC595 const *const ic_74hc595, IC_74HC595_Pin_e const pin) {
	ic_74HC595_setPin(ic_74hc595, pin, 1);
	ic_74HC595_setPin(ic_74hc595, pin, 0);
	ic_74HC595_setPin(ic_74hc595, pin, 1);
}

void ic_74HC595_setPin(IC_74HC595 const *const ic_74hc595, IC_74HC595_Pin_e const pin, uint8_t const state) {
	//Could maybe use a union instead of general_whatever
	switch (pin) {
	case IC_74HC595_Pin_sero:
		general_SetBitFlag_u8(ic_74hc595->data_port_sero_p, ic_74hc595->data_shift_sero, state);
		break;
	case IC_74HC595_Pin_clr:
		general_SetBitFlag_u8(ic_74hc595->data_port_clr_p, ic_74hc595->data_shift_clr, state);
		break;
	case IC_74HC595_Pin_shck:
		general_SetBitFlag_u8(ic_74hc595->data_port_shck_p, ic_74hc595->data_shift_shck, state);
		break;
	case IC_74HC595_Pin_stck:
		general_SetBitFlag_u8(ic_74hc595->data_port_stck_p, ic_74hc595->data_shift_stck, state);
		break;
	case IC_74HC595_Pin_oe:
		general_SetBitFlag_u8(ic_74hc595->data_port_oe_p, ic_74hc595->data_shift_oe, state);
		break;
	default:
		break;
	}
}

uint8_t ic_74HC595_getPin(IC_74HC595 const *const ic_74hc595, IC_74HC595_Pin_e const pin) {
	switch (pin) {
	case IC_74HC595_Pin_seri:
		return(general_GetBitFlag_u8(ic_74hc595->data_port_seri_p, ic_74hc595->data_shift_seri));
		break;
	default:
		break;
	}
	return(0);
}

void ic_74HC595_clear(IC_74HC595 *const ic_74hc595) {
	ic_74hc595->flag_function_virtual = 0;
	ic_74hc595->flag_connected_seri = 0;
	ic_74hc595->data_shift_sero = 0;
	ic_74hc595->data_shift_clr = 0;
	ic_74hc595->data_shift_shck = 0;
	ic_74hc595->data_shift_stck = 0;
	ic_74hc595->data_shift_seri = 0;
	ic_74hc595->data_register_size = 0;
	ic_74hc595->data_port_seri_p = NULL;
	ic_74hc595->data_port_clr_p = NULL;
	ic_74hc595->data_port_shck_p = NULL;
	ic_74hc595->data_port_stck_p = NULL;
	ic_74hc595->data_port_seri_p = NULL;
	ic_74hc595->data_port_oe_p = NULL;
	ic_74hc595->data_register_virtual = NULL;
}

void ic_74HC595_defaultSetup(IC_74HC595 *const ic_74hc595) {
	ic_74hc595->flag_function_virtual = 0;
	ic_74hc595->flag_connected_seri = 1;
}

void ic_74HC595_enableOutput(IC_74HC595 const *const ic_74hc595, uint8_t const state) {
	ic_74HC595_setPin(ic_74hc595, IC_74HC595_Pin_oe, !state);
}

void ic_74HC595_latchOutput(IC_74HC595 const *const ic_74hc595) {
	risingEdgePulse(ic_74hc595, IC_74HC595_Pin_stck);
}

void ic_74HC595_clearContents(IC_74HC595 const *const ic_74hc595) {
	fallingEdgePulse(ic_74hc595, IC_74HC595_Pin_clr);
}

uint8_t ic_74HC595_push_pop_byte(IC_74HC595 const *const ic_74hc595, uint8_t const byte) {
	uint8_t rtrn = 0;
	for (uint8_t i = 0; i < 8; i++) {
		rtrn |= (ic_74HC595_getPin(ic_74hc595, IC_74HC595_Pin_seri) << i);
		ic_74HC595_setPin(ic_74hc595, IC_74HC595_Pin_sero, byte & _BV(i));
		risingEdgePulse(ic_74hc595, IC_74HC595_Pin_shck);
	}
	if (ic_74hc595->flag_function_virtual) {
		for (size_t i = 0; i < ic_74hc595->data_register_size; i++) {
			((uint8_t*)(ic_74hc595->data_register_virtual))[i] = ((uint8_t*)(ic_74hc595->data_register_virtual))[i + 1];
		}
		((uint8_t*)(ic_74hc595->data_register_virtual))[ic_74hc595->data_register_size - 1] = rtrn;
	}
	return(rtrn);
}

void ic_74HC595_push_pop(IC_74HC595 const *const ic_74hc595, void *const buffer, size_t const buffer_size) {
	for (size_t i = 0; i < buffer_size; i++) {
		((uint8_t*)buffer)[i] = ic_74HC595_push_pop_byte(ic_74hc595, ((uint8_t*)buffer)[i]);
	}
}

void ic_74HC595_push_byte(IC_74HC595 const *const ic_74hc595, uint8_t const byte) {
	ic_74HC595_push_pop_byte(ic_74hc595, byte);
}

void ic_74HC595_push(IC_74HC595 const *const ic_74hc595, void const *const buffer, size_t const buffer_size) {
	for (size_t i = 0; i < buffer_size; i++) {
		ic_74HC595_push_pop_byte(ic_74hc595, ((uint8_t*)buffer)[i]);
	}
}

uint8_t ic_74HC595_pop_byte(IC_74HC595 const *const ic_74hc595) {
	return(ic_74HC595_push_pop_byte(ic_74hc595, 0));
}

void ic_74HC595_pop(IC_74HC595 const *const ic_74hc595, void *const buffer, size_t const buffer_size) {
	for (size_t i = 0; i < buffer_size; i++) {
		((uint8_t*)buffer)[i] = ic_74HC595_push_pop_byte(ic_74hc595, 0);
	}
}

void ic_74HC595_allocateVirtualMemory(IC_74HC595 *const ic_74hc595) {
	ic_74hc595->data_register_virtual = malloc(ic_74hc595->data_register_size);
}