#include "../include/tedavr/button.h"

void button_clear(Button *const button) {
	button->flag_function_updateCurrent = 0;
	button->flag_function_updatePrevious = 0;
	button->flag_function_updatePushed = 0;
	button->flag_function_updateReleased = 0;
	button->flag_logic_idle = 0;
	button->flag_state_current = 0;
	button->flag_state_previous = 0;
	button->flag_state_pushed = 0;
	button->flag_state_released = 0;
	button->data_shift_portBit = 0;
	button->data_port_p = NULL;
}

void button_defaultSetup(Button *const button) {
	button->flag_function_updateCurrent = 1;
	button->flag_function_updatePrevious = 1;
	button->flag_function_updatePushed = 1;
	button->flag_function_updateReleased = 1;
	button->flag_logic_idle = 1;
	button->flag_state_current = 0;
	button->flag_state_previous = 0;
	button->flag_state_pushed = 0;
	button->flag_state_released = 0;
	button->data_shift_portBit = 0;
	button->data_port_p = NULL;
}

void button_update(Button *const button) {
	if (button->flag_function_updatePrevious)
		button->flag_state_previous = button->flag_state_current;
	if (button->flag_function_updateCurrent)
		if(button->data_port_p != NULL)
			button->flag_state_current = (*(button->data_port_p) & _BV(button->data_shift_portBit)) ? (1 - (button->flag_logic_idle)) : (0 + (button->flag_logic_idle));	//Probs a better way to do...
	if (button->flag_function_updatePushed)
		button->flag_state_pushed = (button->flag_state_current && (!(button->flag_state_previous))) ? 1 : 0;
	if (button->flag_function_updateReleased)
		button->flag_state_released = ((!(button->flag_state_current)) && button->flag_state_previous) ? 1 : 0;
}
