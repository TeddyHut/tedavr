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

#endif


uint8_t buttonGet_attrib_released(Button const*const button) {
	return(general_GetBitFlag_u8(&(button->attrib), BUTTON_ATTRIB_RELEASED));
}

uint8_t buttonGet_attrib_portBit(Button const*const button) {
	return((button->attrib & BUTTON_PORT_BIT_MASK)>>BUTTON_PORT_BIT_SHIFT);
}

volatile uint8_t* buttonGet_port(Button const*const button) {
	return(button->port);
}

void button_clear(Button*const button) {
	button->attrib = 0;
}

void button_setDefault(Button*const button) {
	button_clear(button);
	buttonSet_attrib_auto(button, 1);
}

void button_runover(Button*const button) {
	button_runoverSegment0(button);
	button_runoverSegment1(button);
	button_runoverSegment2(button);
}

void button_runoverSegment0(Button*const button) {
	if(buttonGet_attrib_auto(button))
		buttonSet_attrib_currentState(button, bit_is_set(*(button->port), buttonGet_attrib_portBit(button)));
}

void button_runoverSegment1(Button*const button) {
	buttonSet_attrib_pushed(button, ((!buttonGet_attrib_previousState(button)) && buttonGet_attrib_currentState(button)));
	buttonSet_attrib_released(button, (buttonGet_attrib_previousState(button) && (!buttonGet_attrib_currentState(button))));
}

void button_runoverSegment2(Button*const button) {
	buttonSet_attrib_previousState(button, buttonGet_attrib_currentState(button));
}
